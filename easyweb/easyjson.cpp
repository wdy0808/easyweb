#include "stdafx.h"
#include "easyjson.h"
#include <cstdlib>


EasyJson::EasyJson()
	: m_change(false), m_type(empty)
{
}

EasyJson::EasyJson(std::string json)
	: m_jsonStringValue(json), m_change(false)
{
	if (m_jsonStringValue.size() == 0)
	{
		m_type = empty;
		return;
	}
	switch (m_jsonStringValue[0])
	{
	case '{':
		m_type = object;
		m_jsonStringValue = m_jsonStringValue.substr(1, m_jsonStringValue.size() - 2);
		break;
	case '[':
		m_type = array;
		m_jsonStringValue = m_jsonStringValue.substr(1, m_jsonStringValue.size() - 2);
		break;
	case '\"':
		m_type = string;
		m_jsonStringValue = m_jsonStringValue.substr(1, m_jsonStringValue.size() - 2);
		return ;
	default:
		m_type = number;
		return ;
	}
	
	std::vector<std::string> jsonData = split();
	int index;
	std::string key, tem;
	for (auto i = jsonData.begin(); i != jsonData.end(); i++)
	{
		if (m_type == array)
			m_jsonArrayObject.push_back(EasyJson(*i));
		else
		{
			index = (*i).find(':');
			tem = (*i).substr(0, index);
			key = tem.substr(1, tem.size() - 2);
			m_jsonPairValue[key] = EasyJson((*i).substr(index + 1));
		}
	}
}


EasyJson::~EasyJson()
{
}

std::string EasyJson::toString()
{
	if (m_change && m_type == object)
	{
		m_jsonStringValue.clear();
		m_jsonStringValue = "{";
		for (auto i = m_jsonPairValue.begin(); i != m_jsonPairValue.end(); i++)
		{
			if (i != m_jsonPairValue.begin())
				m_jsonStringValue += ",";
			m_jsonStringValue += "\"" + i->first + "\"" + ":" + i->second.toString();
		}
		m_jsonStringValue += "}";
		m_change = false;
	}
	else if (m_type == array)
		m_jsonStringValue = "[" + m_jsonStringValue + "]";
	else if (m_type == string)
		m_jsonStringValue = "\"" + m_jsonStringValue + "\"";

	return m_jsonStringValue;
}

void EasyJson::add(std::string key, int value)
{
	if (getType() == number || getType() == string)
		return ;
	m_change = true;

	m_jsonPairValue[key] = EasyJson(std::to_string(value));
	if (m_type == empty)
		m_type = object;
}

void EasyJson::add(std::string key, std::string value)
{
	if (getType() == number || getType() == string)
		return;
	m_change = true;

	m_jsonPairValue[key] = EasyJson("\"" + value + "\"");
	if (m_type == empty)
		m_type = object;
}

void EasyJson::add(std::string key, EasyJson value)
{
	if (getType() == number || getType() == string)
		return;
	m_change = true;

	if (m_jsonPairValue.find(key) == m_jsonPairValue.end())
		m_jsonPairValue[key] = value;
	else
	{
		std::string tem = m_jsonPairValue[key].toString();
		if (m_jsonPairValue[key].getType() == object)
			m_jsonPairValue[key] = EasyJson("[" + tem + "," + value.toString() + "]");
		else if (m_jsonPairValue[key].getType() == array)
			m_jsonPairValue[key] = EasyJson(tem.substr(0, tem.size() - 1) + "," + value.toString() + "]");
	}
	if (m_type == empty)
		m_type = object;
}

std::string EasyJson::getString(std::string key)
{
	if (m_jsonPairValue.find(key) == m_jsonPairValue.end() || m_jsonPairValue[key].getType() != string)
		return "";

	std::string tem = m_jsonPairValue[key].toString();
	return tem.substr(1, tem.size() - 2);
}

int EasyJson::getNumber(std::string key)
{
	if (m_jsonPairValue.find(key) == m_jsonPairValue.end() || m_jsonPairValue[key].getType() != number)
		return -1;

	return atoi(m_jsonPairValue[key].toString().c_str());
}

EasyJson EasyJson::getObject(std::string key)
{
	if (m_jsonPairValue.find(key) == m_jsonPairValue.end())
		return EasyJson();

	return m_jsonPairValue[key];
}

EasyJson::iterator EasyJson::begin()
{
	return m_jsonArrayObject.begin();
}

EasyJson::iterator EasyJson::end()
{
	return m_jsonArrayObject.end();
}

JsonType EasyJson::getType()
{
	return m_type;
}

std::vector<std::string> EasyJson::split()
{
	if (m_jsonStringValue.empty())
		return std::vector<std::string>();
	std::vector<std::string> vec;
	int count = 0;
	std::string tem = "";
	for (int i = 0; i < m_jsonStringValue.size(); i++)
	{
		if (m_jsonStringValue[i] == ',' && count == 0)
		{
			vec.push_back(tem);
			tem.clear();
			continue;
		}
		if (m_jsonStringValue[i] == '[' || m_jsonStringValue[i] == '{')
			count++;
		else if (m_jsonStringValue[i] == ']' || m_jsonStringValue[i] == '}')
			count--;
		tem += m_jsonStringValue[i];
	}
	vec.push_back(tem);
	return vec;
}