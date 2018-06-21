#pragma once

#include "GameObject.h"

#define TILE_ROW 100
#define TILE_COL 100

#define TILE_WIDTH 25
#define TILE_HEIGHT 25

#define TILE_SIZE (TILE_ROW * TILE_COL)
// ���� ��ġ�� �ߺ��Ǵ� uv ��ǥ ������ 1 Ÿ�� �� 4���� vertex�� �ʿ���
#define VERTEX_SIZE (TILE_SIZE * 4)
#define INDEX_SIZE (TILE_SIZE * 2 * 6) 


struct tagTile {
	// ���� Ÿ�� ��ȣ
	int x, y;
	// �浹 üũ�� ���� �� Ÿ���� 6���� ��ġ�� ���� �־����
	Vector2 position[6];
};

class TileMap : public GameObject {
private:
	struct Vertex {
		Vector2 position;
		DWORD color;
		Vector2 uv;
	};
	Vertex* vertices;
	DWORD* indices;

	LPDIRECT3DTEXTURE9 pTex;
	LPD3DXEFFECT pEffect;

	class Camera* camera;

	tagTile tiles[TILE_ROW][TILE_COL];
public:
	TileMap();
	~TileMap();

	void Init(wstring shaderFile, const Vector2 uv,
		const Vector2 size = Vector2(25, 25), const Vector2 pivot = Vector2(0, 0));
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void RenderRect();

	void InitShader(wstring shaderFile);
	void InitVertex(Vector2 size, Vector2 uv, Vector2 pivot);
	void InitBuffer();

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex) { pTex = tex; }
};