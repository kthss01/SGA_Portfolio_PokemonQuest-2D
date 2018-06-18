#pragma once

struct Particle_Vertex {
	Vector2 position;
	DWORD color;
	Vector2 uv;

	enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
};

typedef vector<D3DXCOLOR> VEC_COLOR;
typedef vector<float> VEC_SCALE;

// ���� �ϳ�
class Particle
{
public:
	class Transform* transform;
private:
	bool bLive; // Ȱ��ȭ ����
	float fTotalLiveTime; // ���� �ð�
	float fDeltaLiveTime; // ���ʱ��� ����Ǿ�����
						  // �����̳� ũ�Ⱚ ������ �� ����Ҳ�
	float fNormalizeLiveTime; // Ȱ��ȭ ���� �ð� (0 ~ 1) 1�̸� �������

	Vector2 velocity; // ���� �� �ӵ���
	Vector2 accelation; // ���ӵ�
	float rotate; // ó�� �� + ȸ���� degree ������ ����
	float rotateAccel; // ȸ�� ���ӵ�

	float fScale; // �⺻ ������ ��
public:
	Particle();
	~Particle();

	// ���� �ϳ��� ����
	void Start(
		float liveTime, // �󸶸�ŭ ȭ�鿡 ��� ������
		const Vector2* pos, // ���� ��ġ, �׻� ���� ��ġ�� �̹��Ͱ� �ƴ� ���� ����
		const Vector2* velo, // ���� �ӵ�
		const Vector2* accel, // ���ӵ�
		float rotate, // �����Ҷ� ȸ����
		float rotateAccel, // ȸ�� ������
		float scale // �⺻ ������ ��
	);

	void Update();

	// ������ �̹��Ϳ��� ���ٲ�
	// ���� ������� �ʰ� �����Ҳ�
	void GetParticleVertex(
		Particle_Vertex* pOut, // vertex buffer �迭
		DWORD* pIndex, // index buffer �迭
		const VEC_COLOR& colors,
		const VEC_SCALE& scales,
		DWORD particleNum // ���� ��ƼŬ�� � �߻��Ǿ����� 
	);

	bool IsLive() { return bLive; }
};

