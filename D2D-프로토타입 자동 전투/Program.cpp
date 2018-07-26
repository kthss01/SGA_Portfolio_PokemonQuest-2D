#include "stdafx.h"
#include "Program.h"

#include "Scene\TestScene.h"
#include "Scene\ExploreScene\ExploreScene.h"
#include "Scene\ExploreScene\MapTool.h"
#include "Scene\MainScene\MainScene.h"

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
		TEXTURE->AddTexture(L"ui_panel2", L"Textures/UI_Panel_Blue.png");
		TEXTURE->AddTexture(L"ui_panel3", L"Textures/UI_Panel_Red.png");
		
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

		// main_UI
		TEXTURE->AddTexture(L"ui_bg", L"Textures/bg.jpg");
		//TEXTURE->AddTexture(L"ui_poketball_1", L"Textures/poketballs/pokeballs_01.png");
		//TEXTURE->AddTexture(L"ui_poketball_14", L"Textures/poketballs/pokeballs_14.png");
		TEXTURE->AddTexture(L"ui_start", L"Textures/start.png");
		TEXTURE->AddTexture(L"ui_tool", L"Textures/tool.png");
		TEXTURE->AddTexture(L"ui_exit", L"Textures/exit4.png");
		TEXTURE->AddTexture(L"ui_glasses", L"Textures/glasses2.png");

		TEXTURE->AddTexture(L"ui_fade", L"Textures/fade.png");

		// Pokemon
		// Pikachu
		TEXTURE->AddTexture(L"pikachu_idle",				L"Textures/pokemon/pikachu/idle_8x1.png");
		TEXTURE->AddTexture(L"pikachu_movement",			L"Textures/pokemon/pikachu/movement_24x1.png");
		TEXTURE->AddTexture(L"pikachu_hurt",				L"Textures/pokemon/pikachu/hurt_8x1.png");
		TEXTURE->AddTexture(L"pikachu_attack",				L"Textures/pokemon/pikachu/attack_8x1.png");
		TEXTURE->AddTexture(L"pikachu_special_attack",		L"Textures/pokemon/pikachu/special_attack_16x1.png");
		TEXTURE->AddTexture(L"pikachu_portrait",			L"Textures/pokemon/pikachu/portrait.png");

		// Charmander
		TEXTURE->AddTexture(L"charmander_idle",				L"Textures/pokemon/charmander/idle_8x1.png");
		TEXTURE->AddTexture(L"charmander_movement",			L"Textures/pokemon/charmander/movement_24x1.png");
		TEXTURE->AddTexture(L"charmander_hurt",				L"Textures/pokemon/charmander/hurt_8x1.png");
		TEXTURE->AddTexture(L"charmander_attack",			L"Textures/pokemon/charmander/attack_24x1.png");
		TEXTURE->AddTexture(L"charmander_special_attack",	L"Textures/pokemon/charmander/special_attack_8x1.png");
		TEXTURE->AddTexture(L"charmander_portrait",			L"Textures/pokemon/charmander/portrait.png");

		// Bulbasaur
		TEXTURE->AddTexture(L"bulbasaur_idle",				L"Textures/pokemon/bulbasaur/idle_8x1.png");
		TEXTURE->AddTexture(L"bulbasaur_movement",			L"Textures/pokemon/bulbasaur/movement_24x1.png");
		TEXTURE->AddTexture(L"bulbasaur_hurt",				L"Textures/pokemon/bulbasaur/hurt_8x1.png");
		TEXTURE->AddTexture(L"bulbasaur_attack",			L"Textures/pokemon/bulbasaur/attack_24x1.png");
		TEXTURE->AddTexture(L"bulbasaur_special_attack",	L"Textures/pokemon/bulbasaur/special_attack_16x1.png");
		TEXTURE->AddTexture(L"bulbasaur_portrait",			L"Textures/pokemon/bulbasaur/portrait.png");

		// Squirtle
		TEXTURE->AddTexture(L"squirtle_idle",				L"Textures/pokemon/squirtle/idle_8x1.png");
		TEXTURE->AddTexture(L"squirtle_movement",			L"Textures/pokemon/squirtle/movement_24x1.png");
		TEXTURE->AddTexture(L"squirtle_hurt",				L"Textures/pokemon/squirtle/hurt_8x1.png");
		TEXTURE->AddTexture(L"squirtle_attack",				L"Textures/pokemon/squirtle/attack_24x1.png");
		TEXTURE->AddTexture(L"squirtle_special_attack",		L"Textures/pokemon/squirtle/special_attack_24x1.png");
		TEXTURE->AddTexture(L"squirtle_portrait",			L"Textures/pokemon/squirtle/portrait.png");

		// Rattata
		TEXTURE->AddTexture(L"rattata_idle",				L"Textures/pokemon/rattata/idle_16x1.png");
		TEXTURE->AddTexture(L"rattata_movement",			L"Textures/pokemon/rattata/movement_24x1.png");
		TEXTURE->AddTexture(L"rattata_hurt",				L"Textures/pokemon/rattata/hurt_8x1.png");
		TEXTURE->AddTexture(L"rattata_attack",				L"Textures/pokemon/rattata/attack_32x1.png");
		TEXTURE->AddTexture(L"rattata_special_attack",		L"Textures/pokemon/rattata/special_attack_32x1.png");
		TEXTURE->AddTexture(L"rattata_portrait",			L"Textures/pokemon/rattata/portrait.png");

		// Raticate
		TEXTURE->AddTexture(L"raticate_idle",				L"Textures/pokemon/raticate/idle_24x1.png");
		TEXTURE->AddTexture(L"raticate_movement",			L"Textures/pokemon/raticate/movement_24x1.png");
		TEXTURE->AddTexture(L"raticate_hurt",				L"Textures/pokemon/raticate/hurt_8x1.png");
		TEXTURE->AddTexture(L"raticate_attack",				L"Textures/pokemon/raticate/attack_16x1.png");
		TEXTURE->AddTexture(L"raticate_special_attack",		L"Textures/pokemon/raticate/special_attack_24x1.png");
		TEXTURE->AddTexture(L"raticate_portrait",			L"Textures/pokemon/raticate/portrait.png");

		// Pidgey
		TEXTURE->AddTexture(L"pidgey_idle",					L"Textures/pokemon/pidgey/idle_16x1.png");
		TEXTURE->AddTexture(L"pidgey_movement",				L"Textures/pokemon/pidgey/movement_24x1.png");
		TEXTURE->AddTexture(L"pidgey_hurt",					L"Textures/pokemon/pidgey/hurt_8x1.png");
		TEXTURE->AddTexture(L"pidgey_attack",				L"Textures/pokemon/pidgey/attack_24x1.png");
		TEXTURE->AddTexture(L"pidgey_special_attack",		L"Textures/pokemon/pidgey/special_attack_16x1.png");
		TEXTURE->AddTexture(L"pidgey_portrait",				L"Textures/pokemon/pidgey/portrait.png");

		// Pidgeot
		TEXTURE->AddTexture(L"pidgeot_idle",				L"Textures/pokemon/pidgeot/idle_8x1.png");
		TEXTURE->AddTexture(L"pidgeot_movement",			L"Textures/pokemon/pidgeot/movement_24x1.png");
		TEXTURE->AddTexture(L"pidgeot_hurt",				L"Textures/pokemon/pidgeot/hurt_8x1.png");
		TEXTURE->AddTexture(L"pidgeot_attack",				L"Textures/pokemon/pidgeot/attack_24x1.png");
		TEXTURE->AddTexture(L"pidgeot_special_attack",		L"Textures/pokemon/pidgeot/special_attack_16x1.png");
		TEXTURE->AddTexture(L"pidgeot_portrait",			L"Textures/pokemon/pidgeot/portrait.png");

		// Pidgeotto
		TEXTURE->AddTexture(L"pidgeotto_idle",				L"Textures/pokemon/pidgeotto/idle_8x1.png");
		TEXTURE->AddTexture(L"pidgeotto_movement",			L"Textures/pokemon/pidgeotto/movement_24x1.png");
		TEXTURE->AddTexture(L"pidgeotto_hurt",				L"Textures/pokemon/pidgeotto/hurt_8x1.png");
		TEXTURE->AddTexture(L"pidgeotto_attack",			L"Textures/pokemon/pidgeotto/attack_24x1.png");
		TEXTURE->AddTexture(L"pidgeotto_special_attack",	L"Textures/pokemon/pidgeotto/special_attack_16x1.png");
		TEXTURE->AddTexture(L"pidgeotto_portrait",			L"Textures/pokemon/pidgeotto/portrait.png");

		// Caterpie
		TEXTURE->AddTexture(L"caterpie_idle",				L"Textures/pokemon/caterpie/idle_24x1.png");
		TEXTURE->AddTexture(L"caterpie_movement",			L"Textures/pokemon/caterpie/movement_24x1.png");
		TEXTURE->AddTexture(L"caterpie_hurt",				L"Textures/pokemon/caterpie/hurt_8x1.png");
		TEXTURE->AddTexture(L"caterpie_attack",				L"Textures/pokemon/caterpie/attack_32x1.png");
		TEXTURE->AddTexture(L"caterpie_special_attack",		L"Textures/pokemon/caterpie/special_attack_24x1.png");
		TEXTURE->AddTexture(L"caterpie_portrait",			L"Textures/pokemon/caterpie/portrait.png");

		// Metapod
		TEXTURE->AddTexture(L"metapod_idle",				L"Textures/pokemon/metapod/idle_24x1.png");
		TEXTURE->AddTexture(L"metapod_movement",			L"Textures/pokemon/metapod/movement_24x1.png");
		TEXTURE->AddTexture(L"metapod_hurt",				L"Textures/pokemon/metapod/hurt_8x1.png");
		TEXTURE->AddTexture(L"metapod_attack",				L"Textures/pokemon/metapod/attack_32x1.png");
		TEXTURE->AddTexture(L"metapod_special_attack",		L"Textures/pokemon/metapod/special_attack_8x1.png");
		TEXTURE->AddTexture(L"metapod_portrait",			L"Textures/pokemon/metapod/portrait.png");

		// Butterfree
		TEXTURE->AddTexture(L"butterfree_idle",				L"Textures/pokemon/butterfree/idle_24x1.png");
		TEXTURE->AddTexture(L"butterfree_movement",			L"Textures/pokemon/butterfree/movement_24x1.png");
		TEXTURE->AddTexture(L"butterfree_hurt",				L"Textures/pokemon/butterfree/hurt_8x1.png");
		TEXTURE->AddTexture(L"butterfree_attack",			L"Textures/pokemon/butterfree/attack_16x1.png");
		TEXTURE->AddTexture(L"butterfree_special_attack",	L"Textures/pokemon/butterfree/special_attack_16x1.png");
		TEXTURE->AddTexture(L"butterfree_portrait",			L"Textures/pokemon/butterfree/portrait.png");

		// Weedle
		TEXTURE->AddTexture(L"weedle_idle",					L"Textures/pokemon/weedle/idle_16x1.png");
		TEXTURE->AddTexture(L"weedle_movement",				L"Textures/pokemon/weedle/movement_24x1.png");
		TEXTURE->AddTexture(L"weedle_hurt",					L"Textures/pokemon/weedle/hurt_8x1.png");
		TEXTURE->AddTexture(L"weedle_attack",				L"Textures/pokemon/weedle/attack_16x1.png");
		TEXTURE->AddTexture(L"weedle_special_attack",		L"Textures/pokemon/weedle/special_attack_16x1.png");
		TEXTURE->AddTexture(L"weedle_portrait",				L"Textures/pokemon/weedle/portrait.png");

		// KaKuna
		TEXTURE->AddTexture(L"kakuna_idle",					L"Textures/pokemon/kakuna/idle_8x1.png");
		TEXTURE->AddTexture(L"kakuna_movement",				L"Textures/pokemon/kakuna/movement_16x1.png");
		TEXTURE->AddTexture(L"kakuna_hurt",					L"Textures/pokemon/kakuna/hurt_8x1.png");
		TEXTURE->AddTexture(L"kakuna_attack",				L"Textures/pokemon/kakuna/attack_48x1.png");
		TEXTURE->AddTexture(L"kakuna_special_attack",		L"Textures/pokemon/kakuna/special_attack_8x1.png");
		TEXTURE->AddTexture(L"kakuna_portrait",				L"Textures/pokemon/kakuna/portrait.png");

		// Beedril
		TEXTURE->AddTexture(L"beedril_idle",				L"Textures/pokemon/beedril/idle_24x1.png");
		TEXTURE->AddTexture(L"beedril_movement",			L"Textures/pokemon/beedril/movement_24x1.png");
		TEXTURE->AddTexture(L"beedril_hurt",				L"Textures/pokemon/beedril/hurt_8x1.png");
		TEXTURE->AddTexture(L"beedril_attack",				L"Textures/pokemon/beedril/attack_88x1.png");
		TEXTURE->AddTexture(L"beedril_special_attack",		L"Textures/pokemon/beedril/special_attack_8x1.png");
		TEXTURE->AddTexture(L"beedril_portrait",			L"Textures/pokemon/beedril/portrait.png");
	}

	// SPRITE Setting
	{
		D3DXCreateSprite(D2D::GetDevice(), &sprite);
	}

	// JSON Setting
	{
		// GameManager의 pokemonInfo Json에서 읽어오기
		GAME->ReadPokemonInfo();

		//root = new Json::Value();
		//readJson = new Json::Value();
		//float time = FRAME->GetElapsedTime();
		//Json::SetValue(*root, "test", time);
		//
		//Json::WriteJsonData(L"Test.Json", root);
		//Json::ReadJsonData(L"Test.Json", readJson);
		//
		//float temp;
		//Json::GetValue(*readJson, "test", temp);
	}

	// SCENE Setting
	{
		TestScene * test = new TestScene;
		SCENE->AddScene("Test", test);
		MainScene * main = new MainScene;
		SCENE->AddScene("Main", main);
		ExploreScene * explore = new ExploreScene;
		SCENE->AddScene("Explore", explore);
		MapTool * mapTool = new MapTool;
		SCENE->AddScene("MapTool", mapTool);
	}
	
	// 기타 Setting
	{
		//SOUND->Play("Test");
		//SCENE->ChangeScene("Test");
		SCENE->ChangeScene("Main");
		//SCENE->ChangeScene("Explore");
		//SCENE->ChangeScene("MapTool");

		isDebug = false;
	}

	// fade in/out
	{
		mainCamera = new Camera;
		mainCamera->UpdateCamToDevice();

		fade = new Rect;
		fade->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
		fade->SetCamera(mainCamera);
		fade->GetTransform()->SetScale(
			Vector2(10.3f, 7.7f));
		fade->SetTexture(TEXTURE->GetTexture(L"ui_fade"));
		fade->SetMixedColor(true);

		fadeSwitch = 0;
		fadeOut = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		fadeIn =  D3DXCOLOR(1.0f,1.0f,1.0f,0.0f);
	}

}

Program::~Program()
{
	TEXTURE->Release();
	SCENE->Release();
	GAME->Release();

	SAFE_DELETE(readJson);
	SAFE_DELETE(root);

	SAFE_DELETE(particle);

	SAFE_RELEASE(sprite);

	SAFE_RELEASE(fade);
	SAFE_DELETE(fade);

	SAFE_DELETE(mainCamera);
}

void Program::Update()
{
	SCENE->Update();

	D3DXCOLOR curColor;
	float deltaTime = FRAME->GetElapsedTime();

	switch (fadeSwitch)
	{
	// 정지 상태
	case 0:
		break;
	// fade in
	case 1:
		curColor = fade->GetColor();
		if (fadeIn.a != curColor.a) {
			D3DXCOLOR color;
			D3DXColorLerp(&color, &curColor, &fadeIn,
				deltaTime);
			fade->ChangeColor(color);
		}
		else
			fadeSwitch = 0;
		break;
	// fade out
	case 2:
		curColor = fade->GetColor();
		if (fadeOut.a != curColor.a) {
			D3DXCOLOR color;
			D3DXColorLerp(&color, &curColor, &fadeOut,
				deltaTime);
			fade->ChangeColor(color);
		}
		else
			fadeSwitch = 0;
		break;
	}

	if (INPUT->GetKeyDown(VK_F11))
		isDebug = !isDebug;

	if (INPUT->GetKeyDown('O')) {
		//fade->ChangeColor(
		//	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
		//fade->ChangeColor(0x88ffffff);
		fadeSwitch = 1;
	}
	if (INPUT->GetKeyDown('P')) {
		fadeSwitch = 2;
	}
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

	fade->Render();

	if (isDebug) {

	}
}