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
	// 알파값 제거
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
	// 셰이더로 렌더
	UINT iPassNum = 0;

	this->pEffect->Begin(
		&iPassNum,			// pEffect에 있는 패스 수를 받아온다.
		0					// 플래그 
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
	// 쉐이더 초기화
	// 에러값 확인이 어려우므로 
	// 쉐이더 로딩시 에러가 나면 에러 내용을 받아올 꺼
	LPD3DXBUFFER pError = NULL;

	// 쉐이더 옵션 지정
	DWORD shaderFlag = 0;
	// 지정되어야만 디버깅시 쉐이더 확인이 가능함
#ifdef _DEBUG
	shaderFlag |= D3DXSHADER_DEBUG;
#endif // _DEBUG

	// 쉐이더 내부에서 쓸 수 있는 게 #define, #include 
	D3DXCreateEffectFromFile(
		D2D::GetDevice(),			// 디바이스
									//L"./Shader/BaseColor.fx",	// 셰이더 파일
									//L"./Shader/TextureMapping.fx",
									//L"./Shader/MultiTexture.fx",
		shaderFile.c_str(),
		NULL,						// 셰이더 컴파일시 추가 #define
		NULL,						// 셰이더 컴파일시 추가 #include
									// include를 쓸 수 있는거
									// 외부에서 내부로 추가 할 수도 있음
									// 잘쓰진 않음
		shaderFlag,					// 셰이더 옵션
		NULL, // 셰이더 매개변수를 공유할 메모리풀
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

			// 중점 = 좌상단 + 우하단 / 2
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
		D3DUSAGE_WRITEONLY,		// dynamic 쓰게 되면
		FVF,
		D3DPOOL_MANAGED,		// 이걸로 해줘야함 default 해주면 data 남아있지 않음
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
				pos.y + // 현재 위치
				(-(TILE_HEIGHT * (float)TILE_COL / 2) // 0 0 타일로 이동
					+ (float)TILE_HEIGHT / 2 // 중점으로 이동
					+ i * TILE_HEIGHT) // 타일 번호의 중점 계산
				* scale.y; // 스케일값 계산
				
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
			// 좌상단
			vertices[(i * col + j) * 4 + k].uv = startUV;
			break;
		case 1:
			// 우상단
			vertices[(i * col + j) * 4 + k].uv = 
				Vector2(endUV.x, startUV.y);
			break;
		case 2:
			// 좌하단
			vertices[(i * col + j) * 4 + k].uv = 
				Vector2(startUV.x, endUV.y);
			break;
		case 3:
			// 우하단
			vertices[(i * col + j) * 4 + k].uv = endUV;
			break;
		}
	}

	if(!isStart)
		SetVertexBuffer();
}

bool TileMap::CheckTileBlock(Vector2 uv)
{
	// Ground만 false로 반환

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
