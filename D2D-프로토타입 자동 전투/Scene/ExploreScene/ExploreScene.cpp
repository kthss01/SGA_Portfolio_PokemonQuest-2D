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

	TileInit();

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

	SAFE_RELEASE(pokemon);
	SAFE_DELETE(pokemon);

	SAFE_RELEASE(pokemon2);
	SAFE_DELETE(pokemon2);

	SAFE_RELEASE(enemy);
	SAFE_DELETE(enemy);

	SAFE_RELEASE(enemy2);
	SAFE_DELETE(enemy2);

	//SAFE_RELEASE(circle);
	//SAFE_DELETE(circle);
}

void ExploreScene::Update()
{
	if (cameraFollow) {
		//mainCamera->SetWorldPosition(
		//	pokemon->GetTransform()->GetWorldPosition());

		mainCamera->Interpolate(mainCamera, pokemon->GetTransform(),
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

	
	pokemon->Update();
	//pokemon2->Update();
	//enemy->Update();
	enemy2->Update();

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
	
	pokemon->Render();
	pokemon2->Render();

	enemy->Render();
	enemy2->Render();

	if (isDebug) {
		DebugRender();

		pokemon->DrawAttackRange();
		pokemon->GetAStar()->DrawPath();

		enemy->DrawAttackRange();
		enemy->GetAStar()->DrawPath();
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

	// Ÿ�� ��ġ �� ũ�� ����
	tile->GetTransform()->SetScale(Vector2(0.9f, 0.9f));
	tile->GetTransform()->SetWorldPosition(Vector2(-130.0f, -1.0f));

	Vector2 center = tile->GetTileCenterPos(20, 12);

	tile->UpdateTileCenterPos();

	center = tile->GetTileCenterPos(20, 12);
}

void ExploreScene::PokemonInit()
{
	// player
	pokemon = new Pokemon;
	int frameCnt[5];

	//// pikachu
	//frameCnt[STATE_IDLE] = 8;
	//frameCnt[STATE_ATTACK] = 8;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 16;
	//pokemon->SetTileMap(tile);
	//pokemon->SetCamera(mainCamera);
	//pokemon->Init(L"pikachu", frameCnt, L"player",
	//	Vector2(13.0f, 0));

	//// bulbasaur
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

	//// rattata
	//frameCnt[STATE_IDLE] = 16;
	//frameCnt[STATE_ATTACK] = 32;
	//frameCnt[STATE_HURT] = 8;
	//frameCnt[STATE_MOVE] = 24;
	//frameCnt[STATE_SPECIAL_ATTACK] = 32;
	//pokemon->SetTileMap(tile);
	//pokemon->SetCamera(mainCamera);
	//pokemon->Init(L"rattata", frameCnt, L"player",
	//	Vector2(13.0f, 0));

	// pidgey
	frameCnt[STATE_IDLE] = 16;
	frameCnt[STATE_ATTACK] = 24;
	frameCnt[STATE_HURT] = 8;
	frameCnt[STATE_MOVE] = 24;
	frameCnt[STATE_SPECIAL_ATTACK] = 16;
	pokemon->SetTileMap(tile);
	pokemon->SetCamera(mainCamera);
	pokemon->Init(L"pidgey", frameCnt, L"player",
		Vector2(13.0f, 0));

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

	FindPokemon();
}

void ExploreScene::DebugInit()
{
	HRESULT hr = D3DXCreateFont(
		D2D::GetDevice(),
		15,					// �ʺ�
		0,					// ����
		FW_BOLD,			// �β�, �β��� ���ڴٴ°�
		1,					// �� ���� ũ�� �ٲ� �� ����
		FALSE,				// ���Ÿ�ü ��� ����
		DEFAULT_CHARSET,	// �� ���� ũ�� �ٲ� �� ����
		OUT_DEFAULT_PRECIS,	// �ܰ��� ó��
		DEFAULT_QUALITY,	// ����Ƽ
							// FF_DONTCARE ���� ũ�� ������� ū �༮ �������� ����ִ°� �ʺ�� ���� �߿�
		DEFAULT_PITCH || FF_DONTCARE,		// �ɼ�, 
		L"����",
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

		// ��Ƽ����Ʈ�� DrawTextA
		font->DrawTextW(
			// �̹��� 2D ��ǥ���� ���°� sprite��� ��
			NULL,
			str.c_str(),
			-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
			&rc,
			// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
			// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
			DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
								 // 0x~~ �̰� �������� �Լ��� ����
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

		// ��Ƽ����Ʈ�� DrawTextA
		font->DrawTextW(
			// �̹��� 2D ��ǥ���� ���°� sprite��� ��
			NULL,
			str.c_str(),
			-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
			&rc,
			// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
			// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
			DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
			// 0x~~ �̰� �������� �Լ��� ����
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

		// ��Ƽ����Ʈ�� DrawTextA
		font->DrawTextW(
			// �̹��� 2D ��ǥ���� ���°� sprite��� ��
			NULL,
			str.c_str(),
			-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
			&rc,
			// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
			// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
			DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
			// 0x~~ �̰� �������� �Լ��� ����
			//D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
			0xFFFF0000
		);
	}

	// radian
	//rc.top += 20;
	//str = L"Current Radian : ";
	//str += to_wstring(tile->GetTransform()->GetZRadian());
	//
	//// ��Ƽ����Ʈ�� DrawTextA
	//font->DrawTextW(
	//	// �̹��� 2D ��ǥ���� ���°� sprite��� ��
	//	NULL,
	//	str.c_str(),
	//	-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
	//	&rc,
	//	// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
	//	// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
	//	DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
	//						 // 0x~~ �̰� �������� �Լ��� ����
	//						 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
	//	0xFFFF0000
	//);

	// ���콺�� Ÿ�� ���� ���� ��
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

			// ��Ƽ����Ʈ�� DrawTextA
			font->DrawTextW(
				// �̹��� 2D ��ǥ���� ���°� sprite��� ��
				NULL,
				str.c_str(),
				-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
				&rc,
				// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
				// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
				DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
				// 0x~~ �̰� �������� �Լ��� ����
				//D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
				0xFFFF0000
			);
		}

		rc.top += 20;
		
		// tile block ����
		{
			bool isBlock = tile->GetTileInfo(
				currentTile.x, currentTile.y).block;

			str = L"Tile Block: ";
			str += isBlock ? L"True" : L"False";

			// ��Ƽ����Ʈ�� DrawTextA
			font->DrawTextW(
				// �̹��� 2D ��ǥ���� ���°� sprite��� ��
				NULL,
				str.c_str(),
				-1,	// ��ü ������ -1, �ƴϸ� ���ڿ� ���̸�ŭ �ϸ��
				&rc,
				// DT_NOCLIP�� rc�� ������� ����ϰڴٴ°�
				// �̰ž��� rc�� 10,10�� ��ǥ ������ �Ǵ°�
				DT_LEFT | DT_NOCLIP, // �ɼ�, ���� ���ķ� �ϰڴٴ°�
									 // 0x~~ �̰� �������� �Լ��� ����
									 //D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)
				0xFFFF0000
			);
		}
	}
}

void ExploreScene::MapLoad()
{
	Json::ReadJsonData(L"Save/TileMap.Json", jsonTileMap);

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

void ExploreScene::FindPokemon()
{
	Pokemon* targetEnemy = FindNearPokemon();

	if (pokemon->GetEnemy() == NULL 
		|| pokemon->GetEnemy()->GetIsDied()
		|| pokemon->GetEnemy() != targetEnemy) {
		pokemon->SetEnemy(targetEnemy);
	}
	
	pokemon2->SetEnemy(enemy);

	enemy->SetEnemy(pokemon);
	enemy2->SetEnemy(pokemon);
}

Pokemon * ExploreScene::FindNearPokemon(bool isEnemy)
{
	if(isEnemy) {
		float dist;
		if (enemy->GetIsDied()) dist = BIGNUM;
		else
			dist = (pokemon->GetTransform()->GetWorldPosition() -
					enemy->GetTransform()->GetWorldPosition()).Length();
		float dist2;
		if (enemy2->GetIsDied()) dist2 = BIGNUM;
		else
			dist2 = (pokemon->GetTransform()->GetWorldPosition() -
					enemy2->GetTransform()->GetWorldPosition()).Length();

		if (dist <= dist2)
			return enemy;
		else
			return enemy2;
	}
	else {
		return pokemon;
	}
}

void ExploreScene::UpdateCameraChange(Vector2 tileScale, Vector2 pokemonScale)
{
	tile->GetTransform()->SetScale(tileScale);
	
	// ������ ���� -> �̻���
	//Transform temp;
	//temp.SetScale(tileScale);
	//
	//tile->GetTransform()->ScaleLerp(
	//	tile->GetTransform(), &temp, 
	//	FRAME->GetElapsedTime());

	tile->UpdateTileCenterPos();

	UpdatePokemonChange(pokemon, pokemonScale);
	UpdatePokemonChange(pokemon2, pokemonScale);

	UpdatePokemonChange(enemy, pokemonScale);
	UpdatePokemonChange(enemy2, pokemonScale);
}

void ExploreScene::UpdatePokemonChange(Pokemon * pokemon, Vector2 scale)
{
	tagPokemonInfo pokemonInfo = pokemon->GetPokemonInfo();
	Transform* transform = pokemon->GetTransform();

	transform->SetScale(scale);

	pokemon->GetHpBar()->SetScale(transform->GetScale());

	// ������ ���� -> �̻���
	//Transform temp;
	//temp.SetScale(scale);
	//
	//transform->ScaleLerp(transform, &temp, 
	//	FRAME->GetElapsedTime());

	pokemon->CaculateAttackRange();

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
