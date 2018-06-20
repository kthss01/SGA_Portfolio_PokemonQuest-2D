#include "stdafx.h"
#include "Util.h"

Util::Util()
{
}


Util::~Util()
{
}

void Util::GetMousePos(Vector2 * out)
{
	// API �Լ��� ���ؼ� �޾ƿ� ��
	POINT cursorPos;

	GetCursorPos(&cursorPos);	// ���� ������ â�� �������� ������

	ScreenToClient(g_hWnd, &cursorPos);

	// ���콺 �������� ȭ�鿡 ����� ���������� ����Ҳ�
	Vector2 temp;

	// 0 ~ 1 ���� ������ ���� ����
	temp.x = (float)cursorPos.x / (float)WINSIZE_X;
	// y���� D3D�� �ϰ� �־ ���� +�̰� �Ʒ��� -�̾ �������� ���� ��
	temp.y = 1.0f - (cursorPos.y / (float)WINSIZE_Y);

	// x���� ���� ������ ������ -1 �������� 1�̶� ���ϰ� ���ذ�
	temp.x = temp.x * 2.0f - 1.0f;
	temp.y = temp.y * 2.0f - 1.0f;

	// �̷��� �ϸ� ȭ�� �߽��� 0,0 ���� -1 ������ 1 �� 1 �Ʒ� -1��

	*out = temp;
}

void Util::GetMousePosWithScreen(Vector2 * out)
{
	POINT mousePos;

	GetCursorPos(&mousePos);

	ScreenToClient(g_hWnd, &mousePos);

	out->x = mousePos.x - WINSIZE_X / 2;
	out->y = mousePos.y - WINSIZE_Y / 2;
}

float Util::GetRandomSeed()
{
	int seed = rand() % 1001;

	float result = (float)seed / 1000.0f;

	return result;
}

float Util::RandomFloatRange(float min, float max)
{
	// 0 ~ 1000���� �޾ƿ;� �ؼ� 1001�� ����
	float factor = (rand() % 1001) * 0.001f;
	float delta = (max - min);
	float result = (delta * factor) + min;

	return result;
}

float Util::Clamp(float value, float min, float max)
{
	if (value < min) return min;
	else if (value > max) return max;

	return value;
}

float Util::Clamp01(float value)
{
	return Util::Clamp(value, 0.0f, 1.0f);
}
