#include "stdafx.h"
#include "Gizmo.h"

Gizmo* Gizmo::instance = NULL;

Gizmo::Gizmo()
{
	float intervalAngle = (D3DX_PI * 2.0f) / CIRCLEGIZMO_SEGMENTS;

	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++) {
		float angle = i * intervalAngle;
		circlePos[i] = Vector2(cosf(angle), sinf(angle));
	}
}

Gizmo::~Gizmo()
{
}

Gizmo * Gizmo::Get()
{
	if (instance == NULL)
		instance = new Gizmo;
	return instance;
}

void Gizmo::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

// ���� �̵��ϰų� ȸ���ϴ��� �߾�(����)�� �پ��־�� ��
void Gizmo::Line(const Vector2 & startPos, const Vector2 & endPos, DWORD color) const
{
	D3DXMATRIX matPrevWorld;
	D2D::GetDevice()->GetTransform(D3DTS_WORLD, &matPrevWorld);

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matIdentity);

	// light�� ���⵵ ������ �ȵ�
	// ����� ������ �ִ� ���� �� �ǹ̰� ����
	// x(red), y(green), z(blue)�� ����ϱ�� �Ϲ������� �Ǿ����� 
	DWORD prevLight = 0;

	D2D::GetDevice()->GetRenderState(D3DRS_LIGHTING, &prevLight);

	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	GIZMOVERTEX vertices[2];
	vertices[0].position = startPos;
	vertices[1].position = endPos;
	vertices[0].color = color;
	vertices[1].color = color;

	// �׸���
	// ���������� �̷��� �׸��� ������ ���ϰ� ������
	// �ʱ�ȭ ������Ʈ ���� 3���� �����ؼ� �� �� �ְ�
	// ������ ���� �� ���� �ϰ� ���� (�� �����Ӹ��� �� �ع�����)
	D2D::GetDevice()->SetFVF(GIZMOVERTEX::FVF);
	D2D::GetDevice()->DrawPrimitiveUP(
		D3DPT_LINELIST, 1, vertices, sizeof(GIZMOVERTEX));

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matPrevWorld);
	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, prevLight);
}

// centerPos ���� ȸ���� ���� �ʰ� position���� ������ (worldPosition���� ���� ����)
void Gizmo::Circle(Vector2 & centerPos, float radius, DWORD color) const
{
	D3DXMATRIX matPrev;
	// Transform �̶� TS RenderState RS
	D2D::GetDevice()->GetTransform(D3DTS_WORLD, &matPrev);

	DWORD prevLight;
	D2D::GetDevice()->GetRenderState(D3DRS_LIGHTING, &prevLight);

	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	GIZMOVERTEX vertices[CIRCLEGIZMO_SEGMENTS + 1];

	for (int i = 0; i <= CIRCLEGIZMO_SEGMENTS; i++) {
		vertices[i].position = circlePos[i];
		vertices[i].color = color;
	}

	// scale �� ����
	// 3d���� �ٲ�� �Ǹ� ������ �ٲ�Ե� ȸ���� ����
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 right = D3DXVECTOR3(1, 0, 0);

	// 1,0,0	0,0,1	0,1,0
	// �о�� ������ �ϸ� �ȴٰ� �� x��, y��, z�� ���� ����ߵ�
	forward *= radius;
	up *= radius;
	right *= radius;

	D3DXMATRIX matWorld;
	D3DXMatrixIdentity(&matWorld);

	// 11, 12, 13 ���� ����
	memcpy(&matWorld._11, right, sizeof(D3DXVECTOR3));
	// 21, 22, 23 ���� ����
	memcpy(&matWorld._21, up, sizeof(D3DXVECTOR3));
	// 31, 32, 33 ���� ����
	memcpy(&matWorld._31, forward, sizeof(D3DXVECTOR3));

	matWorld._41 = centerPos.x;
	matWorld._42 = centerPos.y;

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld);
	// [0][1][2][3]
	// strip = [0][1], [1][2], [2][3], [3][4] ��ĭ �� �з����鼭 �׸��°�
	// list = [0][1], [2][3], [4][5],
	D2D::GetDevice()->DrawPrimitiveUP(
		D3DPT_LINESTRIP, CIRCLEGIZMO_SEGMENTS,
		vertices, sizeof(GIZMOVERTEX)
	);

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matPrev);
	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, prevLight);
}

void Gizmo::AABBBox(const Vector2 & minPos, const Vector2 & maxPos, DWORD color) const
{
	this->Line(Vector2(minPos.x, minPos.y), Vector2(maxPos.x, minPos.y), color);
	this->Line(Vector2(maxPos.x, minPos.y), Vector2(maxPos.x, maxPos.y), color);
	this->Line(Vector2(maxPos.x, maxPos.y), Vector2(minPos.x, maxPos.y), color);
	this->Line(Vector2(minPos.x, maxPos.y), Vector2(minPos.x, minPos.y), color);
}
