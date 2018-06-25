#include "stdafx.h"
#include "MapTool.h"

#include "GameObject\TileMap.h"
#include "GameObject\Rect.h"
#include "./Common/Camera.h"

MapTool::MapTool()
{
}

MapTool::~MapTool()
{
}

void MapTool::Init()
{
	mainCamera = new Camera;

	SubTileInit();

	TileInit();

	UIInit();

	DebugInit();

	isDebug = true;
}

void MapTool::Release()
{
	SAFE_RELEASE(subTile);
	SAFE_DELETE(subTile);

	SAFE_RELEASE(tile);
	SAFE_DELETE(tile);

	SAFE_DELETE(mainCamera);

	SAFE_RELEASE(font);

	SAFE_DELETE(panel);
	for (int i = 0; i < 5; i++)
		SAFE_DELETE(ui[i]);
}

void MapTool::Update()
{
	mainCamera->UpdateCamToDevice();
	//subTile->Update();
	//tile->Update();
	//panel->Update();

	// 위치 및 크기 조정용
	for (int i = 0; i < 1; i++)
		ui[i]->Update();
	if (INPUT->GetKeyDown(VK_TAB)) {
		Vector2 pos = ui[0]->GetTransform()->GetWorldPosition();
		Vector2 scale = ui[0]->GetTransform()->GetScale();
		
		//Vector2 pos = panel->GetTransform()->GetWorldPosition();
		//Vector2 scale = panel->GetTransform()->GetScale();
		//float radian = panel->GetTransform()->GetZRadian();

		int temp = 0;
	}

	if (INPUT->GetKeyDown(VK_F11))
		isDebug = !isDebug;
}

void MapTool::Render()
{
	subTile->Render();
	tile->Render();

	panel->Render();
	for (int i = 0; i < 5; i++)
		ui[i]->Render();

	if (isDebug)
		DebugRender();
}

void MapTool::TileInit()
{
	LPDIRECT3DTEXTURE9 tileTex = TEXTURE->GetTexture(L"tile_forest");

	tile = new TileMap;
	tile->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		TILE_ROW, TILE_COL, TILE_SIZE,
		TILE_VERTEX_SIZE, TILE_INDEX_SIZE,
		Vector2(TILE_WIDTH, TILE_HEIGHT),
		Vector2(
			-TILE_ROW * TILE_WIDTH / 2,
			-TILE_COL * TILE_HEIGHT / 2));
	tile->SetTexture(tileTex);
	tile->SetCamera(mainCamera);

	// 타일 위치 및 크기 설정
	tile->GetTransform()->SetScale(Vector2(0.9f, 0.9f));
	tile->GetTransform()->SetWorldPosition(Vector2(-130.0f, -1.0f));
}

void MapTool::SubTileInit()
{
	LPDIRECT3DTEXTURE9 tileTex = TEXTURE->GetTexture(L"tile_forest");

	subTile = new TileMap;
	subTile->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		MAPTOOL_ROW, MAPTOOL_COL, MAPTOOL_SIZE,
		MAPTOOL_VERTEX_SIZE, MAPTOOL_INDEX_SIZE,
		Vector2(TILE_WIDTH, TILE_HEIGHT),
		Vector2(
			-MAPTOOL_ROW * MAPTOOL_WIDTH / 2,
			-MAPTOOL_COL * MAPTOOL_HEIGHT / 2));
	subTile->SetTexture(tileTex);
	subTile->SetCamera(mainCamera);

	subTile->GetTransform()->SetScale(Vector2(1.5f, 1.2f));
	subTile->GetTransform()->SetWorldPosition(Vector2(420.0f, 0.0f));
}

void MapTool::UIInit()
{
	panel = new Rect;
	panel->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	panel->SetTexture(TEXTURE->GetTexture(L"ui_panel"));
	panel->SetCamera(mainCamera);
	panel->GetTransform()->SetScale(Vector2(2.8f, 1.0f));
	panel->GetTransform()->SetWorldPosition(
		Vector2(300, -216));
	panel->GetTransform()->SetRotateLocal(-D3DX_PI / 2);

	for (int i = 0; i < 5; i++) {
		ui[i] = new Rect;
		ui[i]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
		ui[i]->SetCamera(mainCamera);

		switch (i)
		{
			// ui_new
		case 0:
			ui[i]->SetTexture(TEXTURE->GetTexture(L"ui_new"));
			ui[i]->GetTransform()->SetScale(Vector2(0.4f, 0.4f));
			ui[i]->GetTransform()->SetWorldPosition(
				Vector2(310, -310));
			break;
			// ui_save
		case 1:
			ui[i]->SetTexture(TEXTURE->GetTexture(L"ui_save"));
			ui[i]->GetTransform()->SetScale(Vector2(0.4f, 0.4f));
			ui[i]->GetTransform()->SetWorldPosition(
				Vector2(310, -260));
			break;
			// ui_load
		case 2:
			ui[i]->SetTexture(TEXTURE->GetTexture(L"ui_load"));
			ui[i]->GetTransform()->SetScale(Vector2(0.4f, 0.4f));
			ui[i]->GetTransform()->SetWorldPosition(
				Vector2(310, -210));
			break;
			// ui_up
		case 3:
			ui[i]->SetTexture(TEXTURE->GetTexture(L"ui_up"));
			ui[i]->GetTransform()->SetScale(Vector2(0.4f, 0.4f));
			ui[i]->GetTransform()->SetWorldPosition(
				Vector2(310, -160));
			break;
			// ui_down
		case 4:
			ui[i]->SetTexture(TEXTURE->GetTexture(L"ui_down"));
			ui[i]->GetTransform()->SetScale(Vector2(0.4f, 0.4f));
			ui[i]->GetTransform()->SetWorldPosition(
				Vector2(310, -110));
			break;
		}
	}
}

void MapTool::DebugInit()
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

void MapTool::DebugRender()
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


	// subTile
	rc.top += 20;
	Vector2 tilePos = subTile->GetTransform()->GetWorldPosition();
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

	Vector2 tileScale = subTile->GetTransform()->GetScale();
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

	// 마우스가 타일 위에 있을 때
	if (mousePos.x >=
		tilePos.x - (MAPTOOL_COL * MAPTOOL_HEIGHT / 2) * tileScale.x &&
		mousePos.x <=
		tilePos.x + (MAPTOOL_COL * MAPTOOL_HEIGHT / 2) * tileScale.x &&
		mousePos.y >=
		tilePos.y - (MAPTOOL_ROW * MAPTOOL_WIDTH / 2) * tileScale.y &&
		mousePos.y <=
		tilePos.y + (MAPTOOL_ROW * MAPTOOL_WIDTH / 2) * tileScale.y) {

		rc.top += 20;
		POINT currentTile;
		currentTile.x = (mousePos.x - tilePos.x +
			(MAPTOOL_COL * MAPTOOL_HEIGHT / 2) * tileScale.x) /
			(int)(MAPTOOL_HEIGHT * tileScale.x);
		currentTile.y = (mousePos.y - tilePos.y +
			(MAPTOOL_ROW * MAPTOOL_WIDTH / 2) * tileScale.y) /
			(int)(MAPTOOL_WIDTH * tileScale.y);

		// currentTile
		{
			str = L"Current Sub Tile : (";
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
	}

	// tile
	/*
	rc.top += 20;
	Vector2 tilePos = tile->GetTransform()->GetWorldPosition();
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
		currentTile.x = (mousePos.x - tilePos.x +
		(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
		(int)(TILE_HEIGHT * tileScale.x);
		currentTile.y = (mousePos.y - tilePos.y +
		(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
		(int)(TILE_WIDTH * tileScale.y);

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
	}
	*/
}