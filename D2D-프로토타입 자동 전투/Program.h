#pragma once

// MainGame��� ���ư���

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
	D3DXCOLOR fadeOut; // ��Ҵ� ȭ�� ��ο����� �� ���İ� 1��
	D3DXCOLOR fadeIn; // ��ο��� ȭ�� ������� �� ���İ� 0����

	bool isDebug;
public:
	Program();
	~Program();

	void Update();
	void Render();
};
