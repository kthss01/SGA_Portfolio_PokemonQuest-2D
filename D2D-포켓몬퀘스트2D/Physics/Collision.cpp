#include "stdafx.h"
#include "Collision.h"

#include "./Common/Transform.h"
#include "./Physics/RectCollider.h"

Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 mousePos)
{
	Vector2 cross;	// ������ ����ϴ� ���� vector

	float dotA, dotB;	// ���� ����

	Vector2 temp[3];	// ����� �༮, ���� ������

	temp[0].x = p2.x - p0.x;
	temp[0].y = p2.y - p0.y;

	temp[1].x = p1.x - p0.x;
	temp[1].y = p1.y - p0.y;

	temp[2].x = mousePos.x - p0.x;
	temp[2].y = mousePos.y - p0.y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	temp[0].x = p0.x - p1.x;
	temp[0].y = p0.y - p1.y;

	temp[1].x = p2.x - p1.x;
	temp[1].y = p2.y - p1.y;

	temp[2].x = mousePos.x - p1.x;
	temp[2].y = mousePos.y - p1.y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	temp[0].x = p1.x - p2.x;
	temp[0].y = p1.y - p2.y;

	temp[1].x = p0.x - p2.x;
	temp[1].y = p0.y - p2.y;

	temp[2].x = mousePos.x - p2.x;
	temp[2].y = mousePos.y - p2.y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	return true;
}

bool Collision::IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, Transform* transform, Vector2 mousePos)
{
	Vector2 cross;	// ������ ����ϴ� ���� vector

	float dotA, dotB;	// ���� ����

	Vector2 temp[3];	// ����� �༮, ���� ������

						// ���� ����̹Ƿ� view projection �ʿ�
	D3DXMATRIX matView;
	D3DXMATRIX matProjection;

	D2D::GetDevice()->GetTransform(D3DTS_VIEW, &matView);
	D2D::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);

	Matrix view = Matrix(matView);
	Matrix proj = Matrix(matProjection);
	Matrix world = transform->GetFinalMatrix();

	Vector2 point[3];
	point[0] = p0.TransformCoord(world);
	point[1] = p1.TransformCoord(world);
	point[2] = p2.TransformCoord(world);

	for (int i = 0; i < 3; i++) {
		point[i] = point[i].TransformCoord(view);
		point[i] = point[i].TransformCoord(proj);
	}

	temp[0].x = point[2].x - point[0].x;
	temp[0].y = point[2].y - point[0].y;

	temp[1].x = point[1].x - point[0].x;
	temp[1].y = point[1].y - point[0].y;

	temp[2].x = mousePos.x - point[0].x;
	temp[2].y = mousePos.y - point[0].y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	temp[0].x = point[0].x - point[1].x;
	temp[0].y = point[0].y - point[1].y;

	temp[1].x = point[2].x - point[1].x;
	temp[1].y = point[2].y - point[1].y;

	temp[2].x = mousePos.x - point[1].x;
	temp[2].y = mousePos.y - point[1].y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	temp[0].x = point[1].x - point[2].x;
	temp[0].y = point[1].y - point[2].y;

	temp[1].x = point[0].x - point[2].x;
	temp[1].y = point[0].y - point[2].y;

	temp[2].x = mousePos.x - point[2].x;
	temp[2].y = mousePos.y - point[2].y;

	// 2D���� ����ϴ� ���� 
	// temp[0]�� ������ ���Ͱ� ������� (�븻����, ��������)
	cross.x = -temp[0].y;
	cross.y = temp[0].x;

	// dot < 0 �а��̶�� 180���� ����
	// dot�� ���� 90���� ������ > 0 180���� ũ�� ����
	dotA = Vector2::Dot(cross, temp[1]);
	dotB = Vector2::Dot(cross, temp[2]);

	// �� ����� ��� �ﰢ�� ������ �����Ե�
	// ��� ���� ��� �� ���ָ� �ǰ�
	if (dotA * dotB < 0)
		return false;

	return true;
}

bool Collision::IntersectRayToLine(Ray ray, Vector2 start, Vector2 end, Vector2 * outPos, Vector2 * outNormal)
{
	Vector2 normal;
	normal.x = -(end.y - start.y);
	normal.y = (end.x - start.x);

	// ������ �� ���� �ְ�
	// ������ �����ε� �� �� ����
	// normal �� ���������� �ִ� �Ÿ� ���Ѱ�
	float dist = -Vector2::Dot(normal, end);
	float t1 = -Vector2::Dot(normal, ray.origin);
	float t2 = Vector2::Dot(normal, ray.direction);
	float t = (t1 - dist) / t2;

	// ������ ������ ��Ȳ�� �� (����, ���� �� �� ����)
	if (t >= 0.0f && t <= 1.0f) {
		if (outPos != NULL) {
			*outPos = ray.origin + ray.direction * t;

			if (outNormal != NULL) {
				*outNormal = normal * -1;
			}
		}
		return true;
	}
	return false;
}

bool Collision::IsOverlap(Transform * pTransA, CircleCollider * pBoundA, Transform * pTransB, CircleCollider * pBoundB)
{
	Vector2 centerA;
	float radiusA;
	Vector2 centerB;
	float radiusB;

	// ���� ������ �־ ���� ó�� ���ص� ��
	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	Vector2 dirTo = centerB - centerA;

	float dist = dirTo.Length();

	float distPow = (dist * dist);
	float sumRadiusPow = radiusA + radiusB;
	sumRadiusPow *= sumRadiusPow;

	if (distPow > sumRadiusPow) return false;
	return true;
}

bool Collision::IsOverlap(Transform * pTransA, RectCollider * pBoundA, Transform * pTransB, RectCollider * pBoundB)
{
	// Rect�� ���ΰ� �ִ� ������ �浹 ���� üũ
	if (!Collision::IsOverlap(
		pTransA, (CircleCollider*)pBoundA,
		pTransB, (CircleCollider*)pBoundB)) return false;

	Vector2 centerA;
	float radiusA;
	Vector2 centerB;
	float radiusB;

	pBoundA->GetWorldCenterRadius(pTransA, &centerA, &radiusA);
	pBoundB->GetWorldCenterRadius(pTransB, &centerB, &radiusB);

	struct OBB {
		Vector2 center;
		Vector2 axis[2];
		float halfLength[2];
	};

	OBB obbA;
	obbA.axis[0] = pTransA->GetRight();
	obbA.axis[1] = pTransA->GetUp();

	obbA.center = centerA;
	Vector2 scaleA = pTransA->GetScale();
	obbA.halfLength[0] = pBoundA->GetHalfSize().x * scaleA.x;
	obbA.halfLength[1] = pBoundA->GetHalfSize().y * scaleA.y;

	OBB obbB;
	obbB.axis[0] = pTransB->GetRight();
	obbB.axis[1] = pTransB->GetUp();

	obbB.center = centerB;
	Vector2 scaleB = pTransB->GetScale();
	obbB.halfLength[0] = pBoundB->GetHalfSize().x * scaleB.x;
	obbB.halfLength[1] = pBoundB->GetHalfSize().y * scaleB.y;

	// OBB �浹
	float cos[2][2]; // ���� ���� ���� �ڻ��� �� [A��][B��]
	float absCos[2][2]; // ���밪
	float dist[2]; // �߽����� ������ ���̰� 

				   // A���� B�� ���� ����
	Vector2 D = obbB.center - obbA.center;

	float r, r0, r1;
	// r D�� ������ �� 
	// r0 A���� ������ ��, r1 B���� ������ ��
	// r > r0 + r1 �̸� �浹 ����

	for (int a = 0; a < 2; a++) {
		for (int b = 0; b < 2; b++) {
			cos[a][b] = Vector2::Dot(obbA.axis[a], obbB.axis[b]);
			absCos[a][b] = abs(cos[a][b]);
		}

		// ���ͳ����� ���⺤�͸� A�ٿ��
		// Axis�� ������ ����
		dist[a] = Vector2::Dot(obbA.axis[a], D);
		// Dot �������� ����ϰų�(���� �ΰ� �����ְų�) 
		// cos ���� ���ϰų� �� �� ����
	}

	// �߽����� ���ؼ� �浹 �Ǿ����� Ȯ��

	r = abs(dist[0]);
	r0 = obbA.halfLength[0];
	/*
	Ǯ� �����ϸ� �̷��� �Ǵµ� �̸� ����ص�
	r1 = abs(Vector2::Dot(obbA.axis[0], obbB.axis[0] * obbB.halfLength[0]))
	+ abs(Vector2::Dot(obbA.axis[0], obbB.axis[1] * obbB.halfLength[1]))
	//+ abs(Vector2::Dot(obbA.axis[0], obbB.axis[0] * obbB.halfLength[2]))
	*/
	r1 = absCos[0][0] * obbB.halfLength[0] +
		absCos[0][1] * obbB.halfLength[1];

	if (r > r0 + r1) return false;

	// �ٸ� �൵ ���־����
	r = abs(dist[1]);
	r0 = obbA.halfLength[1];
	r1 = absCos[1][0] * obbB.halfLength[0] +
		absCos[1][1] * obbB.halfLength[1];

	if (r > r0 + r1) return false;

	// �ݴ� �ʵ� ���־����
	r = abs(Vector2::Dot(obbB.axis[0], D));
	r0 = absCos[0][0] * obbA.halfLength[0] +
		absCos[1][0] * obbA.halfLength[1];
	r1 = obbB.halfLength[0];

	if (r > r0 + r1) return false;

	r = abs(Vector2::Dot(obbB.axis[1], D));
	r0 = absCos[0][1] * obbA.halfLength[0] +
		absCos[1][1] * obbA.halfLength[1];
	r1 = obbB.halfLength[1];

	if (r > r0 + r1) return false;

	// �������ε� Ȯ���غ��� �� (�浹�� �ƴѵ� �浹�ȴٰ� ������ ��찡 ����)
	// 3D���� �߽����� ������µ� ��ġ�� �ʴ� ��찡 ����ٰ� �Ͻ�
	// 2D������ z���� ��� ������ �ȵȴٰ� �Ͻ�

	return true;
}