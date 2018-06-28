#include "stdafx.h"
#include "Pokemon.h"

#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"
#include "GameObject\TileMap.h"


Pokemon::Pokemon()
{
}

Pokemon::~Pokemon()
{
}

void Pokemon::Init(wstring name, int* frameCnt, Vector2 pivot)
{
	InitShader();
	InitVertex(pivot);
	InitBuffer();

	transform->UpdateTransform();

	Vector2 center = Vector2(0, 0);
	Vector2 halfSize = center - (vertice[1].position - Vector2(1,1));

	this->collider = new RectCollider;
	this->collider->SetBound(&center, &halfSize);

	deltaTime = 0.0f;

	AnimationData  data;
	for (int i = 0; i < STATE_END; i++) {
		clips[i] = new AnimationClip;
		for (int j = 0; j < frameCnt[i]; j++) {
			data.keyName = to_wstring(i) + L"_" + to_wstring(j);
			data.maxFrame = Vector2(frameCnt[i], 1.0f);
			data.currentFrame = Vector2(j, 0.0f);
			clips[i]->PushAnimationData(data);
		}
	}

	targetTransform = new Transform;

	pokemonInfo.name = name;
	pokemonInfo.state = STATE_IDLE;
	pokemonInfo.dir = DIRECTION_BOTTOM;
	for (int i = 0; i < STATE_END; i++) {
		pokemonInfo.frameCnt[i] = frameCnt[i];
		wstring str = pokemonInfo.name;
		switch (i)
		{
		case STATE_IDLE:
			str += L"_idle";
			break;
		case STATE_MOVE:
			str += L"_movement";
			break;
		case STATE_HURT:
			str += L"_hurt";
			break;
		case STATE_ATTACK:
			str += L"_attack";
			break;
		case STATE_SPECIAL_ATTACK:
			str += L"_special_attack";
			break;
		}
		pokemonInfo.pTex[i] = TEXTURE->GetTexture(str);
	}
	pokemonInfo.isDied = false;
	pokemonInfo.moveSpeed = 5.0f;

	pokemonInfo.curTile = { 0, 0 };
	pokemonInfo.targetTile = { -1, -1 };

	transform->SetWorldPosition(
		tile->GetTileCenterPos(
			pokemonInfo.curTile.x, pokemonInfo.curTile.y));
}

void Pokemon::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);

	for (int i = 0; i < STATE_END; i++)
		SAFE_DELETE(clips[i]);

	SAFE_DELETE(collider);

	SAFE_DELETE(targetTransform);
}

void Pokemon::Update()
{
	this->transform->DefaultControl2();
	
	bool aniEnd = clips[pokemonInfo.state]->Update(pokemonInfo.dir);

	switch (pokemonInfo.state)
	{
	case STATE_IDLE:
		Move();
		Attack();
		break;
	case STATE_MOVE:
		Move();
		Attack();

		//switch (pokemonInfo.dir)
		//{
		//case DIRECTION_BOTTOM:
		//	
		//	break;
		//case DIRECTION_TOP:
		//
		//	break;
		//case DIRECTION_LEFT:
		//	
		//	break;
		//case DIRECTION_RIGHT:
		//	
		//	break;
		//case DIRECTION_LEFTBOTTOM:
		//	
		//	break;
		//case DIRECTION_RIGHTBOTTOM:
		//	
		//	break;
		//case DIRECTION_LEFTTOP:
		//	
		//	break;
		//case DIRECTION_RIGHTTOP:
		//	
		//	break;
		//}
		
		// �����ϸ� ���� Ÿ�Ϸ� ����
		if(MovePosition(pokemonInfo.targetTile))
			pokemonInfo.curTile = pokemonInfo.targetTile;

		if (pokemonInfo.curTile.x == pokemonInfo.targetTile.x
			&& pokemonInfo.curTile.y == pokemonInfo.targetTile.y) {
			pokemonInfo.state = STATE_IDLE;
			clips[pokemonInfo.state]->Play(pokemonInfo.dir);
		}

		break;
	case STATE_HURT:
		break;
	case STATE_ATTACK:
		break;
	case STATE_SPECIAL_ATTACK:
		break;
	}
}

void Pokemon::Render()
{
	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetVector("maxFrame",
		&D3DXVECTOR4(
			clips[pokemonInfo.state]->GetCurrentData().maxFrame.x,
			clips[pokemonInfo.state]->GetCurrentData().maxFrame.y,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			clips[pokemonInfo.state]->GetCurrentData().currentFrame.x,
			clips[pokemonInfo.state]->GetCurrentData().currentFrame.y,
			0.0f, 0.0f));

	//// test
	//this->pEffect->SetVector("maxFrame",
	//	&D3DXVECTOR4(
	//		8.0f, 1.0f,
	//		0.0f, 0.0f));
	//this->pEffect->SetVector("currentFrame",
	//	&D3DXVECTOR4(
	//		0.0f, 0.0f,
	//		0.0f, 0.0f));


	this->pEffect->SetTexture("tex", 
		pokemonInfo.pTex[pokemonInfo.state]);

	this->pEffect->SetTechnique("MyShader");

	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	transform->RenderGizmo(true);
	collider->RenderGizmo(transform);
}

void Pokemon::RenderRect()
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
				// ���࿡ �ؽ�ó �����ϴ� ����̸� pEffect->setTexture��
				//D2D::GetDevice()->SetTexture(0, pTex);

				D2D::GetDevice()->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST,
					0, 0, 4, 0, 2);
			}
			this->pEffect->EndPass();
		}
	}

	this->pEffect->End();
}

void Pokemon::InitShader()
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
		L"./Shader/ColorTexture.fx",
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

void Pokemon::InitVertex(Vector2 pivot)
{
	Vector2 size = Vector2(50, 50);
	//Vector2 pivot = Vector2(13.0f, 0);
	Vector2 uv = Vector2(1, 1);

	vertice[0].position = Vector2(-size.x + pivot.x, size.y + pivot.y);
	vertice[1].position = Vector2(-size.x + pivot.x, -size.y + pivot.y);
	vertice[2].position = Vector2(size.x + pivot.x, -size.y + pivot.y);
	vertice[3].position = Vector2(size.x + pivot.x, size.y + pivot.y);

	vertice[0].color = 0xffffffff;
	vertice[1].color = 0xffffffff;
	vertice[2].color = 0xffffffff;
	vertice[3].color = 0xffffffff;

	vertice[0].uv = Vector2(0, uv.y);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(uv.x, 0);
	vertice[3].uv = uv;

	stride = sizeof(Vertex);
	//FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
}

void Pokemon::InitBuffer()
{
	HRESULT hr = D2D::GetDevice()->CreateVertexBuffer(
		stride * 4,
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
	memcpy(pVertex, vertice, stride * 4);
	hr = vb->Unlock();
	assert(SUCCEEDED(hr));

	DWORD indices[] = {
		0,1,2,
		0,2,3,
	};

	hr = D2D::GetDevice()->CreateIndexBuffer(
		sizeof(DWORD) * 6,
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
	memcpy(pIndex, indices, sizeof(DWORD) * 6);
	hr = ib->Unlock();
	assert(SUCCEEDED(hr));
}

bool Pokemon::MovePosition(POINT targetTile)
{
	transform->Interpolate(
		transform, targetTransform,
		FRAME->GetElapsedTime() * 1.5f);

	float temp = 1.0f;
	// Ÿ�ٿ� �����Ÿ����� �����ϸ� true �ƴϸ� false
	if (abs(transform->GetWorldPosition().x -
		targetTransform->GetWorldPosition().x)
		<= temp &&
		abs(transform->GetWorldPosition().y -
			targetTransform->GetWorldPosition().y)
		<= temp) {
		transform->SetWorldPosition(
			targetTransform->GetWorldPosition());
		return true;
	}

	return false;
}

POKEMON_DIRECTION Pokemon::FindDirection(POINT curTile, POINT targetTile)
{
	if (curTile.y > targetTile.y) {
		if (curTile.x == targetTile.x)
			return DIRECTION_LEFT;
		else if (curTile.x > targetTile.x)
			return DIRECTION_LEFTTOP;
		else
			return DIRECTION_LEFTBOTTOM;
	}
	else if (curTile.y < targetTile.y) {
		if (curTile.x == targetTile.x)
			return DIRECTION_RIGHT;
		else if (curTile.x > targetTile.x)
			return DIRECTION_RIGHTTOP;
		else
			return DIRECTION_RIGHTBOTTOM;
	}
	else {
		if (curTile.x > targetTile.x)
			return DIRECTION_TOP;
		else
			return DIRECTION_BOTTOM;
	}
}

void Pokemon::Move()
{

	if (INPUT->GetKeyDown(VK_LBUTTON)) {

		Vector2 mousePos;
		Util::GetMousePosWithScreen(&mousePos);
		Vector2 tilePos = tile->GetTransform()->GetWorldPosition();
		Vector2 tileScale = tile->GetTransform()->GetScale();

		// ���콺�� Ÿ�� ���� ���� ��
		if (mousePos.x >=
			tilePos.x - (TILE_COL * TILE_HEIGHT / 2) * tileScale.x &&
			mousePos.x <=
			tilePos.x + (TILE_COL * TILE_HEIGHT / 2) * tileScale.x &&
			mousePos.y >=
			tilePos.y - (TILE_ROW * TILE_WIDTH / 2) * tileScale.y &&
			mousePos.y <=
			tilePos.y + (TILE_ROW * TILE_WIDTH / 2) * tileScale.y) {

			POINT targetTile;
			targetTile.x = (mousePos.y - tilePos.y +
				(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
				(TILE_WIDTH * tileScale.y);
			targetTile.y = (mousePos.x - tilePos.x +
				(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
				(TILE_HEIGHT * tileScale.x);

			if (targetTile.x != pokemonInfo.curTile.x ||
				targetTile.y != pokemonInfo.curTile.y) {
				if (pokemonInfo.targetTile.x != targetTile.x
					|| pokemonInfo.targetTile.y != targetTile.y) {
					POINT currentTile;
					Vector2 pos = transform->GetWorldPosition();
					currentTile.x = (pos.y - tilePos.y +
						(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
						(TILE_WIDTH * tileScale.y);
					currentTile.y = (pos.x - tilePos.x +
						(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
						(TILE_HEIGHT * tileScale.x);
					pokemonInfo.curTile = currentTile;
				}
				pokemonInfo.targetTile = targetTile;
			
				bool stateChange = false;
				bool dirChange = false;

				if (pokemonInfo.state != STATE_MOVE) {
					pokemonInfo.state = STATE_MOVE;
					stateChange = true;
				}

				POKEMON_DIRECTION dir = FindDirection(
					pokemonInfo.curTile, pokemonInfo.targetTile);
				if (pokemonInfo.dir != dir) {
					pokemonInfo.dir = dir;
					dirChange = true;
				}
				
				if(stateChange || dirChange)
					clips[pokemonInfo.state]->Play(pokemonInfo.dir);

				targetTransform->SetScale(transform->GetScale());
				Vector2 tileCenter = tile->GetTileCenterPos(
					targetTile.x, targetTile.y);
				Vector2 targetPos = tileCenter + Vector2(
					0 * transform->GetScale().x,
					-10.0f * transform->GetScale().y);
				targetTransform->SetWorldPosition(targetPos);
			}
		}
	}
}

void Pokemon::Attack()
{
}
