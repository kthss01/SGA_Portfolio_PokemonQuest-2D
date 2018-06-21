#include "stdafx.h"
#include "TileMap.h"

#include "Common\Camera.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

void TileMap::Init(wstring shaderFile, const Vector2 uv, const Vector2 size, const Vector2 pivot)
{
	InitShader(shaderFile);

	InitVertex(size, uv, pivot);

	InitBuffer();

	transform->UpdateTransform();

	pTex = NULL;
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
					0, 0, VERTEX_SIZE, 0, TILE_SIZE * 2);
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
	vertices = new Vertex[VERTEX_SIZE];

	for (int i = 0; i < TILE_ROW; i++) {
		for (int j = 0; j < TILE_COL; j++) {
			for (int k = 0; k < 4; k++) {
				vertices[(i * TILE_COL + j) * 4 + k].position =
					Vector2(
						j * size.y + (k % 2) * size.y + pivot.x,
						i * size.x + (k / 2) * size.x + pivot.y);
				switch (k) {
				case 0:
					// �»��
					vertices[(i * TILE_COL + j) * 4 + k].uv = Vector2(0, 0);
					break;
				case 1:
					// ����
					vertices[(i * TILE_COL + j) * 4 + k].uv = Vector2(0, uv.y);
					break;
				case 2:
					// ���ϴ�
					vertices[(i * TILE_COL + j) * 4 + k].uv = Vector2(uv.x, 0);
					break;
				case 3:
					// ���ϴ�
					vertices[(i * TILE_COL + j) * 4 + k].uv = uv;
					break;
				}
			}
		}
	}

	indices = new DWORD[INDEX_SIZE];
	int count = 0;
	for (int i = 0; i < TILE_ROW; i++) {
		for (int j = 0; j < TILE_COL; j++) {
			indices[count++] = (i * TILE_COL + j) * 4;
			indices[count++] = (i * TILE_COL + j) * 4 + 1;
			indices[count++] = (i * TILE_COL + j) * 4 + 2;

			indices[count++] = (i * TILE_COL + j) * 4 + 1;
			indices[count++] = (i * TILE_COL + j) * 4 + 3;
			indices[count++] = (i * TILE_COL + j) * 4 + 2;

			//tiles[i][j].x = i;
			//tiles[i][j].y = j;
			//
			//tiles[i][j].position[0] = vertices[(i * TILE_COL + j) * 4].position;
			//tiles[i][j].position[1] = vertices[(i * TILE_COL + j) * 4 + 1].position;
			//tiles[i][j].position[2] = vertices[(i * TILE_COL + j) * 4 + 2].position;
			//
			//tiles[i][j].position[3] = vertices[(i * TILE_COL + j) * 4 + 1].position;
			//tiles[i][j].position[4] = vertices[(i * TILE_COL + j) * 4 + 3].position;
			//tiles[i][j].position[5] = vertices[(i * TILE_COL + j) * 4 + 2].position;
		}
	}

	stride = sizeof(Vertex);
	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
}

void TileMap::InitBuffer()
{
	HRESULT hr = D2D::GetDevice()->CreateVertexBuffer(
		stride * VERTEX_SIZE,
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
	memcpy(pVertex, vertices, stride * VERTEX_SIZE);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	hr = D2D::GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * INDEX_SIZE,
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
	memcpy(pIndex, indices, sizeof(DWORD) * INDEX_SIZE);
	hr = ib->Unlock();
	assert(SUCCEEDED(hr));
}
