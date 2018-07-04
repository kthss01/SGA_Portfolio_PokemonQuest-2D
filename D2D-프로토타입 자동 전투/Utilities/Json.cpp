#include "stdafx.h"
#include "Json.h"

void Json::GetValue(Json::Value & parent, string name, float & vec)
{
	vec = parent[name.c_str()].asFloat();
}

void Json::GetValue(Json::Value & parent, string name, int & vec)
{
	vec = parent[name.c_str()].asInt();
}

void Json::GetValue(Json::Value & parent, string name, bool & vec)
{
	vec = parent[name.c_str()].asBool();
}

void Json::GetString(Json::Value & parent, string name, string &str) {
	str = parent[name.c_str()].asString();
}

void Json::GetString(Json::Value & parent, string name, wstring & str)
{
	str = String::StringToWString(parent[name.c_str()].asString());
}

void Json::SetValue(Json::Value & parent, string name, float & vec)
{
	parent[name.c_str()] = vec;
}

void Json::SetValue(Json::Value & parent, string name, int & vec)
{
	parent[name.c_str()] = vec;
}

void Json::SetValue(Json::Value & parent, string name, bool & vec)
{
	parent[name.c_str()] = vec;
}

void Json::SetString(Json::Value & parent, string name, string & str)
{
	parent[name.c_str()] = str;
}

void Json::SetString(Json::Value & parent, string name, wstring & str)
{
	parent[name.c_str()] = String::WStringToString(str);
}

void Json::WriteJsonData(wstring fileName, Json::Value * root)
{
	ofstream stream;
	string temp = String::WStringToString(fileName);
	stream.open(temp);
	{
		Json::StyledWriter writer;
		stream << writer.write(*root);
	}
	stream.close();
}

void Json::ReadJsonData(wstring fileName, Json::Value * root)
{
	ifstream stream;
	stream.open(fileName);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}
