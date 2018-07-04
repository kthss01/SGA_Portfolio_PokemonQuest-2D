#include "stdafx.h"
#include "GameManager.h"

GameManager* GameManager::instance = NULL;

GameManager::GameManager()
{
	this->Init();
}

GameManager::~GameManager()
{
	this->Release();
}

GameManager * GameManager::Get()
{
	if (instance == NULL)
		instance = new GameManager;

	return instance;
}

void GameManager::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

HRESULT GameManager::Init()
{
	readPokemonInfo = new Json::Value();

	return S_OK;
}

void GameManager::Release()
{
	SAFE_DELETE(readPokemonInfo);
}

void GameManager::ReadPokemonInfo()
{
	Json::ReadJsonData(L"Save/PokemonInfo.Json", readPokemonInfo);

	int cnt;
	Json::GetValue(*readPokemonInfo, "PokemonInfo_Count", cnt);

	for (int i = 0; i < cnt; i++) {
		string str = "PokemonInfo_";

		tagPokemonStatus status;

		string name;

		Json::GetString(*readPokemonInfo, str + "Name_" + to_string(i), name);

		status.name = String::StringToWString(name);

		this->pokemonName.push_back(status.name);

		// name에 대한 정보로 json에서 찾음
		string temp = str +  
			String::WStringToString(status.name);

		// state 마다의 frameCnt
		for (int j = 0; j < STATE_END; j++) {
			string temp2 = temp;

			switch (j)
			{
			case STATE_IDLE:
				temp2 += "_FrameCnt_IDLE";
				break;
			case STATE_MOVE:
				temp2 += "_FrameCnt_MOVE";
				break;
			case STATE_HURT:
				temp2 += "_FrameCnt_HURT";
				break;
			case STATE_ATTACK:
				temp2 += "_FrameCnt_ATTACK";
				break;
			case STATE_SPECIAL_ATTACK:
				temp2 += "_FrameCnt_SPECIAL_ATTACK";
				break;
			}

			Json::GetValue(*readPokemonInfo,
				temp2,
				status.frameCnt[j]);
		}

	
		Json::GetValue(*readPokemonInfo, temp + "_moveSpeed", status.moveSpeed);
		Json::GetValue(*readPokemonInfo, temp + "_attackSpeed", status.attackSpeed);
		Json::GetValue(*readPokemonInfo, temp + "_attackRangeForTile", status.attackRangeForTile);
		Json::GetValue(*readPokemonInfo, temp + "_attack", status.attack);
		Json::GetValue(*readPokemonInfo, temp + "_defense", status.defense);
		Json::GetValue(*readPokemonInfo, temp + "_maxHp", status.maxHp);
		Json::GetValue(*readPokemonInfo, temp + "_curHp", status.curHp);

		GAME->SetPokemonInfo(status.name, status);
	}

}
