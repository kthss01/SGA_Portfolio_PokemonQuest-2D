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

	// ��ġ �� ũ�� ������
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

	// Ÿ�� ��ġ �� ũ�� ����
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


	// subTile
	rc.top += 20;
	Vector2 tilePos = subTile->GetTransform()->GetWorldPosition();
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

	Vector2 tileScale = subTile->GetTransform()->GetScale();
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

	// ���콺�� Ÿ�� ���� ���� ��
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
	*/
}