#pragma once
class String
{
public:
	String();
	~String();

	// ���� ����� �ϸ� �ݴ�ε� �ٲ� �� ����
	static wstring StringToWString(string value);
	static string WStringToString(wstring value);
};

