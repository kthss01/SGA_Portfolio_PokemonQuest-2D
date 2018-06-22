#pragma once

#include "Scene\Scene.h"

class MapTool : public Scene {
private:
	class Camera* mainCamera;

	class TileMap* tile;

	LPD3DXFONT font;
	bool isDebug;
public:
	MapTool();
	~MapTool();

	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	void DebugInit();
	void DebugRender();
};