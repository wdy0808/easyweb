#pragma once
#include <mysql.h>

const int CANVASLEN = 960000;

class CanvasData
{
public:
	CanvasData();
	CanvasData(std::string u, std::string c, MYSQL* db, std::string ctime, std::string mtime, std::string init = "");

	void record(std::string data);
	std::string generateJson(bool index);
	void writeBack();

private:
	void clean();
	std::string getNowTime();

	std::map<int, int> m_canvasValue;
	std::string m_user, m_canvas, m_ctime, m_mtime;
	MYSQL* m_db;
};

class Data
{
public:
	Data();
	~Data();

	static Data* getInstance();
	CanvasData* getCanvas(std::string jsondata);

private:
	bool connectDB();
	void initData();

	static Data* m_data;
	std::map<std::string, std::map<std::string, CanvasData> > m_canvas;
	MYSQL* m_db;
};

