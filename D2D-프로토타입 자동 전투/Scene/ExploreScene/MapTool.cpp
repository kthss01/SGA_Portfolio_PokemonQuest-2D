#include "stdafx.h"
#include "MapTool.h"

#include "GameObject\TileMap.h"
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

	LPDIRECT3DTEXTURE9 tileTex = TEXTURE->GetTexture(L"tile_forest");

	tile = new TileMap;
	tile->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1),
		Vector2(TILE_WIDTH, TILE_HEIGHT),
		Vector2(
			-TILE_ROW * TILE_WIDTH / 2,
			-TILE_COL * TILE_HEIGHT / 2));
	tile->SetTexture(tileTex);
	tile->SetCamera(mainCamera);

	isDebug = true;
}

void MapTool::Release()
{
	SAFE_RELEASE(tile);
	SAFE_DELETE(tile);

	SAFE_DELETE(mainCamera);

	SAFE_RELEASE(font);
}

void MapTool::Update()
{
	mainCamera->UpdateCamToDevice();
	tile->Update();

	if (INPUT->GetKeyDown(VK_F11))
		isDebug = !isDebug;
}

void MapTool::Render()
{
	tile->Render();
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
	Vector2 mousePos;
	RECT rc = { 10, 70, 0,0 };

	Util::GetMousePosWithScreen(&mousePos);

	wstring str;
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

	Vector2 tilePos = tile->GetTransform()->GetWorldPosition();

	rc.top += 20;
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

	Vector2 tileScale = tile->GetTransform()->GetScale();

	rc.top += 20;
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
		tilePos.x - (TILE_ROW * TILE_WIDTH / 2) * tileScale.x &&
		mousePos.x <=
		tilePos.x + (TILE_ROW * TILE_WIDTH / 2) * tileScale.x &&
		mousePos.y >=
		tilePos.y - (TILE_COL * TILE_HEIGHT / 2) * tileScale.y &&
		mousePos.y <=
		tilePos.y + (TILE_COL * TILE_HEIGHT / 2) * tileScale.y) {

		rc.top += 20;

		POINT currentTile;
		currentTile.x = (mousePos.x - tilePos.x +
			(TILE_ROW * TILE_WIDTH / 2) * tileScale.x) /
			(int)(TILE_WIDTH * tileScale.x);
		currentTile.y = (mousePos.y - tilePos.y +
			(TILE_COL * TILE_HEIGHT / 2) * tileScale.y) /
			(int)(TILE_HEIGHT * tileScale.y);

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