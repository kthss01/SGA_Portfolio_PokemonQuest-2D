#include "stdafx.h"
#include "CircleCollider.h"


CircleCollider::CircleCollider()
{
	// �浹 ��ü�� ��� ������ 
	transform = new Transform;
}


CircleCollider::~CircleCollider()
{
	SAFE_DELETE(transform);
}

// �ܺο� ��ȯ���ִ� �뵵
void CircleCollider::GetWorldCenterRadius(const Transform * trans, Vector2 * center, float * radius)
{
	Matrix matFinal = trans->GetFinalMatrix();

	*center = this->localCenter.TransformCoord(matFinal);
	Vector2 scale = trans->GetScale();
	Vector2 halfSize;
	halfSize.x = this->halfSize.x * scale.x;
	halfSize.y = this->halfSize.y * scale.y;

	*radius = halfSize.Length();
}

void CircleCollider::RenderGizmo(const Transform * trans)
{
	Vector2 center;
	float radius;

	this->GetWorldCenterRadius(trans, &center, &radius);

	GIZMO->Circle(center, radius, 0xffffff00);
}

// �ܺο��� �����ϴ� �뵵
void CircleCollider::SetBound(const Vector2 * pCenter, const Vector2 * pHalfSize)
{
	this->localCenter = *pCenter;
	this->halfSize = *pHalfSize;
	this->radius = pHalfSize->Length();
}
