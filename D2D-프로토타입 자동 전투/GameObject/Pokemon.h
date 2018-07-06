#pragma once
#include "GameObject.h"

#include "PokemonInfo.h"

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

	tagPokemonStatus pokemonStatus;

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

	void Init(wstring name, int* frameCnt, wstring team,
		Vector2 pivot, POINT startPos = { 1,1 });
	void Init(wstring name, wstring team,
		POINT startPos = { 1,1 }, Vector2 pivot = Vector2(13.0f, 0.0f));
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
	void SetDie(bool isDied) { this->pokemonStatus.isDied = isDied; }
	void SetHp(float damage);

	tagPokemonStatus& GetPokemonStatus() { return pokemonStatus; }

	AStar* GetAStar() { return aStar; }
	bool GetIsDied() { return pokemonStatus.isDied; }
	Pokemon* GetEnemy() { return enemy; }

	void DrawAttackRange();
	
	void CalculateAttackRange();

	ProgressBar* GetHpBar() { return hp; }
	void ChangeHpBar();

	//void UpdateCurTileBlocked(bool isBlocked = true);
};