#pragma once

// MainGame대신 돌아갈꺼

class Program
{
private:
	class Camera* mainCamera;
	class Rect* fade;
	class ParticleEmitter* particle;

	LPD3DXSPRITE sprite;

	Json::Value* root;
	Json::Value* readJson;

	int fadeSwitch;
	D3DXCOLOR fadeOut; // 밝았던 화면 어두워지는 것 알파값 1로
	D3DXCOLOR fadeIn; // 어두웠던 화면 밝아지는 것 알파값 0으로

	bool isDebug;
public:
	Program();
	~Program();

	void Update();
	void Render();
};
