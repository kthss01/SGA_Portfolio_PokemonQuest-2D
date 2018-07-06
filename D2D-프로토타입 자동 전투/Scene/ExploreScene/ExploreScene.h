#pragma once

#include "Scene\Scene.h"
#include "GameObject\StageInfo.h"

class ExploreScene : public Scene
{
private:
	class Camera* mainCamera;

	class TileMap* tile;

	Json::Value* jsonTileMap;
	Json::Value* jsonStageInfo;

	vector<tagStageInfo> stageInfo;
	int curStage;
	int curTeam;
	int teamCount;

	//class Rect* circle;

	//class Pokemon* pokemon;
	//class Pokemon* pokemon2;
	//class Pokemon* enemy;
	//class Pokemon* enemy2;

	class Pokemon** pokemon; // 일단 4명으로 고정
	class Pokemon** enemy;

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
	void StageInit();
	void PokemonInit();

	void PokemonSetting();

	void DebugInit();
	void DebugRender();

	void MapLoad();
	void StageLoad();

	void FindPokemon();
	Pokemon* FindNearPokemon(Pokemon* curPokemon, bool isEnemy = true);

	void UpdateCameraChange(Vector2 tileScale, Vector2 pokemonScale);
	void UpdatePokemonChange(class Pokemon* pokemon, Vector2 scale);
};