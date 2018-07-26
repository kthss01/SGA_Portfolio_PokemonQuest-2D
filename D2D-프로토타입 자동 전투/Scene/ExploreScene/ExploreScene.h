#pragma once

#include "Scene\Scene.h"
#include "GameObject\StageInfo.h"

#define UI_SIZE 8

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

	class Rect* exitBtn;
	class Rect* glassesBtn;

	class Rect* panel[UI_SIZE];
	class Rect* portrait[UI_SIZE];
	class Transform* tempPanelTransform[UI_SIZE];
	//class Transform* tempPortraitTransform[UI_SIZE];

	class Transform* curCameraTarget;

	class Pokemon** pokemon; // 일단 4명으로 고정
	class Pokemon** enemy;

	class Transform* tempCameraTransform;
	bool cameraFollow;
	bool isChange;

	LPD3DXFONT font;
	bool isDebug;

	bool isSceneChange;
	float deltaTime;
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
	void UIInit();

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