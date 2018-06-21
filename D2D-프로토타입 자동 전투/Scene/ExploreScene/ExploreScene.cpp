#include "stdafx.h" 
#include "ExploreScene.h"

#include "GameObject\TileMap.h"
#include "./Common/Camera.h"

ExploreScene::ExploreScene()
{
}

ExploreScene::~ExploreScene()
{
}

void ExploreScene::Init()
{
	mainCamera = new Camera;
	
	LPDIRECT3DTEXTURE9 tileTex = TEXTURE->GetTexture(L"Tile");

	tile = new TileMap;
	tile->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(TILE_WIDTH, TILE_HEIGHT), 
		Vector2(
			-TILE_ROW * TILE_WIDTH / 2,
			-TILE_COL * TILE_HEIGHT / 2));
	tile->SetTexture(tileTex);
	tile->SetCamera(mainCamera);
}

void ExploreScene::Release()
{
	SAFE_RELEASE(tile);
	SAFE_DELETE(tile);

	SAFE_DELETE(mainCamera);
}

void ExploreScene::Update()
{
	mainCamera->UpdateCamToDevice();
	tile->GetTransform()->DefaultControl2();
}

void ExploreScene::Render()
{
	tile->Render();
}
