#pragma once

#include "stdafx.h"

#define PLAYERCOUNT 4 // 포켓몬 등장수 4명으로 고장
#define MAX_ENEMYCOUNT 16 // 최대 등장가능한 야생 포켓몬 갯수

// 각 팀 정보 포켓몬 구성과 각 포켓몬 위치를 저장
// 한 팀은 4명으로 구성 예정
struct tagTeamInfo {
	int count = 4; // 팀 구성원
	wstring name[4]; // 포켓몬 이름
	POINT pos[4]; // 포켓몬의 위치 타일 행렬로 저장
};

struct tagStageInfo {
	wstring name; // stage 명 

	int pokemonCount; // 등장하는 포켓몬 수
	POINT startPos[PLAYERCOUNT]; // 등장하는 포켓몬의 위치

	int enemyCount; // 등장하는 야생 포켓몬 갯수
	int enemyKind; // 등장하는 야생 포켓몬 종류
	vector<wstring> enemyName; // 등장하는 야생 포켓몬 이름

	int teamCount; // 등장하는 팀 갯수
	int curTeam = 0; // 현재 팀
	vector<tagTeamInfo> teams;
};