#include "stdafx.h"
#include "ParticleEmitter.h"


ParticleEmitter::ParticleEmitter()
{
}


ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Init(DWORD particleNum, float emission, float liveTimeMin, float liveTimeMax, const Vector2 & velocityMin, const Vector2 & velocityMax, const Vector2 & accelMin, const Vector2 & accelMax, float startRotateMin, float startRotateMax, float rotateMin, float rotateMax, float rotateAccelMin, float rotateAccelMax, const VEC_COLOR & colors, const VEC_SCALE & scales, float scaleMin, float scaleMax, LPDIRECT3DTEXTURE9 pTex, bool bLocal)
{
	this->particleNum = particleNum;

	this->particleVertex = new Particle_Vertex[particleNum * 4];
	this->particleIndex = new DWORD[particleNum * 6];
	this->particles = new Particle[particleNum];

	this->fEmissionPerSec = emission;
	this->fEmissionIntervalTime = 1.0f / fEmissionPerSec;
	this->fEmissionDeltaTime = 0.0f;

	this->bEmission = false;

	this->colors.clear();
	for (int i = 0; i < colors.size(); i++) {
		this->colors.push_back(colors[i]);
	}

	this->scales.clear();
	for (int i = 0; i < scales.size(); i++) {
		this->scales.push_back(scales[i]);
	}

	this->fStartLiveTimeMin = liveTimeMin;
	this->fStartLiveTimeMax = liveTimeMax;

	this->startVelocityMin = velocityMin;
	this->startVelocityMax = velocityMax;

	this->startAccelateMin = accelMin;
	this->startAccelateMax = accelMax;

	this->startSetRotateMin = startRotateMin;
	this->startSetRotateMax = startRotateMax;

	this->startRotateMin = rotateMin;
	this->startRotateMax = rotateMax;

	this->startRotateAccelateMin = rotateAccelMin;
	this->startRotateAccelateMax = rotateAccelMax;

	this->fStartScaleMin = scaleMin;
	this->fStartScaleMax = scaleMax;

	this->dwParticleCount = 0;
	this->pTex = pTex;
	this->bLocal = bLocal;
}

void ParticleEmitter::Update()
{
	if (bEmission) {
		float delta = Frame::Get()->GetFrameDeltaSec();
		fEmissionDeltaTime += delta;
		while (fEmissionDeltaTime > fEmissionIntervalTime) {
			fEmissionDeltaTime -= fEmissionIntervalTime;
			StartOneParticle();
		}
	}
	for (int i = 0; i < particleNum; i++)
		particles[i].Update();
}

void ParticleEmitter::Render()
{
	// ������ ��ƼŬ ��
	DWORD drawParticleNum = 0;
	for (int i = 0; i < particleNum; i++) {
		if (particles[i].IsLive()) {
			particles[i].GetParticleVertex(
				&(particleVertex[drawParticleNum * 4]),
				&(particleIndex[drawParticleNum * 6]),
				colors,
				scales,
				drawParticleNum
			);
			drawParticleNum++;
		}
	}

	D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, false);
	// z ���� ���� �Ǹ� 2D���� ũ�� ��������� z���� ���� �־����ų� �۾����� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, false);

	// ������ �������� ���ư��°� ���� �ƴѰ� ���� X �� CULL MODE, D3DCULL_NONE �ϸ� ������ �׸��°�
	D2D::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//// ���ĺ��� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �����̶� �ؽ�ó ����
	D2D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	D2D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	D2D::GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

	Matrix matWorld;

	if (bLocal == false) {
		matWorld = Matrix::Identity(4);
	}
	else {
		matWorld = this->transform->GetFinalMatrix();
	}

	D2D::GetDevice()->SetTransform(D3DTS_WORLD, &matWorld.ToDXMatrix());

	D2D::GetDevice()->SetTexture(0, pTex);
	D2D::GetDevice()->SetFVF(Particle_Vertex::FVF);

	D2D::GetDevice()->DrawIndexedPrimitiveUP(
		D3DPT_TRIANGLELIST,
		0,
		drawParticleNum * 4,
		drawParticleNum * 2,
		particleIndex,
		D3DFMT_INDEX32,
		particleVertex,
		sizeof(Particle_Vertex)
	);

	//D2D::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	//// z ���� ���� �Ǹ� 2D���� ũ�� ��������� z���� ���� �־����ų� �۾����� ����
	//D2D::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, true);
	//// ������ �������� ���ư��°� ���� �ƴѰ� ���� X �� CULL MODE, D3DCULL_NONE �ϸ� ������ �׸��°�
	//// CCW ��������
	//D2D::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//// ���ĺ��� ����
	//D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	//D2D::GetDevice()->SetTexture(0, NULL);
}

void ParticleEmitter::StartEmission()
{
	this->bEmission = true;
}

void ParticleEmitter::StopEmission()
{
	this->bEmission = false;
}

void ParticleEmitter::StartOneParticle()
{
	// ���̺� Ÿ�� ����
	float liveTime = Util::RandomFloatRange(
		fStartLiveTimeMin, fStartLiveTimeMax);

	// �����ǵ� �����ϰ� �ϰ� ������ ���� �� ���� ��
	Vector2 position = transform->GetWorldPosition();
	if (this->bLocal == true) {
		position = Vector2(0, 0, 0);
	}

	Vector2 velocity;
	velocity.x = Util::RandomFloatRange(
		startVelocityMin.x, startVelocityMax.x);
	velocity.y = Util::RandomFloatRange(
		startVelocityMin.y, startVelocityMax.y);

	Vector2 accelation;
	accelation.x = Util::RandomFloatRange(
		startAccelateMin.x, startAccelateMax.x);
	accelation.y = Util::RandomFloatRange(
		startAccelateMin.y, startAccelateMax.y);

	float setRotate;
	setRotate = Util::RandomFloatRange(
		startSetRotateMin, startSetRotateMax);

	float rotate = Util::RandomFloatRange(
		startRotateMin, startRotateMax);

	float rotateAccel = Util::RandomFloatRange(
		startRotateAccelateMin, startRotateAccelateMax);

	if (bLocal == false) {
		Matrix matWorld = transform->GetFinalMatrix();

		velocity = velocity.TransformNormal(matWorld);
		accelation = accelation.TransformNormal(matWorld);
	}

	float scale = Util::RandomFloatRange(
		fStartScaleMin, fStartScaleMax);

	particles[dwParticleCount].transform->SetRotateLocal(setRotate);

	particles[dwParticleCount].Start(
		liveTime,
		&position,
		&velocity,
		&accelation,
		rotate,
		rotateAccel,
		scale
	);

	dwParticleCount++;
	if (dwParticleCount == this->particleNum)
		dwParticleCount = 0;
}
