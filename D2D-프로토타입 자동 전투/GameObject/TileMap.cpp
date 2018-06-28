#include "stdafx.h"
#include "TileMap.h"

#include "Common\Camera.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

void TileMap::Init(wstring shaderFile, const Vector2 uv, 
	int row, int col, int mapSize, int vertexSize, int indexSize,
	const Vector2 tileSize, const Vector2 pivot)
{
	this->row = row;
	this->col = col;
	this->mapSize = mapSize;
	this->vertexSize = vertexSize;
	this->indexSize = indexSize;

	InitShader(shaderFile);

	InitVertex(tileSize, uv, pivot);

	InitBuffer();

	transform->UpdateTransform();

	pTex = NULL;

	//curTile.x = 0;
	//curTile.y = 0;
}

void TileMap::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);

	SAFE_DELETE(collider);
}

void TileMap::Update()
{
	this->transform->DefaultControl2();

	//if (INPUT->GetKeyDown(VK_UP) && curTile.y > 0)
	//	curTile.y -= 1.0f;
	//if (INPUT->GetKeyDown(VK_DOWN) && curTile.y < TILE_MAXFRAME_Y) 
	//	curTile.y += 1.0f;
	//if (INPUT->GetKeyDown(VK_LEFT) && curTile.x > 0) 
	//	curTile.x -= 1.0f;
	//if (INPUT->GetKeyDown(VK_RIGHT) && curTile.x < TILE_MAXFRAME_X)
	//	curTile.x += 1.0f;

	//UpdateUV();
}

void TileMap::Render()
{
	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetVector("maxFrame",
		&D3DXVECTOR4(
			1.0f, 1.0f,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			0.0f, 0.0f,
			0.0f, 0.0f));

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");

	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void TileMap::RenderRect()
{
	// ���̴��� ����
	UINT iPassNum = 0;

	this->pEffect->Begin(
		&iPassNum,			// pEffect�� �ִ� �н� ���� �޾ƿ´�.
		0					// �÷��� 
	);
	{
		for (UINT i = 0; i < iPassNum; i++) {
			this->pEffect->BeginPass(i);
			{
				D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
				D2D::GetDevice()->SetIndices(ib);
				D2D::GetDevice()->SetFVF(FVF);

				D2D::GetDevice()->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, vertexSize, 0, mapSize * 2);
			}
			this->pEffect->EndPass();
		}
	}

	this->pEffect->End();
}

void TileMap::InitShader(wstring shaderFile)
{
	// ���̴� �ʱ�ȭ
	// ������ Ȯ���� �����Ƿ� 
	// ���̴� �ε��� ������ ���� ���� ������ �޾ƿ� ��
	LPD3DXBUFFER pError = NULL;

	// ���̴� �ɼ� ����
	DWORD shaderFlag = 0;
	// �����Ǿ�߸� ������ ���̴� Ȯ���� ������
#ifdef _DEBUG
	shaderFlag |= D3DXSHADER_DEBUG;
#endif // _DEBUG

	// ���̴� ���ο��� �� �� �ִ� �� #define, #include 
	D3DXCreateEffectFromFile(
		D2D::GetDevice(),			// ����̽�
									//L"./Shader/BaseColor.fx",	// ���̴� ����
									//L"./Shader/TextureMapping.fx",
									//L"./Shader/MultiTexture.fx",
		shaderFile.c_str(),
		NULL,						// ���̴� �����Ͻ� �߰� #define
		NULL,						// ���̴� �����Ͻ� �߰� #include
									// include�� �� �� �ִ°�
									// �ܺο��� ���η� �߰� �� ���� ����
									// �߾��� ����
		shaderFlag,					// ���̴� �ɼ�
		NULL, // ���̴� �Ű������� ������ �޸�Ǯ
		&pEffect,
		&pError
	);

	if (pError != NULL) {
		string lastError = (char*)pError->GetBufferPointer();

		MessageBox(NULL, String::StringToWString(lastError).c_str(),
			L"Shader Error", MB_OK);
		SAFE_RELEASE(pError);
	}
}

void TileMap::InitVertex(Vector2 size, Vector2 uv, Vector2 pivot)
{
	vertices = new Vertex[vertexSize];

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			for (int k = 0; k < 4; k++) {
				vertices[(i * col + j) * 4 + k].position =
					Vector2(
						j * size.y + (k % 2) * size.y + pivot.y,
						i * size.x + (k / 2) * size.x + pivot.x);

				vertices[(i * col + j) * 4 + k].color = 0xffffffff;
			}

			ChangeTile(i, j, uv, true);

			// ���� = �»�� + ���ϴ� / 2
			tileInfo[i][j].center =
				vertices[(i * col + j) * 4 + 0].position +
					vertices[(i * col + j) * 4 + 3].position;
			tileInfo[i][j].center.x /= 2;
			tileInfo[i][j].center.y /= 2;
				
			tileInfo[i][j].block = this->CheckTileBlock(tileInfo[i][j].uv);
		}
	}

	indices = new DWORD[indexSize];
	int count = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			indices[count++] = (i * col + j) * 4;
			indices[count++] = (i * col + j) * 4 + 1;
			indices[count++] = (i * col + j) * 4 + 2;

			indices[count++] = (i * col + j) * 4 + 1;
			indices[count++] = (i * col + j) * 4 + 3;
			indices[count++] = (i * col + j) * 4 + 2;
		}
	}

	stride = sizeof(Vertex);
	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
}

void TileMap::InitBuffer()
{
	HRESULT hr = D2D::GetDevice()->CreateVertexBuffer(
		stride * vertexSize,
		D3DUSAGE_WRITEONLY,		// dynamic ���� �Ǹ�
		FVF,
		D3DPOOL_MANAGED,		// �̰ɷ� ������� default ���ָ� data �������� ����
		&vb,
		NULL
	);
	assert(SUCCEEDED(hr));

	Vertex * pVertex = NULL;
	hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pVertex, vertices, stride * vertexSize);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	hr = D2D::GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * indexSize,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&ib,
		NULL
	);
	assert(SUCCEEDED(hr));

	void* pIndex;
	hr = ib->Lock(0, 0, &pIndex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pIndex, indices, sizeof(DWORD) * indexSize);
	hr = ib->Unlock();
	assert(SUCCEEDED(hr));
}

void TileMap::SetVertexBuffer()
{
	Vertex * pVertex = NULL;
	HRESULT hr = vb->Lock(0, 0, (void**)&pVertex, 0);
	assert(SUCCEEDED(hr));
	memcpy(pVertex, this->vertices, stride * vertexSize);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));
}

void TileMap::UpdateTileInfo()
{
	for (int i = 0; i < TILE_ROW; i++) {
		for (int j = 0; j < TILE_COL; j++) {
			ChangeTile(i, j, tileInfo[i][j].uv);
		}
	}

	this->SetVertexBuffer();
}

void TileMap::UpdateTileCenterPos()
{
	Vector2 pos = transform->GetWorldPosition();
	Vector2 scale = transform->GetScale();

	for (int i = 0; i < TILE_ROW; i++) {
		for (int j = 0; j < TILE_COL; j++) {
			tileInfo[i][j].center.x =
				pos.x +
				(-(TILE_WIDTH * (float)TILE_ROW / 2)
					+ (float)TILE_WIDTH / 2
					+ j * TILE_WIDTH)
				* scale.x;

			tileInfo[i][j].center.y =
				pos.y + // ���� ��ġ
				(-(TILE_HEIGHT * (float)TILE_COL / 2) // 0 0 Ÿ�Ϸ� �̵�
					+ (float)TILE_HEIGHT / 2 // �������� �̵�
					+ i * TILE_HEIGHT) // Ÿ�� ��ȣ�� ���� ���
				* scale.y; // �����ϰ� ���
				
		}
	}
}

void TileMap::ChangeTile(int i, int j, Vector2 uv, bool isStart)
{
	tileInfo[i][j].uv = uv;

	Vector2 startUV = Vector2(
		tileInfo[i][j].uv.x / TILE_MAXFRAME_X,
		tileInfo[i][j].uv.y / TILE_MAXFRAME_Y);
	Vector2 endUV;
	endUV.x = (tileInfo[i][j].uv.x + 1.0f - TILE_PADDING) / TILE_MAXFRAME_X;
	endUV.y = (tileInfo[i][j].uv.y + 1.0f - TILE_PADDING) / TILE_MAXFRAME_Y;

	tileInfo[i][j].block = this->CheckTileBlock(uv);

	for (int k = 0; k < 4; k++) {
		switch (k) {
		case 0:
			// �»��
			vertices[(i * col + j) * 4 + k].uv = startUV;
			break;
		case 1:
			// ����
			vertices[(i * col + j) * 4 + k].uv = 
				Vector2(endUV.x, startUV.y);
			break;
		case 2:
			// ���ϴ�
			vertices[(i * col + j) * 4 + k].uv = 
				Vector2(startUV.x, endUV.y);
			break;
		case 3:
			// ���ϴ�
			vertices[(i * col + j) * 4 + k].uv = endUV;
			break;
		}
	}

	if(!isStart)
		SetVertexBuffer();
}

bool TileMap::CheckTileBlock(Vector2 uv)
{
	// Ground�� false�� ��ȯ

	if (FLOATEQUAL(uv.x, 5.0f) && FLOATEQUAL(uv.y, 1.0f)) return false;
	if (FLOATEQUAL(uv.x, 16.0f) && FLOATEQUAL(uv.y, 1.0f)) return false;
	if (FLOATEQUAL(uv.x, 19.0f) && FLOATEQUAL(uv.y, 1.0f)) return false;
	if (FLOATEQUAL(uv.x, 22.0f) && FLOATEQUAL(uv.y, 1.0f)) return false;

	if (uv.x >= 12.0f && uv.x < 15.0f) {
		if (uv.y >= 0 && uv.y <= 3) return false;

		if (uv.x <= 14.0f && 
			uv.y >= 15.0f && uv.y <= 23.0f) return false;

		if (FLOATEQUAL(uv.y, 4.0f)) {
			if (FLOATEQUAL(uv.x, 12.0f))	return false;
			if (FLOATEQUAL(uv.x, 13.0f))	return false;
		}

		if (FLOATEQUAL(uv.y, 5.0f)) {
			if (FLOATEQUAL(uv.x, 12.0f))	return false;
			if (FLOATEQUAL(uv.x, 14.0f))	return false;
		}
		if (FLOATEQUAL(uv.x, 13.0f) && FLOATEQUAL(uv.y, 6.0f)) return false;

		if (FLOATEQUAL(uv.y, 7)) return false;
		if (FLOATEQUAL(uv.x, 13.0f) && FLOATEQUAL(uv.y, 8.0f)) return false;
		if (FLOATEQUAL(uv.x, 13.0f) && FLOATEQUAL(uv.y, 9.0f)) return false;

		if (FLOATEQUAL(uv.y, 10.0f)) {
			if (FLOATEQUAL(uv.x, 12.0f))	return false;
			if (FLOATEQUAL(uv.x, 14.0f))	return false;
		}

		if (FLOATEQUAL(uv.x, 13.0f) && FLOATEQUAL(uv.y, 11.0f)) return false;
		if (FLOATEQUAL(uv.x, 13.0f) && FLOATEQUAL(uv.y, 12.0f)) return false;

		if (FLOATEQUAL(uv.y, 13.0f)) {
			if (FLOATEQUAL(uv.x, 12.0f))	return false;
			if (FLOATEQUAL(uv.x, 14.0f))	return false;
		}

		if (FLOATEQUAL(uv.x, 13.0f) && FLOATEQUAL(uv.y, 14.0f)) return false;
	}

	return true;
}
