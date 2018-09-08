#pragma once

enum JsonType{object, array, number, string, empty};
class EasyJson
{
public:
	typedef std::vector<EasyJson>::iterator iterator;

	EasyJson();
	EasyJson(std::string json);
	~EasyJson();

	std::string toString();

	void add(std::string key, int value);
	void add(std::string key, std::string value);
	void add(std::string key, EasyJson value);

	std::string getString(std::string key = "");
	int getNumber(std::string key = "");
	EasyJson getObject(std::string key);

	iterator begin();
	iterator end();

	JsonType getType();

private:
	std::vector<std::string> split();

	std::string m_jsonStringValue;
	std::map<std::string, EasyJson> m_jsonPairValue;
	std::vector<EasyJson> m_jsonArrayObject;
	JsonType m_type;
	bool m_change;
};

