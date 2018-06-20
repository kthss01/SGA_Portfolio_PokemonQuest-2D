#include "stdafx.h"
#include "TimeManager.h"

#define Test 

TimeManager* TimeManager::instance = NULL;

TimeManager::TimeManager()
{
}


TimeManager::~TimeManager()
{
}

TimeManager * TimeManager::Get()
{
	if (instance == NULL)
		instance = new TimeManager;
	return instance;
}

void TimeManager::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

HRESULT TimeManager::Init()
{
	_frame = new Frame;
	_frame->Init();

	HRESULT hr = D3DXCreateFont(
		D2D::GetDevice(),
		15,					// 너비
		0,					// 높이
		FW_BOLD,			// 두께, 두껍게 쓰겠다는거
		1,					// 이 둘은 크게 바꿀 꺼 없음
		FALSE,				// 이탤릭체 사용 여부
		DEFAULT_CHARSET,	// 이 둘은 크게 바꿀 꺼 없음
		OUT_DEFAULT_PRECIS,	// 외곽선 처리
		DEFAULT_QUALITY,	// 퀄리티
							// FF_DONTCARE 글자 크기 상관없이 큰 녀석 기준으로 잡아주는거 너비랑 높이 중에
		DEFAULT_PITCH || FF_DONTCARE,		// 옵션, 
		L"굴림",
		&font
	);
	assert(SUCCEEDED(hr));

	return S_OK;
}

void TimeManager::Release()
{
	if (_frame != NULL)
		SAFE_DELETE(_frame);
}

void TimeManager::Update(float lock)
{
	if (_frame != NULL) {
		_frame->Tick(lock);
	}
}

void TimeManager::Render()
{
	RECT rc = { 10, 10, 0,0 };

	wstring str;
	str = L"FPS : ";
	str += to_wstring(_frame->GetFrameRate());

	// 우리가 사용하는 if문이랑 동일 하다고 보면 됨
	// _DEBUG 비주얼에서 미리 만든 조건문이라고 보면됨
	// 개발 상태가 Debug라고 보면됨
#ifdef _DEBUG
	if (_frame != NULL) {
		// 멀티바이트면 DrawTextA
		font->DrawTextW(
			// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
			NULL,
			str.c_str(),
			-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
			&rc,
			// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
			// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
			DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
								 // 0x~~ 이거 귀찮으면 함수도 있음
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);

		rc.top += 20;
		str = L"WorldTime : ";
		str += to_wstring(_frame->GetWorldTime());

		// 멀티바이트면 DrawTextA
		font->DrawTextW(
			// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
			NULL,
			str.c_str(),
			-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
			&rc,
			// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
			// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
			DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
								 // 0x~~ 이거 귀찮으면 함수도 있음
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);

		rc.top += 20;
		str = L"ElapsedTime : ";
		str += to_wstring(_frame->GetElapsedTime());

		// 멀티바이트면 DrawTextA
		font->DrawTextW(
			// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
			NULL,
			str.c_str(),
			-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
			&rc,
			// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
			// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
			DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
								 // 0x~~ 이거 귀찮으면 함수도 있음
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}
#else
	if (_frame != NULL) {
		// 멀티바이트면 DrawTextA
		font->DrawTextW(
			// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
			NULL,
			str.c_str(),
			-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
			&rc,
			// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
			// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
			DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
								 // 0x~~ 이거 귀찮으면 함수도 있음
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}
#endif
}
