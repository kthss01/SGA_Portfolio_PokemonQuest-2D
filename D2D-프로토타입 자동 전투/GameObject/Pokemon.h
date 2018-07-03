#pragma once
#include "GameObject.h"

enum POKEMON_STATE {
	STATE_IDLE,
	STATE_MOVE,
	STATE_HURT,
	STATE_ATTACK,
	STATE_SPECIAL_ATTACK,
	STATE_END
};

enum POKEMON_DIRECTION {
	DIRECTION_BOTTOM,
	DIRECTION_TOP,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	DIRECTION_LEFTBOTTOM,
	DIRECTION_RIGHTBOTTOM,
	DIRECTION_LEFTTOP,
	DIRECTION_RIGHTTOP
};

struct tagPokemonInfo {
	wstring name;
	int frameCnt[STATE_END];
	LPDIRECT3DTEXTURE9 pTex[STATE_END];

	POKEMON_STATE state;
	POKEMON_DIRECTION dir;

	float moveSpeed;
	float attackSpeed;
	float attackRange;

	POINT curTile;
	POINT targetTile;

	bool isDied;
};

#define CIRCLE_SEGMENTS 36

class Pokemon : public GameObject
{
private:
	struct Vertex {
		Vector2 position;
		DWORD color;
		Vector2 uv;
	};
	Vertex vertice[4];

	LPD3DXEFFECT pEffect;
	float deltaTime;
	float delayTime;

	class Camera* camera;
	class AnimationClip* clips[STATE_END];

	tagPokemonInfo pokemonInfo;

	class TileMap* tile;

	class Transform* targetTransform;
	class Transform* tempTransform;

	class AStar* aStar;

	class Pokemon* enemy;

	class ProgressBar* hp;

	bool isHurt;
public:
	Pokemon();
	~Pokemon();

	void Init(wstring name, int* frameCnt, Vector2 pivot,
		POINT startPos = { 1,1 });
	void Release();
	void Update();
	void Render();
	void RenderRect();

	void InitShader();
	void InitVertex(Vector2 pivot = Vector2(0,0));
	void InitBuffer();

	void SetCamera(Camera* camera) { this->camera = camera; }

	bool MovePosition(POINT targetTile);
	POKEMON_DIRECTION FindDirection(POINT curTile, POINT targetTile);

	bool IsAttack();

	void Move();
	void Attack();

	void SetEnemy(Pokemon* enemy) { this->enemy = enemy; }
	void SetTileMap(TileMap* tile) { this->tile = tile; }

	void SetHurt(bool isHurt) { this->isHurt = isHurt; }

	tagPokemonInfo& GetPokemonInfo() { return pokemonInfo; }

	AStar* GetAStar() { return aStar; }

	void DrawAttackRange();
	
	void CaculateAttackRange();

	ProgressBar* GetHpBar() { return hp; }
};