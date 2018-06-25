#include "stdafx.h"
#include "Json.h"

void Json::GetValue(Json::Value & parent, string name, float & vec)
{
	vec = parent[name.c_str()].asFloat();
}

void Json::SetValue(Json::Value & parent, string name, float & vec)
{
	parent[name.c_str()] = vec;
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
