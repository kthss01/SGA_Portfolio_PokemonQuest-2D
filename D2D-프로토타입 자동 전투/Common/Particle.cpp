#include "stdafx.h"
#include "Particle.h"

#include "Common\Transform.h"

Particle::Particle()
{
	transform = new Transform;
	bLive = false;
}


Particle::~Particle()
{
	SAFE_DELETE(transform);
}

void Particle::Start(float liveTime, const Vector2 * pos, const Vector2 * velo,
	const Vector2 * accel, float rotate, float rotateAccel, float scale)
{
	this->bLive = true;

	this->fTotalLiveTime = liveTime;
	this->fDeltaLiveTime = 0.0f;

	this->transform->SetWorldPosition(*pos);

	this->velocity = *velo;
	this->accelation = *accel;
	this->rotate = rotate;
	this->rotateAccel = rotateAccel;
	this->fScale = scale;
}

void Particle::Update()
{
	if (bLive == false) return;

	if (fDeltaLiveTime > fTotalLiveTime)
		bLive = false;

	float deltaTime = FRAME->GetElapsedTime();

	fDeltaLiveTime += deltaTime;

	fNormalizeLiveTime = fDeltaLiveTime / fTotalLiveTime;

	if (fNormalizeLiveTime > 1.0f)
		fNormalizeLiveTime = 1.0f;

	// �̵� �� ȸ��
	transform->MovePositionWorld(velocity * deltaTime);
	transform->RotateSelf(rotate * deltaTime);

	this->velocity = velocity + accelation * deltaTime;
	this->rotate = rotate + rotateAccel * deltaTime;
}

// �÷��� ũ�Ⱚ�� ���⼭ ����
void Particle::GetParticleVertex(Particle_Vertex * pOut, DWORD * pIndex, const VEC_COLOR & colors, const VEC_SCALE & scales, DWORD particleNum)
{
	// ������ ��ƼŬ ��ġ��
	Vector2 center = transform->GetWorldPosition();
	DWORD dwColor = 0;
	float scale;

	// ������ ����
	float s = 0.0f;
	int lastIndex = scales.size() - 1;

	// ��������° �༮��
	if (fNormalizeLiveTime == 1.0f) {
		dwColor = colors[colors.size() - 1];
		scale = fScale * scales[scales.size() - 1];
	}
	else if (fNormalizeLiveTime == 0.0f) {
		dwColor = colors[0];
		scale = fScale * scales[0];
	}
	else {
		// �̰� �´��� �� Ȯ������ lastIndex�� �ƴ϶� ������ ������ ��
		float interval = 1.0f / lastIndex;

		// liveTime���� ��� ��ġ�� �ִ���
		float position = fNormalizeLiveTime / interval;

		int startIndex = (int)position;
		int endIndex = startIndex + 1;
		// ������ (0 ~ 1 ���̰�)
		float fN = position - startIndex;

		// �÷��� �����ϴ� �Լ� ���� �������� ��� ����ؾ���
		s = (scales[endIndex] - scales[startIndex]) * fN + scales[startIndex];

		scale = fScale * s;

		// �÷� ����
		lastIndex = colors.size() - 1;

		interval = 1.0f / lastIndex;

		position = fNormalizeLiveTime / interval;

		startIndex = (int)position;
		endIndex = startIndex + 1;
		fN = position - startIndex;

		D3DXCOLOR color;
		D3DXColorLerp(&color, &colors[startIndex], &colors[lastIndex], fN);

		dwColor = (DWORD)color;
	}

	// vertex info
	float halfSize = scale * 0.5f;
	Vector2 x = transform->GetRight();
	Vector2 y = transform->GetUp();

	(pOut + 0)->position = center + (x * -halfSize) + (y * -halfSize); // �޻�
	(pOut + 1)->position = center + (x *  halfSize) + (y * -halfSize); // ������
	(pOut + 2)->position = center + (x * -halfSize) + (y *  halfSize); // ����
	(pOut + 3)->position = center + (x *  halfSize) + (y *  halfSize); // ������

	(pOut + 0)->uv = Vector2(0, 0);
	(pOut + 1)->uv = Vector2(1, 0);
	(pOut + 2)->uv = Vector2(0, 1);
	(pOut + 3)->uv = Vector2(1, 1);

	(pOut + 0)->color = dwColor;
	(pOut + 1)->color = dwColor;
	(pOut + 2)->color = dwColor;
	(pOut + 3)->color = dwColor;

	*(pIndex + 0) = (particleNum * 4) + 0;
	*(pIndex + 1) = (particleNum * 4) + 1;
	*(pIndex + 2) = (particleNum * 4) + 2;
	*(pIndex + 3) = (particleNum * 4) + 2;
	*(pIndex + 4) = (particleNum * 4) + 1;
	*(pIndex + 5) = (particleNum * 4) + 3;
}
