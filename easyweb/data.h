#pragma once

const int CANVASLEN = 960000;

class CanvasData
{
public:
	void record(std::string data);
	std::string generateJson();

private:
	void clean();

	std::map<int, int> m_canvasValue;
};

class Data
{
public:
	Data();
	~Data();

	static Data* getInstance();
	CanvasData* getCanvas(int id);

private:
	static Data* m_data;
	std::map<int, CanvasData> m_canvas;
};

