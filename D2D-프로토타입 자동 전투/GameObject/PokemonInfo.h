#pragma once

#include "stdafx.h"

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

struct tagPokemonStatus {
	wstring name;
	int frameCnt[STATE_END];
	LPDIRECT3DTEXTURE9 pTex[STATE_END];

	POKEMON_STATE state = STATE_IDLE; // 초기 상태
	POKEMON_DIRECTION dir = DIRECTION_BOTTOM; // 초기 방향

	float moveSpeed;
	float attackSpeed;
	// 타일에 대한 attack Range 이 정보만 json으로 우선 관리
	int attackRangeForTile = 1; 
	// pokemon 클래스에서 계산해줌 타일에 대한 attack Range로
	float attackRange; 

	float attack;
	float defense;

	float maxHp;
	float curHp;

	POINT curTile;
	POINT targetTile;

	bool isDied = false; // 초기 죽었는지 체크
};