#pragma once
// Json.h

namespace Json {
	void GetValue(Json::Value& parent, string name, float& vec);
	void GetValue(Json::Value& parent, string name, int& vec);
	void GetValue(Json::Value& parent, string name, bool& vec);
	void GetString(Json::Value & parent, string name, string & str);
	void GetString(Json::Value & parent, string name, wstring & str);

	void SetValue(Json::Value& parent, string name, float& vec);
	void SetValue(Json::Value& parent, string name, int& vec);
	void SetValue(Json::Value& parent, string name, bool& vec);
	void SetString(Json::Value& parent, string name, string& str);
	void SetString(Json::Value& parent, string name, wstring& str);

	void WriteJsonData(wstring fileName, Json::Value* root);
	void ReadJsonData(wstring fileName, Json::Value* root);
}