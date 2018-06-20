#pragma once

#include "GameObject/GameObject.h"
#include "Particle.h"

class ParticleEmitter : public GameObject
{
private:
	DWORD particleNum; // ��ƼŬ�� ���� �� ���ڷ�
	Particle* particles; // ��ƼŬ ���� �Է����η� ������ �� ������ particleVertex�� ������

	Particle_Vertex* particleVertex;
	DWORD* particleIndex;

	float fEmissionPerSec; // 1�ʿ� � �����Ҳ���
	DWORD dwParticleCount; // Ȱ��ȭ ����

	VEC_COLOR colors;
	VEC_SCALE scales;

	float fStartLiveTimeMin;
	float fStartLiveTimeMax;

	Vector2 startVelocityMin;
	Vector2 startVelocityMax;

	Vector2 startAccelateMin;
	Vector2 startAccelateMax;

	float startSetRotateMin;
	float startSetRotateMax;

	float startRotateMin;
	float startRotateMax;

	float startRotateAccelateMin;
	float startRotateAccelateMax;

	float fStartScaleMin;
	float fStartScaleMax;

	bool bEmission; // ���� ������
	float fEmissionIntervalTime; // ���� ����
	float fEmissionDeltaTime; // ������ �ð�

	LPDIRECT3DTEXTURE9 pTex;

	bool bLocal;
public:
	ParticleEmitter();
	~ParticleEmitter();

	void Init(
		DWORD particleNum, float emission,
		float liveTimeMin, float liveTimeMax,
		const Vector2& velocityMin, const Vector2& velocityMax,
		const Vector2& accelMin, const Vector2& accelMax,
		float startRotateMin, float startRotateMax,
		float rotateMin, float rotateMax,
		float rotateAccelMin, float rotateAccelMax,
		const VEC_COLOR& colors, const VEC_SCALE& scales,
		float scaleMin, float scaleMax,
		LPDIRECT3DTEXTURE9 pTex, bool bLocal = false
	);

	void Update();
	void Render();

	void StartEmission();
	void StopEmission();

	void SetRenderLocal(bool bLocal) { this->bLocal = bLocal; }

private:
	void StartOneParticle(); // ��ƼŬ ���� �ϳ� ����
};

