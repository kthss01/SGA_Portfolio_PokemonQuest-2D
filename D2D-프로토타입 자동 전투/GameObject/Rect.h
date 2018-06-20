#pragma once
#include "GameObject.h"

#define TEMPSIZE 10

class Rect : public GameObject
{
private:
	struct Vertex {
		Vector2 position;
		DWORD color;
		Vector2 uv;
	};
	Vertex vertice[4];

	LPDIRECT3DTEXTURE9 pTex;
	LPD3DXEFFECT pEffect;
	float deltaTime;

	class Camera* camera;
	class AnimationClip* clips;

	Transform* tempTrans[TEMPSIZE];

	RectCollider* tempCollider;
	bool colliderCheck;

	bool check;
public:
	Rect();
	~Rect();

	void Init(wstring shaderFile, const Vector2 uv, const Vector2 pivot = Vector2(0, 0));
	void Release();
	void Update();
	void Render();
	void RenderRect();

	void InitShader(wstring shaderFile);
	void InitVertex(Vector2 uv, Vector2 pivot);
	void InitBuffer();

	void DrawInterface();

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex) { pTex = tex; }
};