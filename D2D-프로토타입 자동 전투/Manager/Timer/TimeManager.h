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
	// GameNode 상속 받지 않고 있어서 hdc 필요함
	// 만약 GetMemDC() 함수 상속 받고 싶으면 GameNode 다중 상속받으면 되나 
	// 다중 상속 안쓰는게 좋음
	void Render();

	float GetElapsedTime() const { return _frame->GetElapsedTime(); }
	float GetWorldTime() const { return _frame->GetWorldTime(); }
};

#define FRAME TimeManager::Get()

