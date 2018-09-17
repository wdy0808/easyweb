#include "stdafx.h"
#include "data.h"
#include "easyjson.h"
#include <chrono>

Data* Data::m_data = new Data();

CanvasData::CanvasData()
{

}

CanvasData::CanvasData(std::string u, std::string c, MYSQL* db, std::string ctime, std::string mtime, std::string init)
	: m_user(u), m_canvas(c), m_db(db), m_ctime(ctime), m_mtime(mtime)
{
	record(init);
}

void CanvasData::record(std::string data)
{
	EasyJson json = EasyJson(data);
	if (json.getType() == object)
		json = json.getObject("data");
	int key;
	for (auto i = json.begin(); i != json.end(); i++)
	{
		key = i->getNumber();
		i++;
		if (i != json.end())
			m_canvasValue[key] = i->getNumber();
	}
}

std::string CanvasData::generateJson(bool index)
{
	if (index)
		return "{\"username\":\"" + m_user + 
				"\",\"canvasname\":\"" + m_canvas + 
				"\",\"ctime\":\"" + m_ctime + 
				"\",\"mtime\":\"" + m_mtime + "\"}";

	clean();
	std::string jsonString = "[";
	for (auto i = m_canvasValue.begin(); i != m_canvasValue.end(); i++)
	{
		if (i != m_canvasValue.begin())
			jsonString += ",";
		jsonString += std::to_string(i->first) + "," + std::to_string(i->second);
	}
	jsonString += "]";
	return jsonString;
}

void CanvasData::clean()
{
	auto i = m_canvasValue.begin();
	bool flag = true;
	int pos;
	while (i != m_canvasValue.end())
	{
		flag = true;
		auto j = i;
		if (j->first % 4 == 0 && j->second == 255)
		{
			pos = j->first;
			j++;
			if (j != m_canvasValue.end() && j->first == pos + 1 && j->second == 255)
			{
				j++;
				if (j != m_canvasValue.end() && j->first == pos + 2 && j->second == 255)
				{
					flag = false;
					i = m_canvasValue.erase(i);
					i = m_canvasValue.erase(i);
					i = m_canvasValue.erase(i);
				}
			}
		}
		if (flag)
			i++;
	}
}

std::string CanvasData::getNowTime()
{
	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	struct tm* ptm = new struct tm;
	localtime_s(ptm, &tt);
	char date[20] = { 0 };
	sprintf_s(date, 20, "%d-%02d-%02d %02d:%02d:%02d",
		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);
	delete ptm;
	return std::string(date);
}

void CanvasData::writeBack()
{
	try {
		std::string sql = "update painting set data = '[";
		for (auto i = m_canvasValue.begin(); i != m_canvasValue.end(); i++)
		{
			if (i != m_canvasValue.begin())
				sql += ",";
			sql += std::to_string(i->first) + "," + std::to_string(i->second);
		}
		sql += "]', mtime = '" + getNowTime() + "' where username = '" + m_user + "' and name = '" + m_canvas + "'";

		if (mysql_query(m_db, sql.c_str()))
		{
			std::string err_string = mysql_error(m_db);

			if (err_string.empty())
				throw std::string("MySQL update is error!");
			else
				throw err_string;
		}
	}
	catch (std::string &error_msg)
	{
		std::cout << error_msg << std::endl;
	}
	catch (...)
	{
		std::cout << "MySQL operation is error!" << std::endl;
	}
}

Data::Data()
{
	if (connectDB())
		initData();
}


Data::~Data()
{
	mysql_close(m_db);
}

Data* Data::getInstance()
{
	return m_data;
}

CanvasData* Data::getCanvas(std::string jsondata)
{
	EasyJson json(jsondata);
	std::string username = json.getString("username"), canvasname = json.getString("canvasname");

	return &m_canvas[username][canvasname];
}

bool Data::connectDB()
{
	m_db = new MYSQL;
	try {
		if (!mysql_init(m_db))
			throw std::string("MySQL init is error!");
		if (!mysql_real_connect(m_db, "localhost", "root", "0101,wdy.", "easyweb", 3306, NULL, 0))
			throw std::string("MySQL connect is error!");
	}
	catch (std::string &error_msg)
	{
		return false;
		std::cout << error_msg << std::endl;
	}
	catch (...)
	{
		return false;
		std::cout << "MySQL operation is error!" << std::endl;
	}
	return true;
}

void Data::initData()
{
	try
	{
		std::string sql = "SELECT * from painting;";
		if (mysql_query(m_db, sql.c_str()))
		{
			std::string err_string = mysql_error(m_db);

			if (err_string.empty())
				throw std::string("MySQL query is error!");
			else
				throw err_string;
		}

		MYSQL_RES* result = mysql_store_result(m_db);
		MYSQL_FIELD *fields = mysql_fetch_fields(result);
		int username = 0, name = 1, data = 2, ctime = 3, mtime = 4;
		for (int i = 0; i < mysql_num_fields(result); i++)
		{
			std::string tem = fields[i].name;
			if (tem == "username")
				username = i;
			else if (tem == "name")
				name = i;
			else if (tem == "data")
				data = i;
			else if (tem == "ctime")
				ctime = i;
			else if (tem == "mtime")
				mtime = i;
		}

		while (MYSQL_ROW row = mysql_fetch_row(result))
			m_canvas[row[username]][row[name]] = CanvasData(row[username], row[name], m_db, row[ctime], row[mtime], row[data]);
	}
	catch (std::string &error_msg)
	{
		std::cout << error_msg << std::endl;
	}
	catch (...)
	{
		std::cout << "MySQL operation is error!" << std::endl;
	}
}