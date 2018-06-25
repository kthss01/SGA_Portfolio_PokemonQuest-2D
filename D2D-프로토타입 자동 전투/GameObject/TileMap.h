#pragma once

#include "GameObject.h"

#define TILE_ROW 32
#define TILE_COL 32

#define TILE_WIDTH 25
#define TILE_HEIGHT 25

#define TILE_SIZE (TILE_ROW * TILE_COL)
// ���� ��ġ�� �ߺ��Ǵ� uv ��ǥ ������ 1 Ÿ�� �� 4���� vertex�� �ʿ���
#define TILE_VERTEX_SIZE (TILE_SIZE * 4)
#define TILE_INDEX_SIZE (TILE_SIZE * 2 * 6) 

// �� Ÿ�� ����
#define TILE_MAXFRAME_X 27.0f
#define TILE_MAXFRAME_Y 24.0f

// �������� �� Ÿ�� 
#define TILE_INITFRAME_X 26.0f
#define TILE_INITFRAME_Y 23.0f

// Ÿ�� ���� �е� -> ���������� ������ ���ܽ�ų ũ��
#define TILE_PADDING 0.05f

struct tagTile {
	Vector2 center;
	Vector2 uv;

	bool block;
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

	tagTile tileInfo[TILE_ROW][TILE_COL];

	//Vector2 curTile;

	int row, col;
	int mapSize;
	int vertexSize, indexSize;
public:
	TileMap();
	~TileMap();

	void Init(wstring shaderFile, const Vector2 uv,
		int row, int col, int mapSize, int vertexSize, int indexSize,
		const Vector2 tileSize = Vector2(25, 25), const Vector2 pivot = Vector2(0, 0));
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void RenderRect();

	void InitShader(wstring shaderFile);
	void InitVertex(Vector2 tileSize, Vector2 uv, Vector2 pivot);
	void InitBuffer();

	void SetCamera(Camera* camera) { this->camera = camera; }
	void SetTexture(LPDIRECT3DTEXTURE9 tex) { pTex = tex; }

	//void UpdateUV();
	Vertex* GetVertices() { return vertices; }
	void SetVertexBuffer();
	void UpdateTileInfo();

	void ChangeTile(int row, int col, Vector2 uv, bool isStart = false);
	tagTile GetTileInfo(int row, int col) { return tileInfo[row][col]; }
	void SetTileInfo(int row, int col, tagTile tileInfo) { this->tileInfo[row][col] = tileInfo; }

	bool CheckTileBlock(Vector2 uv);
};