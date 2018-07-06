#include "stdafx.h" 
#include "ExploreScene.h"

#include "GameObject\TileMap.h"
#include "./Common/Camera.h"

#include "GameObject\Rect.h"
#include "GameObject\Pokemon.h"
#include "AStar.h"

#include "Utilities\ProgressBar.h"

ExploreScene::ExploreScene()
{
}

ExploreScene::~ExploreScene()
{
}

void ExploreScene::Init()
{
	mainCamera = new Camera;

	this->jsonTileMap = new Json::Value();
	jsonStageInfo = new Json::Value();

	TileInit();

	StageInit();

	PokemonInit();

	DebugInit();

	//circle = new Rect;
	//circle->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	//circle->SetCamera(mainCamera);
	//circle->SetTexture(TEXTURE->GetTexture(L"ui_circle"));
	//circle->GetTransform()->SetScale(Vector2(0.5f, 0.3f));

	tempTransform = new Transform;
	cameraFollow = false;
	isChange = false;

	isDebug = false;
}

void ExploreScene::Release()
{
	SAFE_RELEASE(tile);
	SAFE_DELETE(tile);

	SAFE_DELETE(mainCamera);
	SAFE_DELETE(tempTransform);

	SAFE_RELEASE(font);

	SAFE_DELETE(jsonTileMap);
	SAFE_DELETE(jsonStageInfo);

	if (pokemon != NULL) {
		for (int i = 0; i < PLAYERCOUNT; i++) {
			SAFE_RELEASE(pokemon[i]);
			SAFE_DELETE(pokemon[i]);
		}
		delete[] pokemon;
	}

	if (enemy != NULL) {
		for (int i = 0; i < MAX_ENEMYCOUNT; i++) {
			SAFE_RELEASE(enemy[i]);
			SAFE_DELETE(enemy[i]);
		}
		delete[] enemy;
	}

	//SAFE_RELEASE(pokemon);
	//SAFE_DELETE(pokemon);
	//SAFE_RELEASE(pokemon2);
	//SAFE_DELETE(pokemon2);
	//SAFE_RELEASE(enemy);
	//SAFE_DELETE(enemy);
	//SAFE_RELEASE(enemy2);
	//SAFE_DELETE(enemy2);

	//SAFE_RELEASE(circle);
	//SAFE_DELETE(circle);
}

void ExploreScene::Update()
{
	curTeam = stageInfo[curStage].curTeam;
	teamCount = stageInfo[curStage].teams[curTeam].count;

	if (cameraFollow) {
		//mainCamera->SetWorldPosition(
		//	pokemon->GetTransform()->GetWorldPosition());

		mainCamera->Interpolate(mainCamera, pokemon[0]->GetTransform(),
			FRAME->GetElapsedTime() * 1.0f);

		if (!isChange) {
			isChange = true;

			UpdateCameraChange(
				Vector2(2.7f, 2.7f), Vector2(0.6f, 0.6f));
		}
	}
	else {
		//mainCamera->SetWorldPosition(Vector2(0, 0));

		mainCamera->Interpolate(mainCamera, tempTransform,
			FRAME->GetElapsedTime() * 1.0f);

		if (!isChange) {
			isChange = true;

			UpdateCameraChange(
				Vector2(0.9f, 0.9f), Vector2(0.2f, 0.2f));
		}
	}

	mainCamera->UpdateCamToDevice();
	//mainCamera->DefaultControl2();
	//tile->Update();

	//circle->Update();
	//POKEMON_DIRECTION dir = pokemon->GetPokemonInfo().dir;
	//// left
	//if (dir % 2 == 0)
	//	circle->GetTransform()->SetWorldPosition(
	//		pokemon->GetTransform()->GetWorldPosition()
	//		+ Vector2(0, 25));
	//else
	//	circle->GetTransform()->SetWorldPosition(
	//		pokemon->GetTransform()->GetWorldPosition()
	//		+ Vector2(10, 25));

	
	//pokemon->Update();
	//pokemon2->Update();
	//enemy->Update();
	//enemy2->Update();

	for (int i = 0; i < PLAYERCOUNT; i++) {
		pokemon[i]->Update();
	}

	for (int i = curTeam * teamCount; 
		i < (curTeam + 1) * teamCount; i++) {
		enemy[i]->Update();
	}

	FindPokemon();

	if (INPUT->GetKeyDown('O')) {
		cameraFollow = !cameraFollow;
		isChange = false;
	}

	if (INPUT->GetKeyDown(VK_F11))
		isDebug = !isDebug;
}

void ExploreScene::Render()
{
	tile->Render();
	
	//circle->Render();
	
	//pokemon->Render();
	//pokemon2->Render();
	//enemy->Render();
	//enemy2->Render();

	for (int i = 0; i < PLAYERCOUNT; i++) {
		pokemon[i]->Render();
	}

	for (int i = curTeam * teamCount;
		i < (curTeam + 1) * teamCount; i++) {
		enemy[i]->Render();
	}

	if (isDebug) {
		DebugRender();

		for (int i = 0; i < PLAYERCOUNT; i++) {
			pokemon[i]->DrawAttackRange();
			pokemon[i]->GetAStar()->DrawPath();
		}

		for (int i = curTeam * teamCount;
			i < (curTeam + 1) * teamCount; i++) {
			enemy[i]->DrawAttackRange();
			enemy[i]->GetAStar()->DrawPath();
		}
	}
}

void ExploreScene::TileInit()
{
	LPDIRECT3DTEXTURE9 tileTex = TEXTURE->GetTexture(L"tile_forest");

	tile = new TileMap;
	tile->Init(L"./Shader/ColorTexture.fx",
		Vector2(TILE_INITFRAME_X, TILE_INITFRAME_Y),
		TILE_ROW, TILE_COL, TILE_SIZE,
		TILE_VERTEX_SIZE, TILE_INDEX_SIZE,
		Vector2(TILE_WIDTH, TILE_HEIGHT),
		Vector2(
			-TILE_ROW * TILE_WIDTH / 2,
			-TILE_COL * TILE_HEIGHT / 2));
	tile->SetTexture(tileTex);
	tile->SetCamera(mainCamera);

	this->MapLoad();

	// 타일 위치 및 크기 설정
	tile->GetTransform()->SetScale(Vector2(0.9f, 0.9f));
	tile->GetTransform()->SetWorldPosition(Vector2(-130.0f, -1.0f));

	Vector2 center = tile->GetTileCenterPos(20, 12);

	tile->UpdateTileCenterPos();

	center = tile->GetTileCenterPos(20, 12);
}

void ExploreScene::StageInit()
{
	StageLoad();

	curStage = 0;

	curTeam = stageInfo[curStage].curTeam;
	teamCount = stageInfo[curStage].teams[curTeam].count;
}

void ExploreScene::PokemonInit()
{
	/*
	// player
	pokemon = new Pokemon;
	int frameCnt[5];

	// pikachu
	//frameCnt[STATE_IDLE] = 8;
	//frameCnt[STATE_ATTACK] = 8;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 16;
	//pokemon->SetTileMap(tile);
	//pokemon->SetCamera(mainCamera);
	//pokemon->Init(L"pikachu", frameCnt, L"player",
	//	Vector2(13.0f, 0));

	// bulbasaur
	//frameCnt[STATE_IDLE] = 8;
	//frameCnt[STATE_ATTACK] = 24;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 16;
	//pokemon->SetTileMap(tile);
	//pokemon->SetCamera(mainCamera);
	//pokemon->Init(L"bulbasaur", frameCnt, L"player", 
	//	Vector2(13.0f, 0));

	// charmander
	//frameCnt[STATE_IDLE] = 8;
	//frameCnt[STATE_ATTACK] = 24;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 8;
	//pokemon->SetTileMap(tile);
	//pokemon->SetCamera(mainCamera);
	//pokemon->Init(L"charmander", frameCnt, L"player",
	//	Vector2(13.0f, 0));

	// squirtle
	//frameCnt[STATE_IDLE] = 8;
	//frameCnt[STATE_ATTACK] = 24;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 24;
	//pokemon->SetTileMap(tile);
	//pokemon->SetCamera(mainCamera);
	//pokemon->Init(L"squirtle", frameCnt, L"player",
	//	Vector2(13.0f, 0));

	// rattata
	frameCnt[STATE_IDLE] = 16;
	frameCnt[STATE_ATTACK] = 32;
	frameCnt[STATE_HURT] = 8;
	frameCnt[STATE_MOVE] = 24;
	frameCnt[STATE_SPECIAL_ATTACK] = 32;
	pokemon->SetTileMap(tile);
	pokemon->SetCamera(mainCamera);
	pokemon->Init(L"rattata", frameCnt, L"player",
		Vector2(13.0f, 0));

	// pidgey
	//frameCnt[STATE_IDLE] = 16;
	//frameCnt[STATE_ATTACK] = 24;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 16;
	//pokemon->SetTileMap(tile);
	//pokemon->SetCamera(mainCamera);
	//pokemon->Init(L"pidgey", frameCnt, L"player",
	//	Vector2(13.0f, 0));

	pokemon->GetTransform()->SetScale(Vector2(0.2f, 0.2f));

	//pokemon->GetHpBar()->GetFront()->
	//	SetTexture(TEXTURE->GetTexture(L"hp_player"));

	// ally charmander
	pokemon2 = new Pokemon;
	frameCnt[STATE_IDLE] = 8;
	frameCnt[STATE_ATTACK] = 24;
	frameCnt[STATE_HURT] = 8;
	frameCnt[STATE_MOVE] = 24;
	frameCnt[STATE_SPECIAL_ATTACK] = 8;
	pokemon2->SetTileMap(tile);
	pokemon2->SetCamera(mainCamera);
	pokemon2->Init(L"charmander", frameCnt, L"ally",
		Vector2(13.0f, 0), { 2, 19 });

	pokemon2->GetTransform()->SetScale(Vector2(0.2f, 0.2f));

	/// enemy
	enemy = new Pokemon;

	// pikachu
	//frameCnt[STATE_IDLE] = 8;
	//frameCnt[STATE_ATTACK] = 8;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 16;
	//enemy->SetTileMap(tile);
	//enemy->SetCamera(mainCamera);
	//enemy->Init(L"pikachu", frameCnt, L"enemy", 
	//	Vector2(13.0f, 0), { 20,15 });

	// rattata
	frameCnt[STATE_IDLE] = 16;
	frameCnt[STATE_ATTACK] = 32;
	frameCnt[STATE_HURT] = 8;
	frameCnt[STATE_MOVE] = 24;
	frameCnt[STATE_SPECIAL_ATTACK] = 32;
	enemy->SetTileMap(tile);
	enemy->SetCamera(mainCamera);
	enemy->Init(L"rattata", frameCnt, L"enemy",
		Vector2(13.0f, 0), { 20,15 });

	enemy->GetTransform()->SetScale(Vector2(0.2f, 0.2f));

	enemy2 = new Pokemon;

	// charmander
	//frameCnt[STATE_IDLE] = 8;
	//frameCnt[STATE_ATTACK] = 24;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 8;
	//enemy2->SetTileMap(tile);
	//enemy2->SetCamera(mainCamera);
	//enemy2->Init(L"charmander", frameCnt, L"enemy",
	//	Vector2(13.0f, 0), { 15,10 });

	// rattata
	frameCnt[STATE_IDLE] = 16;
	frameCnt[STATE_ATTACK] = 32;
	frameCnt[STATE_HURT] = 8;
	frameCnt[STATE_MOVE] = 24;
	frameCnt[STATE_SPECIAL_ATTACK] = 32;
	enemy2->SetTileMap(tile);
	enemy2->SetCamera(mainCamera);
	enemy2->Init(L"rattata", frameCnt, L"enemy",
		Vector2(13.0f, 0), { 15,10 });

	enemy2->GetTransform()->SetScale(Vector2(0.2f, 0.2f));
	*/

	// player
	//pokemon = new Pokemon;
	//pokemon2 = new Pokemon;
	//enemy = new Pokemon;
	//enemy2 = new Pokemon;

	//pokemon->SetTileMap(tile);
	//pokemon2->SetTileMap(tile);
	//enemy->SetTileMap(tile);
	//enemy2->SetTileMap(tile);
	//
	//pokemon->SetCamera(mainCamera);
	//pokemon2->SetCamera(mainCamera);
	//enemy->SetCamera(mainCamera);
	//enemy2->SetCamera(mainCamera);

	pokemon = new Pokemon*[PLAYERCOUNT];

	for (int i = 0; i < PLAYERCOUNT; i++) {
		pokemon[i] = new Pokemon;
		pokemon[i]->SetTileMap(tile);
		pokemon[i]->SetCamera(mainCamera);
	}

	enemy = new Pokemon*[MAX_ENEMYCOUNT];
	for (int i = 0; i < MAX_ENEMYCOUNT; i++) {
		enemy[i] = new Pokemon;
		enemy[i]->SetTileMap(tile);
		enemy[i]->SetCamera(mainCamera);
	}

	// pikachu
	// charmander
	// bulbasaur
	// squirtle
	// rattata
	// pidgey

	//pokemon->Init(L"charmander", L"player");
	//pokemon2->Init(L"pikachu", L"ally", { 2, 19 });
	//enemy->Init(GAME->GetPokemonName(3), L"enemy", { 20,15 });
	//enemy2->Init(GAME->GetPokemonName(4), L"enemy", { 15,10 });
	//
	//pokemon->GetTransform()->SetScale(Vector2(0.2f, 0.2f));
	//pokemon2->GetTransform()->SetScale(Vector2(0.2f, 0.2f));
	//enemy->GetTransform()->SetScale(Vector2(0.2f, 0.2f));
	//enemy2->GetTransform()->SetScale(Vector2(0.2f, 0.2f));

	PokemonSetting();

	FindPokemon();
}

void ExploreScene::PokemonSetting()
{
	wstring pokemonName[] = {
		L"pikachu", L"charmander", L"squirtle", L"bulbasaur" };

	for (int i = 0; i < PLAYERCOUNT; i++) {
		pokemon[i]->Init(pokemonName[i],
			i == 0 ? L"player" : L"ally",
			stageInfo[curStage].startPos[i]);
		pokemon[i]->GetTransform()->SetScale(Vector2(0.2f, 0.2f));
	}

	tagStageInfo stage = stageInfo[curStage];
	for (int i = 0; i < stage.enemyCount; i++) {
		int teamCount = stage.teamCount;
		int memberCount = stage.teams[
			i / (stage.enemyCount / teamCount)].count;

		enemy[i]->Init(
			stage.teams[i / memberCount]
			.name[i % memberCount], L"enemy",
			stage.teams[i / memberCount].pos[i % memberCount]);
		enemy[i]->GetTransform()->SetScale(Vector2(0.2f, 0.2f));
	}
}

void ExploreScene::DebugInit()
{
	HRESULT hr = D3DXCreateFont(
		D2D::GetDevice(),
		15,					// 너비
		0,					// 높이
		FW_BOLD,			// 두께, 두껍게 쓰겠다는거
		1,					// 이 둘은 크게 바꿀 꺼 없음
		FALSE,				// 이탤릭체 사용 여부
		DEFAULT_CHARSET,	// 이 둘은 크게 바꿀 꺼 없음
		OUT_DEFAULT_PRECIS,	// 외곽선 처리
		DEFAULT_QUALITY,	// 퀄리티
							// FF_DONTCARE 글자 크기 상관없이 큰 녀석 기준으로 잡아주는거 너비랑 높이 중에
		DEFAULT_PITCH || FF_DONTCARE,		// 옵션, 
		L"굴림",
		&font
	);
	assert(SUCCEEDED(hr));
}

void ExploreScene::DebugRender()
{
	RECT rc = { 10, 70, 0,0 };
	wstring str;

	Vector2 mousePos;
	
	// mousePos
	{
		Util::GetMousePosWithScreen(&mousePos);

		str = L"Mouse Pos : ";
		str += to_wstring(mousePos.x);
		str += L", ";
		str += to_wstring(mousePos.y);

		// 멀티바이트면 DrawTextA
		font->DrawTextW(
			// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
			NULL,
			str.c_str(),
			-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
			&rc,
			// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
			// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
			DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
								 // 0x~~ 이거 귀찮으면 함수도 있음
								 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}

	// tile
	rc.top += 20;
	Vector2 tilePos = tile->GetTransform()->GetWorldPosition();
	
	tilePos.x -= this->mainCamera->GetWorldPosition().x;
	tilePos.y -= this->mainCamera->GetWorldPosition().y;

	// tilePos
	{
		str = L"TilePos : ";
		str += to_wstring(tilePos.x);
		str += L" , ";
		str += to_wstring(tilePos.y);

		// 멀티바이트면 DrawTextA
		font->DrawTextW(
			// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
			NULL,
			str.c_str(),
			-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
			&rc,
			// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
			// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
			DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
			// 0x~~ 이거 귀찮으면 함수도 있음
			//D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}

	Vector2 tileScale = tile->GetTransform()->GetScale();
	rc.top += 20;

	// tileScale
	{
		str = L"TileScale : ";
		str += to_wstring(tileScale.x);
		str += L" , ";
		str += to_wstring(tileScale.y);

		// 멀티바이트면 DrawTextA
		font->DrawTextW(
			// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
			NULL,
			str.c_str(),
			-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
			&rc,
			// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
			// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
			DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
			// 0x~~ 이거 귀찮으면 함수도 있음
			//D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}

	// radian
	//rc.top += 20;
	//str = L"Current Radian : ";
	//str += to_wstring(tile->GetTransform()->GetZRadian());
	//
	//// 멀티바이트면 DrawTextA
	//font->DrawTextW(
	//	// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
	//	NULL,
	//	str.c_str(),
	//	-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
	//	&rc,
	//	// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
	//	// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
	//	DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
	//						 // 0x~~ 이거 귀찮으면 함수도 있음
	//						 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	//	0xFFFF0000
	//);

	// 마우스가 타일 위에 있을 때
	if (mousePos.x >=
		tilePos.x - (TILE_COL * TILE_HEIGHT / 2) * tileScale.x &&
		mousePos.x <=
		tilePos.x + (TILE_COL * TILE_HEIGHT / 2) * tileScale.x &&
		mousePos.y >=
		tilePos.y - (TILE_ROW * TILE_WIDTH / 2) * tileScale.y &&
		mousePos.y <=
		tilePos.y + (TILE_ROW * TILE_WIDTH / 2) * tileScale.y) {

		rc.top += 20;

		POINT currentTile;
		currentTile.x = (mousePos.y - tilePos.y +
			(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
			(TILE_WIDTH * tileScale.y);
		currentTile.y = (mousePos.x - tilePos.x +
			(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
			(TILE_HEIGHT * tileScale.x);

		// currentTile
		{
			str = L"Current Tile : (";
			str += to_wstring(currentTile.x);
			str += L" , ";
			str += to_wstring(currentTile.y);
			str += L")";

			// 멀티바이트면 DrawTextA
			font->DrawTextW(
				// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
				NULL,
				str.c_str(),
				-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
				&rc,
				// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
				// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
				DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
				// 0x~~ 이거 귀찮으면 함수도 있음
				//D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
				0xFFFF0000
			);
		}

		rc.top += 20;
		
		// tile block 여부
		{
			bool isBlock = tile->GetTileInfo(
				currentTile.x, currentTile.y).block;

			str = L"Tile Block: ";
			str += isBlock ? L"True" : L"False";

			// 멀티바이트면 DrawTextA
			font->DrawTextW(
				// 이미지 2D 좌표에서 띄우는걸 sprite라고 함
				NULL,
				str.c_str(),
				-1,	// 전체 띄우려면 -1, 아니면 문자열 길이만큼 하면됨
				&rc,
				// DT_NOCLIP이 rc에 상관없이 출력하겠다는거
				// 이거쓰면 rc의 10,10이 좌표 정도만 되는거
				DT_LEFT | DT_NOCLIP, // 옵션, 왼쪽 정렬로 하겠다는거
									 // 0x~~ 이거 귀찮으면 함수도 있음
									 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
				0xFFFF0000
			);
		}
	}
}

void ExploreScene::MapLoad()
{
	Json::ReadJsonData(L"Save/Stage/TileMap.Json", jsonTileMap);

	for (int i = 0; i < TILE_ROW; i++) {
		for (int j = 0; j < TILE_COL; j++) {
			string str = "TileInfo_";
			str += to_string(i) + "_" + to_string(j) + "_";

			tagTile tileInfo;

			float block;

			Json::GetValue(*jsonTileMap, str + "CenterX", tileInfo.center.x);
			Json::GetValue(*jsonTileMap, str + "CenterY", tileInfo.center.y);
			Json::GetValue(*jsonTileMap, str + "Block", block);
			if (FLOATZERO(block)) tileInfo.block = false;
			else tileInfo.block = true;
			Json::GetValue(*jsonTileMap, str + "UVX", tileInfo.uv.x);
			Json::GetValue(*jsonTileMap, str + "UVY", tileInfo.uv.y);

			tile->SetTileInfo(i, j, tileInfo);
		}
	}

	tile->UpdateTileInfo();
}

void ExploreScene::StageLoad()
{
	Json::ReadJsonData(L"Save/Stage/StageInfo.Json", jsonStageInfo);

	string str = "StageInfo";

	int stageCount;
	Json::GetValue(*jsonStageInfo, str + "_Count", stageCount);

	tagStageInfo stage;

	for (int i = 0; i < stageCount; i++) {
		string name;
		Json::GetString(*jsonStageInfo,
			str + "_Name_" + to_string(i), name);

		stage.name = String::StringToWString(name);


		string stageName = str + "_" + name;

		// 내 포켓몬 스타트 위치
		stage.pokemonCount = PLAYERCOUNT;
		for (int j = 0; j < 4; j++) {
			string temp = stageName+ "_pokemon_" +
				to_string(j);
			int x, y;

			Json::GetValue(*jsonStageInfo, temp + "_posX", x);
			Json::GetValue(*jsonStageInfo, temp + "_posY", y);

			stage.startPos[j] = { x,y };
		}

		Json::GetValue(*jsonStageInfo, 
			stageName + "_enemyCount", stage.enemyCount);
		Json::GetValue(*jsonStageInfo, 
			stageName + "_enemyKind", stage.enemyKind);

		for (int j = 0; j < stage.enemyKind; j++) {
			string temp = stageName + "_enemyName_";
			string enemyName;
			Json::GetString(*jsonStageInfo,
				temp + to_string(j), enemyName);

			stage.enemyName.push_back(
				String::StringToWString(enemyName));
		}
		
		Json::GetValue(*jsonStageInfo, stageName + "_teamCount",
			stage.teamCount);
		
		for (int j = 0; j < stage.teamCount; j++) {
			int memberCount;
			string team = stageName + "_team_" + to_string(j);

			Json::GetValue(*jsonStageInfo,
				team + "_Count", memberCount);

			tagTeamInfo teamInfo;
			for (int k = 0; k < memberCount; k++) {
				string member = team + "_" + to_string(k);
				string memberName;

				Json::GetString(*jsonStageInfo,
					member + "_name", memberName);
				teamInfo.name[k] = String::StringToWString(memberName);

				int x, y;
				Json::GetValue(*jsonStageInfo,
					member + "_posX", x);
				Json::GetValue(*jsonStageInfo,
					member + "_posY", y);
				teamInfo.pos[k] = { x,y };
			}
			stage.teams.push_back(teamInfo);
		}

		stageInfo.push_back(stage);
	}
}

void ExploreScene::FindPokemon()
{
	//Pokemon* targetEnemy = FindNearPokemon();
	//
	//// enemy 타겟이 설정되지 않았거나 현재 설정된 enemy가 죽엇거나
	//// 현재 계산된 가장 가까운 enemy가 다르면 새로 설정
	//if (pokemon->GetEnemy() == NULL 
	//	|| pokemon->GetEnemy()->GetIsDied()
	//	|| pokemon->GetEnemy() != targetEnemy) {
	//	pokemon->SetEnemy(targetEnemy);
	//}
	//
	//pokemon2->SetEnemy(enemy);
	//
	//enemy->SetEnemy(pokemon);
	//enemy2->SetEnemy(pokemon);

	for (int i = 0; i < PLAYERCOUNT; i++) {
		Pokemon* targetPokemon = FindNearPokemon(pokemon[i], false);

		if (targetPokemon == NULL) {
			if (curTeam < stageInfo[curStage].teamCount) {
				curTeam++;
			}
		}

		pokemon[i]->SetEnemy(targetPokemon);
	}

	for (int i = curTeam * teamCount;
		i < (curTeam + 1) * teamCount; i++) {
		Pokemon* targetPokemon = FindNearPokemon(enemy[i], true);

		enemy[i]->SetEnemy(targetPokemon);
	}
}

Pokemon * ExploreScene::FindNearPokemon(Pokemon* curPokemon, bool isEnemy)
{
	//if(isEnemy) {
	//	float dist;
	//	if (enemy->GetIsDied()) dist = BIGNUM;
	//	else
	//		dist = (pokemon->GetTransform()->GetWorldPosition() -
	//				enemy->GetTransform()->GetWorldPosition()).Length();
	//	float dist2;
	//	if (enemy2->GetIsDied()) dist2 = BIGNUM;
	//	else
	//		dist2 = (pokemon->GetTransform()->GetWorldPosition() -
	//				enemy2->GetTransform()->GetWorldPosition()).Length();
	//
	//	if (dist <= dist2)
	//		return enemy;
	//	else
	//		return enemy2;
	//}
	//else {
	//	return pokemon;
	//}

	float minDist = BIGNUM;
	int minIndex = 0;

	Vector2 pos = curPokemon->GetTransform()->GetWorldPosition();

	if (isEnemy) {
		for (int i = 0; i < PLAYERCOUNT; i++) {
			if (pokemon[i]->GetIsDied()) continue;

			Vector2 pos2 = pokemon[i]->
				GetTransform()->GetWorldPosition();
			float dist = (pos - pos2).Length();

			if (minDist > dist) {
				minDist = dist;
				minIndex = i;
			}
		}
	}
	else {
		for (int i = curTeam * teamCount;
			i < (curTeam + 1) * teamCount; i++) {
			if (enemy[i]->GetIsDied()) continue;

			Vector2 pos2 = enemy[i]->
				GetTransform()->GetWorldPosition();
			float dist = (pos - pos2).Length();

			if (minDist > dist) {
				minDist = dist;
				minIndex = i;
			}
		}
	}

	if (minDist == BIGNUM)
		return NULL;
	else
		return isEnemy ? pokemon[minIndex] : enemy[minIndex];
}

void ExploreScene::UpdateCameraChange(Vector2 tileScale, Vector2 pokemonScale)
{
	tile->GetTransform()->SetScale(tileScale);
	
	// 스케일 보간 -> 이상함
	//Transform temp;
	//temp.SetScale(tileScale);
	//
	//tile->GetTransform()->ScaleLerp(
	//	tile->GetTransform(), &temp, 
	//	FRAME->GetElapsedTime());

	tile->UpdateTileCenterPos();


	//UpdatePokemonChange(pokemon, pokemonScale);
	//UpdatePokemonChange(pokemon2, pokemonScale);
	//
	//UpdatePokemonChange(enemy, pokemonScale);
	//UpdatePokemonChange(enemy2, pokemonScale);

	for (int i = 0; i < PLAYERCOUNT; i++) {
		UpdatePokemonChange(pokemon[i], pokemonScale);
	}

	for (int i = curTeam * teamCount;
		i < (curTeam + 1) * teamCount; i++) {
		UpdatePokemonChange(enemy[i], pokemonScale);
	}
}

void ExploreScene::UpdatePokemonChange(Pokemon * pokemon, Vector2 scale)
{
	tagPokemonStatus pokemonInfo = pokemon->GetPokemonStatus();
	Transform* transform = pokemon->GetTransform();

	transform->SetScale(scale);

	pokemon->GetHpBar()->SetScale(transform->GetScale());

	// 스케일 보간 -> 이상함
	//Transform temp;
	//temp.SetScale(scale);
	//
	//transform->ScaleLerp(transform, &temp, 
	//	FRAME->GetElapsedTime());

	pokemon->CalculateAttackRange();

	Vector2 tileCenter = tile->GetTileCenterPos(
		pokemonInfo.curTile.x,
		pokemonInfo.curTile.y);
	Vector2 targetPos = tileCenter + Vector2(
		0 * transform->GetScale().x,
		-10.0f * transform->GetScale().y);

	transform->SetWorldPosition(targetPos);

	pokemon->GetHpBar()->SetPosition(transform->GetWorldPosition()
		+ Vector2(0, 65 * transform->GetScale().y));
	pokemon->ChangeHpBar();
}
