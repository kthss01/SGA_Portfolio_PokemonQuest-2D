#pragma once

#include "Scene\Scene.h"

class ExploreScene : public Scene
{
private:
	class Camera* mainCamera;

	class TileMap* tile;

	Json::Value* jsonTileMap;

	class Pokemon* pokemon;
	class Pokemon* enemy;
	class Pokemon* enemy2;

	class Transform* tempTransform;
	bool cameraFollow;
	bool isChange;

	LPD3DXFONT font;
	bool isDebug;
public:
	ExploreScene();
	~ExploreScene();

	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;

	void TileInit();
	void PokemonInit();

	void DebugInit();
	void DebugRender();

	void MapLoad();

	void FindEnemyTile();
	Pokemon* FindNearPokemon(bool isEnemy = true);

	void UpdateCameraChange(Vector2 tileScale, Vector2 pokemonScale);
	void UpdatePokemonChange(class Pokemon* pokemon, Vector2 scale);
};