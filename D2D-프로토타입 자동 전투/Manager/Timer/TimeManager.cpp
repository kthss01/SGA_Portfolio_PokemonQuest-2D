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
		15,					// �ʺ�
		0,					// ����
		FW_BOLD,			// �β�, �β��� ���ڴٴ°�
		1,					// �� ���� ũ�� �ٲ� �� ����
		FALSE,				// ���Ÿ�ü ��� ����
		DEFAULT_CHARSET,	// �� ���� ũ�� �ٲ� �� ����
		OUT_DEFAULT_PRECIS,	// �ܰ��� ó��
		DEFAULT_QUALITY,	// ����Ƽ
							// FF_DONTCARE ���� ũ�� ������� ū �༮ �������� ����ִ°� �ʺ�� ���� �߿�
		DEFAULT_PITCH || FF_DONTCARE,		// �ɼ�, 
		L"����",
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

	// �츮�� ����ϴ� if���̶� ���� �ϴٰ� ���� ��
	// _DEBUG ���־󿡼� �̸� ���� ���ǹ��̶�� �����
	// ���� ���°� Debug��� �����
#ifdef _DEBUG
	if (_frame != NULL) {
		// ��Ƽ����Ʈ�� DrawTextA
		font->DrawTextW(
			// �̹��� 2D ��ǥ���� ���°� sprite��� ��
			NULL,
			str.c_str(),
			-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
			&rc,
			// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
			// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
			DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
								 // 0x~~ �̰� �������� �Լ��� ����
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);

		rc.top += 20;
		str = L"WorldTime : ";
		str += to_wstring(_frame->GetWorldTime());

		// ��Ƽ����Ʈ�� DrawTextA
		font->DrawTextW(
			// �̹��� 2D ��ǥ���� ���°� sprite��� ��
			NULL,
			str.c_str(),
			-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
			&rc,
			// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
			// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
			DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
								 // 0x~~ �̰� �������� �Լ��� ����
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);

		rc.top += 20;
		str = L"ElapsedTime : ";
		str += to_wstring(_frame->GetElapsedTime());

		// ��Ƽ����Ʈ�� DrawTextA
		font->DrawTextW(
			// �̹��� 2D ��ǥ���� ���°� sprite��� ��
			NULL,
			str.c_str(),
			-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
			&rc,
			// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
			// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
			DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
								 // 0x~~ �̰� �������� �Լ��� ����
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}
#else
	if (_frame != NULL) {
		// ��Ƽ����Ʈ�� DrawTextA
		font->DrawTextW(
			// �̹��� 2D ��ǥ���� ���°� sprite��� ��
			NULL,
			str.c_str(),
			-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
			&rc,
			// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
			// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
			DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
								 // 0x~~ �̰� �������� �Լ��� ����
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}
#endif
}
