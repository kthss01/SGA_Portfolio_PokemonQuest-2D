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

	POKEMON_STATE state = STATE_IDLE; // �ʱ� ����
	POKEMON_DIRECTION dir = DIRECTION_BOTTOM; // �ʱ� ����

	float moveSpeed;
	float attackSpeed;
	// Ÿ�Ͽ� ���� attack Range �� ������ json���� �켱 ����
	int attackRangeForTile = 1; 
	// pokemon Ŭ�������� ������� Ÿ�Ͽ� ���� attack Range��
	float attackRange; 

	float attack;
	float defense;

	float maxHp;
	float curHp;

	POINT curTile;
	POINT targetTile;

	bool isDied = false; // �ʱ� �׾����� üũ
};