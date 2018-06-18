#include "stdafx.h"
#include "Frame.h"
#include <stdio.h>

//timeGetTime �� �̿��ϱ����� ����� ���̺귯��
#include <MMSystem.h>
#pragma comment( lib, "winmm.lib" )		//winmm.lib �߰�

Frame* Frame::instance = NULL;

Frame::Frame( void )
{
	this->Init();
}

Frame::~Frame( void )
{
	this->Release();
}

Frame * Frame::Get()
{
	if (instance == NULL)
		instance = new Frame;
	return instance;
}

void Frame::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

HRESULT Frame::Init( void )
{
	//���� Ÿ�̸� ���� ����
	//QueryPerformanceFrequency �Լ��� �ʴ� ī��Ʈ ������ ���� ��´�.
	//���� Ÿ�̸� ���� �ȴٸ� TRUE �ȵȴٸ� FALSE
	if ( QueryPerformanceFrequency( (LARGE_INTEGER*) &m_nPreriodTime ) )
	{
		//���� Ÿ�̸Ӹ� �����Ѵٸ�...
		m_bHardWare = true;

		//1 ī��Ʈ�� �ʰ� �󸶳� �Ǵ���?
		m_TimeScaleSec = 1.0 / m_nPreriodTime;

		//�ʱ�ȭ ������ �ð��� LastTime ����...
		QueryPerformanceCounter( (LARGE_INTEGER*) &m_nLastTime );
	}

	else
	{
		//���� Ÿ�̸Ӹ� �������� �ʴ´ٸ�...
		m_bHardWare = false;

		//1000 �и������� ������ �ð����� ��´�.
		//timeGetTime �� GetTickCount ���� ���� �����ϴ�.
		m_nLastTime = timeGetTime();

		//1 ī��Ʈ�� �ʰ� �󸶳� �Ǵ���?
		m_TimeScaleSec = 1.0 / 1000.0;

	}

	m_TotalSec = 0.0f;
	m_FrameCountSec = 0.0f;
	m_FramePerSec = 0;
	m_FrameCount = 0;

	return S_OK;
}

void Frame::Release( void )
{
}

//�ž�����Ʈ �����ش�.
void Frame::UpdateTime( float frameLock )
{
	//���� �ð��� ��´�.
	if ( m_bHardWare )
		QueryPerformanceCounter( (LARGE_INTEGER*) &m_nCurTime );
	else
		m_nCurTime = timeGetTime();

	//���� �ð����� ������ ���
	m_nDeltaTime = m_nCurTime - m_nLastTime;


	//�������� ������ ���� �ʽð� ���
	m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;

	//������ ���� �����Ѵٸ�...( ������ ��Ű�� )
	if ( frameLock > 0.0f ) {

		//������ �ð�.
		float fLockTime = ( 1.0f / frameLock );

		while ( m_FrameDeltaSec < fLockTime )
		{
			//���� �ð��� ��´�.
			if ( m_bHardWare )
				QueryPerformanceCounter( (LARGE_INTEGER*) &m_nCurTime );
			else
				m_nCurTime = timeGetTime();

			//���� �ð����� �ݸ� ���
			m_nDeltaTime = m_nCurTime - m_nLastTime;

			//�������� ������ ���� �ʽð� ���
			m_FrameDeltaSec = m_nDeltaTime * m_TimeScaleSec;
		}

	}

	//���� �ð� ����
	m_nLastTime = m_nCurTime;

	//������ �ð� ����
	m_TotalSec += m_FrameDeltaSec;

	//������ �� ����
	m_FrameCountSec += m_FrameDeltaSec;

	//1�ʰ� �����ٸ�....
	if ( m_FrameCountSec >= 1.0 ) {
		m_FramePerSec = m_FrameCount;
		m_FrameCount = 0;
		m_FrameCountSec -= 1.0;
	}
	m_FrameCount++;


}

double Frame::GetNowRealTimeSec()
{
	__int64 nowTime;

	if (m_bHardWare)
		QueryPerformanceCounter((LARGE_INTEGER*)&nowTime);
	else
		nowTime = GetTickCount();

	return nowTime * m_TimeScaleSec;
}
