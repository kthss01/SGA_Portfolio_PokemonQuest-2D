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

	Vector2 tilePos = tile->GetTransform()->GetWorldPosition();

	rc.top += 20;
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

	Vector2 tileScale = tile->GetTransform()->GetScale();

	rc.top += 20;
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