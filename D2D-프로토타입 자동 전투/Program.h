#pragma once

// MainGame��� ���ư���

class Program
{
private:
	class Camera* mainCamera;
	class Rect* rect;
	class ParticleEmitter* particle;

	LPD3DXSPRITE sprite;

	Json::Value* root;
	Json::Value* readJson;

	bool isDebug;
public:
	Program();
	~Program();

	void Update();
	void Render();
};
