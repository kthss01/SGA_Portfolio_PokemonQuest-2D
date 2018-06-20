#pragma once

#define RESET_POSITION 1
#define RESET_ROTATION 2
#define RESET_SCALE 4

#define AXIS_X 0
#define AXIS_Y 1
//#define AXIS_Z 2

class Transform
{
protected:
	// ���� �ּҷ� ���̱� ������ (�������� �ּҰ� ����) 
	// axis[0]�̳� right�� ���� ���� ȣ���ϰԵ�
	// axis[1]�̳� up�̳� ���� ���� ȣ���ϰԵ�
	union {
		struct {
			Vector2 axis[2];
		};
		struct {
			Vector2 right;
			Vector2 up;
		};
	};

	// ��ġ��
	Vector2 position;
	// ũ�Ⱚ
	Vector2 scale;

	// ������� (���� ���)
	Matrix matFinal;

	// ȸ���̳� ������ ���� �� �ڵ����� Update������ ���� 
	bool bAutoUpdate;

	// ���� ���
	Matrix matLocal;

	Transform* pParent;	// �����ϸ� ���� ��ķ� �ٲ�

	Transform* pFirstChild;	// ù��° �ڽ�

	Transform* pNextSibling; // ���� �θ� ������ �ִ� ����


public:
	Transform();
	~Transform();

	// Parents
	// �� �ڽ����� Transform ���̴°�
	void AddChild(Transform* pNewChild);

	// �θ� �������ִ� �༮
	void AttachTo(Transform* pParent);

	// �θ� �ڽ� ���� ���� �����ִ� �༮
	void ReleaseParent();

	// Transform ���� �ʱ�ȭ -1�̸� ��ü �ʱ�ȭ
	// flag �ɼ��̶� �ǹ̵� ����
	void Reset(int resetFlag = -1);

	// ���� ��ǥ ����
	void SetWorldPosition(Vector2 position);
	// �θ� ���� ��� ���� ��ǥ�� ����, �θ� ���� �� ���� ��ǥ�� ��ȯ
	void SetLocalPosition(Vector2 position);
	// ������ �̵��ϴ°�, �ڱ� �ڽ� ���� �������� �����̴°�
	void MovePositionSelf(Vector2 delta);
	void MovePositionWorld(Vector2 delta);
	void MovePositionLocal(Vector2 delta);

	// scale
	// ũ�Ⱑ ���� �����̶�� �����
	void SetScale(Vector2 scale);
	// ���� ������ �ִ� �����ϰ��� + �����ִ°�
	void SetScaling(Vector2 deltaScale);

	// rotate
	void RotateWorld(float angle);
	// �ڱ� �ڽ� ������ ȸ��
	void RotateSelf(float angle);
	// �θ���� ��� ���� ���� �θ� ������ �θ� ����
	void RotateLocal(float angle);

	// ���� ����� �־ ȸ���� �� �༮
	void SetRotateWorld(const Matrix matWorldRotate);
	void SetRotateWorld(D3DXQUATERNION& worldRotate);
	void SetRotateLocal(const Matrix matLocalRotate);
	void SetRotateLocal(float angle);

	// Rotation (����� ȸ�� == QUATERNION)

	// etc...
	// Ư�� ������ �ٶ󺸰Բ� ȸ��
	void LookPosition(Vector2 pos, Vector2 up = Vector2(0, 0, -1));

	void ScaleLerp(Transform* from, Transform* to, float t);

	// �ڽ��� ȸ������ from�� to ������ ȸ���� ��ŭ ȸ������(���� ����) ����
	void RotateSlerp(Transform* from, Transform* to, float t);

	// �ڽ��� ��ġ���� from�� to ������ ����ŭ ����(���� ����)
	void PositionLerp(Transform* from, Transform* to, float t);

	// ��� ������ ����
	void Interpolate(Transform* from, Transform* to, float t);

	void DefaultControl2();

	// Update()
	void UpdateTransform();

	// Get 
	Matrix GetFinalMatrix() const;	// ���� �� ���Ҷ��� �� ����

	Vector2 GetWorldPosition() const;

	// world ���� ���ΰ��� y���� 1 ���������ΰ��� x���� 1

	// axis ���δ� �޾ƿ��°�
	void GetUnitAxis(Vector2* pVecArr) const;
	// ������ ��ȣ�� axis�� �޾ƿ��°�
	Vector2 GetUnitAxis(int axisNum) const;

	Vector2 GetUp() const;
	Vector2 GetRight() const;

	Vector2 GetScale() const;
	Matrix GetWorldRotateMatrix();

	D3DXQUATERNION GetWorldRotateQuaternion();

	void DrawInterface();

	// apply ����� scale �� �Կ��� �����ؾ� ���� ���ϴ´°�
	// 3d���� ����� �۾Ƽ� �İ�� �� �־
	void RenderGizmo(bool applyScale = false);
};

