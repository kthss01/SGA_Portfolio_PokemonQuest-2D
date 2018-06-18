#pragma once
class Collision
{
public:
	Collision();
	~Collision();

	static bool IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 mousePos);
	// �����̴� ��ü�� ���� �浹 ���
	static bool IntersectTri(Vector2 p0, Vector2 p1, Vector2 p2, class Transform* transform, Vector2 mousePos);

	static bool IntersectRayToLine(Ray ray, Vector2 start, Vector2 end, Vector2* outPos = NULL, Vector2* outNormal = NULL);
	// trans �� ��ü , bound �浹�� ����
	// ������ �浹
	static bool IsOverlap(
		class Transform* pTransA, class CircleCollider* pBoundA,
		class Transform* pTransB, class CircleCollider* pBoundB);

	static bool IsOverlap(
		class Transform* pTransA, class RectCollider* pBoundA,
		class Transform* pTransB, class RectCollider* pBoundB);

};

