#include "stdafx.h"
#include "data.h"
#include "easyjson.h"

Data* Data::m_data = nullptr;

void CanvasData::record(std::string data)
{
	EasyJson json = EasyJson(data).getObject("data"), tem;
	for (auto i = json.begin(); i != json.end(); i++)
		m_canvasValue[i->getNumber("pos")] = i->getNumber("value");
}

std::string CanvasData::generateJson()
{
	clean();
	std::string jsonString = "{\"type\":\"init\",\"data\":[";
	for (auto i = m_canvasValue.begin(); i != m_canvasValue.end(); i++)
	{
		if (i != m_canvasValue.begin())
			jsonString += ",";
		jsonString += "{\"pos\":" + std::to_string(i->first);
		jsonString +=  ",\"value\":" + std::to_string(i->second) + "}";
	}
	jsonString += "]}";
	/*EasyJson jsonString;
	jsonString.add("type", "init");
	EasyJson jsonData;
	for (auto i = m_canvasValue.begin(); i != m_canvasValue.end(); i++)
	{
		jsonData.add("pos", i->first);
		jsonData.add("value", i->second);
		jsonString.add("data", jsonData);
	}
	std::cout << jsonString.toString() << std::endl;*/
	return jsonString;
}

void  CanvasData::clean()
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

Data::Data()
{
}


Data::~Data()
{
}

Data* Data::getInstance()
{
	if (m_data == nullptr)
		m_data = new Data();

	return m_data;
}

CanvasData* Data::getCanvas(int id)
{
	if (m_canvas.find(id) == m_canvas.end())
		m_canvas[id] = CanvasData();

	return &m_canvas[id];
}