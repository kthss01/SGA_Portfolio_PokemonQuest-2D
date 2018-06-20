#pragma once

#include "CircleCollider.h"

class RectCollider : public CircleCollider
{
protected:
	Vector2 localMinPos;
	Vector2 localMaxPos;
public:
	RectCollider();
	~RectCollider();

	// OBB �ڽ��� ������ ���°�
	void GetWorldBox(const Transform* trans, Vector2* outBoxPos);
	// ȸ������ ���� �ʴ� Rect
	// Rect���� min x,y �� ���� ���� �༮ max x,y �� ���� ū �༮ 
	void GetWorldAABBMinMax(const Transform* trans,
		Vector2 * min, Vector2 * max);

	virtual void RenderGizmo(const Transform* trans) override;
	virtual void SetBound(
		const Vector2* pCenter, const Vector2* pHalfSize) override;
};
