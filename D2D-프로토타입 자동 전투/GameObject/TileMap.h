#pragma once

#include "GameObject.h"

#define TILE_ROW 100
#define TILE_COL 100

#define TILE_WIDTH 25
#define TILE_HEIGHT 25

#define TILE_SIZE (TILE_ROW * TILE_COL)
// 같은 위치의 중복되는 uv 좌표 때문에 1 타일 당 4개의 vertex가 필요함
#define VERTEX_SIZE (TILE_SIZE * 4)
#define INDEX_SIZE (TILE_SIZE * 2 * 6) 

// 총 타일 갯수
#define TILE_MAXFRAME_X 27.0f
#define TILE_MAXFRAME_Y 24.0f

// 시작으로 쓸 타일 
#define TILE_INITFRAME_X 0.0f
#define TILE_INITFRAME_Y 0.0f

// 타일 간의 패딩 -> 프레임으로 나눌때 제외시킬 크기
#define TILE_PADDING 0.05f

struct tagTile {
	// 현재 타일 번호
	int x, y;
	// 충돌 체크를 위해 한 타일의 6개의 위치값 갖고 있어야함
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

	Vector2 curTile;
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

	void UpdateUV();
};