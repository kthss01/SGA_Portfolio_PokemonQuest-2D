#include "stdafx.h"
#include "Program.h"

#include "Scene\TestScene.h"
#include "Scene\ExploreScene\ExploreScene.h"
#include "Scene\ExploreScene\MapTool.h"

#include "GameObject\Rect.h"
#include "./Common/Camera.h"
#include "./Common/ParticleEmitter.h"

Program::Program()
{
	srand(time(NULL));
	D2D::GetDevice()->SetRenderState(
		// 라이트 지정
		D3DRS_LIGHTING,
		// 사용 여부
		FALSE
	);

	// SOUND Setting
	{
		//SOUND->AddSound("Test", "sounds/영전3.wav", true, true);
	}
	// TEXTURE Setting
	{
		// Tile
		TEXTURE->AddTexture(L"tile_forest", L"Textures/tile_Forest.png");

		// UI
		TEXTURE->AddTexture(L"ui_panel", L"Textures/UI_Panel_Green.png");
		
		TEXTURE->AddTexture(L"ui_hp_player", L"Textures/hp_player.png");
		TEXTURE->AddTexture(L"ui_hp_ally", L"Textures/hp_ally.png");
		TEXTURE->AddTexture(L"ui_hp_enemy", L"Textures/hp_enemy.png");
		TEXTURE->AddTexture(L"ui_bar", L"Textures/bar.png");

		TEXTURE->AddTexture(L"ui_new",  L"Textures/UI_New.png");
		TEXTURE->AddTexture(L"ui_save", L"Textures/UI_Save.png");
		TEXTURE->AddTexture(L"ui_load", L"Textures/UI_Load.png");
		TEXTURE->AddTexture(L"ui_up",	L"Textures/UI_Up.png");
		TEXTURE->AddTexture(L"ui_down", L"Textures/UI_Down.png");

		TEXTURE->AddTexture(L"ui_circle", L"Textures/circle.png");

		// Pokemon
		// Pikachu
		TEXTURE->AddTexture(L"pikachu_idle",				L"Textures/pokemon/pikachu/idle_8x1.png");
		TEXTURE->AddTexture(L"pikachu_movement",			L"Textures/pokemon/pikachu/movement_24x1.png");
		TEXTURE->AddTexture(L"pikachu_hurt",				L"Textures/pokemon/pikachu/hurt_8x1.png");
		TEXTURE->AddTexture(L"pikachu_attack",				L"Textures/pokemon/pikachu/attack_8x1.png");
		TEXTURE->AddTexture(L"pikachu_special_attack",		L"Textures/pokemon/pikachu/special_attack_16x1.png");

		// Charmander
		TEXTURE->AddTexture(L"charmander_idle",				L"Textures/pokemon/charmander/idle_8x1.png");
		TEXTURE->AddTexture(L"charmander_movement",			L"Textures/pokemon/charmander/movement_24x1.png");
		TEXTURE->AddTexture(L"charmander_hurt",				L"Textures/pokemon/charmander/hurt_8x1.png");
		TEXTURE->AddTexture(L"charmander_attack",			L"Textures/pokemon/charmander/attack_24x1.png");
		TEXTURE->AddTexture(L"charmander_special_attack",	L"Textures/pokemon/charmander/special_attack_8x1.png");
	}

	// SPRITE Setting
	{
		D3DXCreateSprite(D2D::GetDevice(), &sprite);
	}

	// SCENE Setting
	{
		TestScene * test = new TestScene;
		SCENE->AddScene("Test", test);
		ExploreScene * explore = new ExploreScene;
		SCENE->AddScene("Explore", explore);
		MapTool * mapTool = new MapTool;
		SCENE->AddScene("MapTool", mapTool);
	}
	
	// 기타 Setting
	{
		//SOUND->Play("Test");
		//SCENE->ChangeScene("Test");
		SCENE->ChangeScene("Explore");
		//SCENE->ChangeScene("MapTool");

		isDebug = false;
	}
	// JSON Setting
	{
		//root = new Json::Value();
		//readJson = new Json::Value();
		//float time = FRAME->GetElapsedTime();
		//Json::SetValue(*root, "test", time);

		//Json::WriteJsonData(L"Test.Json", root);
		//Json::ReadJsonData(L"Test.Json", readJson);

		//float temp;
		//Json::GetValue(*readJson, "test", temp);
	}
}

Program::~Program()
{
	TEXTURE->Release();
	SCENE->Release();

	SAFE_DELETE(readJson);
	SAFE_DELETE(root);

	SAFE_DELETE(particle);

	SAFE_RELEASE(sprite);
}

void Program::Update()
{
	SCENE->Update();

	if (INPUT->GetKeyDown(VK_F11))
		isDebug = !isDebug;
}

void Program::Render()
{
	SCENE->Render();

	//Vector2 center = Vector2(0, 0);

	//GIZMO->Circle(
	//	center, 100.0f, 0xFF808000
	//);

	//particle->Render();

	FRAME->Render();

	if (isDebug) {

	}
}