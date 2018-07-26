#pragma once

#include "Scene\Scene.h"

#define MAPTOOL_ROW 24
#define MAPTOOL_COL 3

#define MAPTOOL_WIDTH 25
#define MAPTOOL_HEIGHT 25

#define MAPTOOL_SIZE (MAPTOOL_ROW * MAPTOOL_COL)
// 같은 위치의 중복되는 uv 좌표 때문에 1 타일 당 4개의 vertex가 필요함
#define MAPTOOL_VERTEX_SIZE (MAPTOOL_SIZE * 4)
#define MAPTOOL_INDEX_SIZE (MAPTOOL_SIZE * 2 * 6) 

class MapTool : public Scene {
private:
	class Camera* mainCamera;

	class TileMap* tile;
	class TileMap* subTile;

	class Rect* panel;
	class Rect* ui[5];
	class Rect* exitBtn;

	float currentSubTileNum;
	Vector2 currentTileUV;

	Json::Value* writeJsonTileMap;
	Json::Value* readJsonTileMap;

	bool isChangeScene;
	float deltaTime;

	LPD3DXFONT font;
	bool isDebug;
public:
	MapTool();
	~MapTool();

	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	void TileInit();
	void SubTileInit();
	void UIInit();

	void DebugInit();
	void DebugRender();

	void MapInit();
	void MapSave();
	void MapLoad();


};