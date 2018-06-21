#pragma once

#include "Scene\Scene.h"

class ExploreScene : public Scene
{
private:
	class Camera* mainCamera;

	class TileMap* tile;
public:
	ExploreScene();
	~ExploreScene();

	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};