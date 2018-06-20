#pragma once

#include "./Common/Transform.h"

// collider �浹 ��� ���� ����
// ���� �簢�� �ٱ��ʿ� �ִ� ��

class CircleCollider
{
protected:
	Transform * transform;
	Vector2 localCenter;
	float radius;
	Vector2 halfSize;
public:
	CircleCollider();
	~CircleCollider();

	// �ܺο� ��ȯ���ִ� �뵵
	void GetWorldCenterRadius(
		const Transform* trans, Vector2* center, float* radius);
	virtual void RenderGizmo(const Transform* trans);
	// �ܺο��� �����ϴ� �뵵
	virtual void SetBound(
		const Vector2* pCenter, const Vector2* pHalfSize);
public:
	Vector2 GetHalfSize() { return halfSize; }
};

