#pragma once

#include "stdafx.h"

#define PLAYERCOUNT 4 // ���ϸ� ����� 4������ ����
#define MAX_ENEMYCOUNT 16 // �ִ� ���尡���� �߻� ���ϸ� ����

// �� �� ���� ���ϸ� ������ �� ���ϸ� ��ġ�� ����
// �� ���� 4������ ���� ����
struct tagTeamInfo {
	int count = 4; // �� ������
	wstring name[4]; // ���ϸ� �̸�
	POINT pos[4]; // ���ϸ��� ��ġ Ÿ�� ��ķ� ����
};

struct tagStageInfo {
	wstring name; // stage �� 

	int pokemonCount; // �����ϴ� ���ϸ� ��
	POINT startPos[PLAYERCOUNT]; // �����ϴ� ���ϸ��� ��ġ

	int enemyCount; // �����ϴ� �߻� ���ϸ� ����
	int enemyKind; // �����ϴ� �߻� ���ϸ� ����
	vector<wstring> enemyName; // �����ϴ� �߻� ���ϸ� �̸�

	int teamCount; // �����ϴ� �� ����
	int curTeam = 0; // ���� ��
	vector<tagTeamInfo> teams;
};