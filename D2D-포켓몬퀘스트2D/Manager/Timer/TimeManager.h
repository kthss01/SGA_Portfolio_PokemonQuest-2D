#pragma once

#include "Frame.h"

class TimeManager
{
private:
	Frame * _frame;

	static TimeManager* instance;
	TimeManager();
	~TimeManager();

	LPD3DXFONT font;
public:
	static TimeManager* Get();
	static void Delete();

	HRESULT Init();
	void Release();
	void Update(float lock = 0.0f);
	// GameNode ��� ���� �ʰ� �־ hdc �ʿ���
	// ���� GetMemDC() �Լ� ��� �ް� ������ GameNode ���� ��ӹ����� �ǳ� 
	// ���� ��� �Ⱦ��°� ����
	void Render();

	float GetElapsedTime() const { return _frame->GetElapsedTime(); }
	float GetWorldTime() const { return _frame->GetWorldTime(); }
};

#define FRAME TimeManager::Get()

