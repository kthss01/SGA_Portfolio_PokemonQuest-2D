#pragma once

// �̱������� ���� ����
#define CIRCLEGIZMO_SEGMENTS 36

// 0xff808080 �÷��� �� �߰���

// ������ �ٸ� ������Ʈ(����)�� ������ ������ �ȵ�

class Gizmo
{
private:
	typedef struct tagGIZMOVERTEX {
		Vector2 position;
		DWORD color;
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE };
	} GIZMOVERTEX, *LPGIZMOVERTEX;

private:
	static Gizmo* instance;
	// +1 �ϴ� ������ for�� ���� �� ������ �ϳ��� ������ �ȵǼ� �ѹ� �� �ϴ°�
	Vector2 circlePos[CIRCLEGIZMO_SEGMENTS + 1];

	Gizmo();
	~Gizmo();
public:
	static Gizmo* Get();
	static void Delete();

	// �� �׸���
	void Line(const Vector2& startPos, const Vector2& endPos, DWORD color = 0xff808080) const;

	void Circle(Vector2& centerPos, float radius, DWORD color = 0xff808080) const;

	void AABBBox(const Vector2& minPos, const Vector2& maxPos, DWORD color = 0xff808080) const;

	// ���� �׸��� 3D���� ���̰� (���� Ȯ���� ����), 2D���� �󸶸�ŭ �������ִ��� 
	// ���ڹ��̷� ���ִ°� -> ������ ������ �׸��� �� 
};

#define GIZMO Gizmo::Get()
