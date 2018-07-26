#pragma once

#include "GameObject\PokemonInfo.h"

class GameManager {
private:
	Json::Value* readPokemonInfo;

	vector<wstring> pokemonName;
	map<wstring, tagPokemonStatus> pokemonInfo;
private:
	GameManager();
	~GameManager();

	static GameManager* instance;
	int* fadeSwitch;
public:
	static GameManager* Get();
	static void Delete();

	HRESULT Init();
	void Release();

	void SetPokemonInfo(wstring name, tagPokemonStatus status) {
		pokemonInfo[name] = status;
	}

	vector<wstring> GetPokemonName() { return pokemonName; }
	wstring GetPokemonName(int index) { 
		if (pokemonName.size() > index)
			return pokemonName[index];
		else
			return NULL;
	}
	map<wstring, tagPokemonStatus>& GetPokemonInfo() {
		return pokemonInfo;
	}
	tagPokemonStatus FindPokemonInfo(wstring name) {
		return pokemonInfo[name];
	}

	void ReadPokemonInfo();

	void SetFadeSwitch(int* fadeSwitch) { this->fadeSwitch = fadeSwitch; }
	void ChangeFadeSwitch(int _fadeSwitch) { *fadeSwitch = _fadeSwitch; }
	int* GetFadeSwitch() { return fadeSwitch; }
};

#define GAME GameManager::Get()