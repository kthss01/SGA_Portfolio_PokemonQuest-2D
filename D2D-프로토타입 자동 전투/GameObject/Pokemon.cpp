#include "stdafx.h"
#include "Pokemon.h"

#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"
#include "GameObject\TileMap.h"

#include "Scene\ExploreScene\AStar.h"

#include "Utilities\ProgressBar.h"

Pokemon::Pokemon()
{
}

Pokemon::~Pokemon()
{
}

void Pokemon::Init(wstring name, int* frameCnt, wstring team,
	Vector2 pivot, POINT startPos)
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
	delayTime = 0.0f;

	AnimationData  data;
	for (int i = 0; i < STATE_END; i++) {
		clips[i] = new AnimationClip;
		for (int j = 0; j < frameCnt[i]; j++) {
			if (frameCnt[i] == 32)
				data.time = 0.15f;

			data.keyName = to_wstring(i) + L"_" + to_wstring(j);
			data.maxFrame = Vector2(frameCnt[i], 1.0f);
			data.currentFrame = Vector2(j, 0.0f);
			clips[i]->PushAnimationData(data);
		}
	}

	targetTransform = new Transform;
	tempTransform = new Transform;

	// pokemonInfo init
	pokemonStatus.name = name;
	pokemonStatus.state = STATE_IDLE;
	pokemonStatus.dir = DIRECTION_BOTTOM;
	for (int i = 0; i < STATE_END; i++) {
		pokemonStatus.frameCnt[i] = frameCnt[i];
		wstring str = pokemonStatus.name;
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
		pokemonStatus.pTex[i] = TEXTURE->GetTexture(str);
	}

	pokemonStatus.isDied = false;
	pokemonStatus.moveSpeed = 5.0f;
	pokemonStatus.attackSpeed = 0.4f;

	CalculateAttackRange();

	pokemonStatus.curTile = startPos;
	pokemonStatus.targetTile = { -1, -1 };

	pokemonStatus.attack = 5;
	pokemonStatus.defense = 0;
	pokemonStatus.maxHp = 25;
	pokemonStatus.curHp = 25;

	// ��ġ init
	Vector2 tileCenter = tile->GetTileCenterPos(
		pokemonStatus.curTile.x, pokemonStatus.curTile.y);
	Vector2 targetPos = tileCenter + Vector2(
		0 * transform->GetScale().x,
		-10.0f * transform->GetScale().y);

	transform->SetWorldPosition(targetPos);

	// a star init
	aStar = new AStar;
	aStar->SetTileMap(this->tile);

	// ��Ÿ init
	isHurt = false;

	deltaTime = 0;

	hp = new ProgressBar;
	hp->SetCamera(camera);
	hp->Init(L"ui_hp_" + team, L"ui_bar");

	hp->SetScale(transform->GetScale());
	hp->SetPosition(transform->GetWorldPosition()
		+ Vector2(0, 65 * transform->GetScale().y));
}

void Pokemon::Init(wstring name, wstring team, POINT startPos, Vector2 pivot)
{
	InitShader();
	InitVertex(pivot);
	InitBuffer();

	transform->UpdateTransform();

	Vector2 center = Vector2(0, 0);
	Vector2 halfSize = center - (vertice[1].position - Vector2(1, 1));

	this->collider = new RectCollider;
	this->collider->SetBound(&center, &halfSize);

	targetTransform = new Transform;
	tempTransform = new Transform;

	/// json���κ��� �о�� �����ͷ� pokemonInfo Init �� frame Init

	// pokemonInfo Init
	pokemonStatus = GAME->FindPokemonInfo(name);

	// animationClip Init
	AnimationData  data;
	for (int i = 0; i < STATE_END; i++) {
		clips[i] = new AnimationClip;
		for (int j = 0; j < pokemonStatus.frameCnt[i]; j++) {
			if (pokemonStatus.frameCnt[i] == 32)
				data.time = 0.15f;

			data.keyName = to_wstring(i) + L"_" + to_wstring(j);
			data.maxFrame = Vector2(pokemonStatus.frameCnt[i], 1.0f);
			data.currentFrame = Vector2(j, 0.0f);
			clips[i]->PushAnimationData(data);
		}
	}

	// Get Texture
	for (int i = 0; i < STATE_END; i++) {
		wstring str = pokemonStatus.name;

		pokemonStatus.pPortraitTex = TEXTURE->GetTexture(
			str + L"_portrait");

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
		pokemonStatus.pTex[i] = TEXTURE->GetTexture(str);
	}

	pokemonStatus.curTile = startPos;
	pokemonStatus.targetTile = { -1, -1 };

	UpdateCurTileBlocked();

	CalculateAttackRange();

	// ��ġ init
	Vector2 tileCenter = tile->GetTileCenterPos(
		pokemonStatus.curTile.x, pokemonStatus.curTile.y);
	Vector2 targetPos = tileCenter + Vector2(
		0 * transform->GetScale().x,
		-10.0f * transform->GetScale().y);

	transform->SetWorldPosition(targetPos);

	// a star init
	aStar = new AStar;
	aStar->SetTileMap(this->tile);

	// ��Ÿ init
	isHurt = false;

	deltaTime = 0.0f;
	delayTime = 0.0f;

	// hp init
	hp = new ProgressBar;
	hp->SetCamera(camera);
	hp->Init(L"ui_hp_" + team, L"ui_bar");

	hp->SetScale(transform->GetScale());
	hp->SetPosition(transform->GetWorldPosition()
		+ Vector2(0, 65 * transform->GetScale().y));
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
	SAFE_DELETE(tempTransform);

	SAFE_DELETE(aStar);

	SAFE_RELEASE(hp);
	SAFE_DELETE(hp);
}

void Pokemon::Update()
{
	if (pokemonStatus.isDied) return;

	delayTime += FRAME->GetElapsedTime();

	//this->transform->DefaultControl2();
	
	clips[pokemonStatus.state]->Update(pokemonStatus.dir);

	// ���� Ž��������� ��ġ �������ְ� ���� 
	// pokemon update���� �ʴ� ��쿡 ���ؼ� ó���ϱ� ���ؼ�
	hp->SetPosition(transform->GetWorldPosition()
		+ Vector2(0, 65 * transform->GetScale().y));
	//hp->SetScale(transform->GetScale());

	hp->Update();

	switch (pokemonStatus.state)
	{
	case STATE_IDLE:
		if (isHurt)
			isHurt = false;

		Move();
		Attack();
		break;
	case STATE_MOVE:
		Move();
		//Attack();

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
		
		//// �����ϸ� ���� Ÿ�Ϸ� ����
		//if(MovePosition(pokemonInfo.targetTile))
		//	pokemonInfo.curTile = pokemonInfo.targetTile;
		//
		//if (pokemonInfo.curTile.x == pokemonInfo.targetTile.x
		//	&& pokemonInfo.curTile.y == pokemonInfo.targetTile.y) {
		//	pokemonInfo.state = STATE_IDLE;
		//	clips[pokemonInfo.state]->Play(pokemonInfo.dir);
		//}

		if (aStar->GetTargetTile() != NULL) {
			//deltaTime = 0;

			pokemonStatus.targetTile = aStar->GetPosition();
			POKEMON_DIRECTION dir = this->FindDirection(
				pokemonStatus.curTile, pokemonStatus.targetTile);
			if (dir != pokemonStatus.dir) {
				pokemonStatus.dir = dir;
				clips[pokemonStatus.state]->Play(pokemonStatus.dir);
			}
			if (!Attack()) {
				if (MovePosition(pokemonStatus.targetTile)) {
				
					UpdateCurTileBlocked(false);
					pokemonStatus.curTile = pokemonStatus.targetTile;
					UpdateCurTileBlocked();
					aStar->UpdateTargetTile();
				}
			}
		}
		else {
			pokemonStatus.state = STATE_IDLE;
			pokemonStatus.curTile = pokemonStatus.targetTile;
			clips[pokemonStatus.state]->Play(pokemonStatus.dir);
		}

		break;
	case STATE_HURT:
		break;
	case STATE_ATTACK:
		if (enemy != NULL) {
			enemy->SetHurt(true);
			if (delayTime > pokemonStatus.attackSpeed) {
				delayTime = 0.0f;
				enemy->SetHp(pokemonStatus.attack);
				enemy->ChangeHpBar();
				enemy->SetHurt(false);
				pokemonStatus.state = STATE_IDLE;
				clips[pokemonStatus.state]->Play(pokemonStatus.dir);
			}
		}
		else
		{
			pokemonStatus.state = STATE_IDLE;
			clips[pokemonStatus.state]->Play(pokemonStatus.dir);
		}

		break;
	case STATE_SPECIAL_ATTACK:
		break;
	}

	if (INPUT->GetKeyDown('M'))
		isHurt = !isHurt;
}

void Pokemon::Render()
{
	if (pokemonStatus.isDied) return;

	// ���İ� ����
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetVector("maxFrame",
		&D3DXVECTOR4(
			clips[pokemonStatus.state]->GetCurrentData().maxFrame.x,
			clips[pokemonStatus.state]->GetCurrentData().maxFrame.y,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			clips[pokemonStatus.state]->GetCurrentData().currentFrame.x,
			clips[pokemonStatus.state]->GetCurrentData().currentFrame.y,
			0.0f, 0.0f));

	this->pEffect->SetBool("mixedColor", isHurt);

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
		pokemonStatus.pTex[pokemonStatus.state]);

	this->pEffect->SetTechnique("MyShader");

	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	//transform->RenderGizmo(true);
	//collider->RenderGizmo(transform);
	//aStar->DrawPath();
	hp->Render();
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

	vertice[0].color = 0xffff0000;
	vertice[1].color = 0xffff0000;
	vertice[2].color = 0xffff0000;
	vertice[3].color = 0xffff0000;

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
	Vector2 tileCenter = tile->GetTileCenterPos(
		targetTile.x, targetTile.y);
	Vector2 targetPos = tileCenter + Vector2(
		0 * transform->GetScale().x,
		-10.0f * transform->GetScale().y);
	targetTransform->SetWorldPosition(targetPos);

	// �������� �̵�
	transform->PositionLerp(
		transform, targetTransform,
		FRAME->GetElapsedTime() * pokemonStatus.moveSpeed);

	// �׳� �̵�
	//float speed = pokemonInfo.moveSpeed 
	//	* FRAME->GetElapsedTime();
	//switch (pokemonInfo.dir)
	//{
	//case DIRECTION_BOTTOM:
	//	transform->MovePositionSelf(Vector2(0, speed));
	//	break;
	//case DIRECTION_TOP:
	//	transform->MovePositionSelf(Vector2(0, -speed));
	//	break;
	//case DIRECTION_LEFT:
	//	transform->MovePositionSelf(Vector2(-speed, 0));
	//	break;
	//case DIRECTION_RIGHT:
	//	transform->MovePositionSelf(Vector2(speed, 0));
	//	break;
	//case DIRECTION_LEFTBOTTOM:
	//	transform->MovePositionSelf(Vector2(-speed, speed));
	//	break;
	//case DIRECTION_RIGHTBOTTOM:
	//	transform->MovePositionSelf(Vector2(speed, speed));
	//	break;
	//case DIRECTION_LEFTTOP:
	//	transform->MovePositionSelf(Vector2(-speed, -speed));
	//	break;
	//case DIRECTION_RIGHTTOP:
	//	transform->MovePositionSelf(Vector2(speed, -speed));
	//	break;
	//}

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
		tempTransform->SetWorldPosition(
			transform->GetWorldPosition());
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

bool Pokemon::IsAttack()
{
	if (enemy == NULL || enemy->GetIsDied())
		return false;

	return (transform->GetWorldPosition()
		- enemy->GetTransform()->GetWorldPosition()).Length()
		<= pokemonStatus.attackRange;
}

void Pokemon::Move()
{
	//// ���콺 Ŭ������ �̵�
	//if (INPUT->GetKeyDown(VK_LBUTTON)) {
	//
	//	Vector2 mousePos;
	//	Util::GetMousePosWithScreen(&mousePos);
	//	Vector2 tilePos = tile->GetTransform()->GetWorldPosition();
	//	Vector2 tileScale = tile->GetTransform()->GetScale();
	//
	//	tilePos.x -= this->camera->GetWorldPosition().x;
	//	tilePos.y -= this->camera->GetWorldPosition().y;
	//
	//	// ���콺�� Ÿ�� ���� ���� ��
	//	if (mousePos.x >=
	//		tilePos.x - (TILE_COL * TILE_HEIGHT / 2) * tileScale.x &&
	//		mousePos.x <=
	//		tilePos.x + (TILE_COL * TILE_HEIGHT / 2) * tileScale.x &&
	//		mousePos.y >=
	//		tilePos.y - (TILE_ROW * TILE_WIDTH / 2) * tileScale.y &&
	//		mousePos.y <=
	//		tilePos.y + (TILE_ROW * TILE_WIDTH / 2) * tileScale.y) {
	//
	//		POINT targetTile;
	//		targetTile.x = (mousePos.y - tilePos.y +
	//			(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
	//			(TILE_WIDTH * tileScale.y);
	//		targetTile.y = (mousePos.x - tilePos.x +
	//			(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
	//			(TILE_HEIGHT * tileScale.x);
	//
	//		if (targetTile.x != pokemonStatus.curTile.x ||
	//			targetTile.y != pokemonStatus.curTile.y) {
	//			if (pokemonStatus.targetTile.x != targetTile.x
	//				|| pokemonStatus.targetTile.y != targetTile.y) {
	//				POINT currentTile;
	//				Vector2 pos = transform->GetWorldPosition();
	//				pos.x -= this->camera->GetWorldPosition().x;
	//				pos.y -= this->camera->GetWorldPosition().y;
	//
	//				currentTile.x = (pos.y - tilePos.y +
	//					(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
	//					(TILE_WIDTH * tileScale.y);
	//				currentTile.y = (pos.x - tilePos.x +
	//					(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
	//					(TILE_HEIGHT * tileScale.x);
	//				pokemonStatus.curTile = currentTile;
	//			}
	//			pokemonStatus.targetTile = targetTile;
	//		
	//			aStar->PathInit(
	//				pokemonStatus.curTile.x,
	//				pokemonStatus.curTile.y,
	//				pokemonStatus.targetTile.x,
	//				pokemonStatus.targetTile.y);
	//
	//			aStar->PathFind();
	//
	//			bool stateChange = false;
	//			bool dirChange = false;
	//
	//			if (pokemonStatus.state != STATE_MOVE) {
	//				pokemonStatus.state = STATE_MOVE;
	//				stateChange = true;
	//			}
	//
	//			POKEMON_DIRECTION dir = FindDirection(
	//				pokemonStatus.curTile, pokemonStatus.targetTile);
	//			if (pokemonStatus.dir != dir) {
	//				pokemonStatus.dir = dir;
	//				dirChange = true;
	//			}
	//			
	//			if(stateChange || dirChange)
	//				clips[pokemonStatus.state]->Play(pokemonStatus.dir);
	//
	//			tempTransform->SetScale(transform->GetScale());
	//			tempTransform->SetWorldPosition(
	//				transform->GetWorldPosition());
	//
	//			targetTransform->SetScale(transform->GetScale());
	//			Vector2 tileCenter = tile->GetTileCenterPos(
	//				targetTile.x, targetTile.y);
	//			Vector2 targetPos = tileCenter + Vector2(
	//				0 * transform->GetScale().x,
	//				-10.0f * transform->GetScale().y);
	//			targetTransform->SetWorldPosition(targetPos);
	//		}
	//	}
	//}

	// ���̻� ���� ������ Move ����
	if (enemy == NULL) return;

	// ��� ���ϸ����� �̵�
	deltaTime += FRAME->GetElapsedTime();
	if (deltaTime > 1.0f && !IsAttack()) {
		deltaTime = 0;

		POINT targetTile;
		targetTile = enemy->GetPokemonStatus().curTile;

		Vector2 tilePos = tile->GetTransform()->GetWorldPosition();
		Vector2 tileScale = tile->GetTransform()->GetScale();

		tilePos.x -= this->camera->GetWorldPosition().x;
		tilePos.y -= this->camera->GetWorldPosition().y;

		if (targetTile.x != pokemonStatus.curTile.x ||
			targetTile.y != pokemonStatus.curTile.y) {
			if (pokemonStatus.targetTile.x != targetTile.x
				|| pokemonStatus.targetTile.y != targetTile.y) {
				POINT currentTile;
				Vector2 pos = transform->GetWorldPosition();
				pos.x -= this->camera->GetWorldPosition().x;
				pos.y -= this->camera->GetWorldPosition().y;

				currentTile.x = (pos.y - tilePos.y +
					(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
					(TILE_WIDTH * tileScale.y);
				currentTile.y = (pos.x - tilePos.x +
					(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
					(TILE_HEIGHT * tileScale.x);

				UpdateCurTileBlocked(false);
				pokemonStatus.curTile = currentTile;
				UpdateCurTileBlocked();
			}
			pokemonStatus.targetTile = targetTile;

			UpdateCurTileBlocked(false);
			aStar->PathInit(
				pokemonStatus.curTile.x,
				pokemonStatus.curTile.y,
				pokemonStatus.targetTile.x,
				pokemonStatus.targetTile.y);
			UpdateCurTileBlocked();

			aStar->PathFind();

			bool stateChange = false;
			bool dirChange = false;

			if (pokemonStatus.state != STATE_MOVE) {
				pokemonStatus.state = STATE_MOVE;
				stateChange = true;
			}

			POKEMON_DIRECTION dir = FindDirection(
				pokemonStatus.curTile, aStar->GetPosition());
			if (pokemonStatus.dir != dir) {
				pokemonStatus.dir = dir;
				dirChange = true;
			}

			if (stateChange || dirChange)
				clips[pokemonStatus.state]->Play(pokemonStatus.dir);

			tempTransform->SetScale(transform->GetScale());
			tempTransform->SetWorldPosition(
				transform->GetWorldPosition());

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

bool Pokemon::Attack()
{
	if (IsAttack() && delayTime > pokemonStatus.attackSpeed) {
		
		aStar->SetTargetTile(NULL);

		delayTime = 0;

		bool stateChange = false;
		bool dirChange = false;

		if (pokemonStatus.state != STATE_ATTACK) {
			pokemonStatus.state = STATE_ATTACK;
			stateChange = true;
		}

		POKEMON_DIRECTION dir = FindDirection(
			pokemonStatus.curTile, enemy->GetPokemonStatus().curTile);
		if (pokemonStatus.dir != dir) {
			pokemonStatus.dir = dir;
			dirChange = true;
		}

		if (stateChange || dirChange)
			clips[pokemonStatus.state]->Play(pokemonStatus.dir);

		return true;
	}

	return false;
}

void Pokemon::SetHp(float damage)
{
	pokemonStatus.curHp -= damage - pokemonStatus.defense;
	if (pokemonStatus.curHp <= 0) {
		pokemonStatus.curHp = 0;
		pokemonStatus.isDied = true;
		UpdateCurTileBlocked(false);
	}
}

void Pokemon::DrawAttackRange()
{
	Vector2 centerPos;
	//centerPos = tile->GetTileCenterPos(
	//	pokemonInfo.curTile.x, pokemonInfo.curTile.y);
	centerPos = transform->GetWorldPosition();

	GIZMO->Circle(centerPos, pokemonStatus.attackRange,
		0xff0000ff);
	
}

void Pokemon::CalculateAttackRange()
{
	// ���ݹ����� 1 Ÿ���� ���
	if (pokemonStatus.attackRangeForTile == 1) {
		pokemonStatus.attackRange =
			(tile->GetTileCenterPos(0, 0)
				- tile->GetTileCenterPos(1, 1)).Length();
	}
}

void Pokemon::ChangeHpBar()
{
	hp->SetFrontScale(pokemonStatus.curHp / pokemonStatus.maxHp);
}

void Pokemon::UpdateCurTileBlocked(bool isBlocked)
{
	POINT pos = this->pokemonStatus.curTile;
	tagTile tileInfo = tile->GetTileInfo(pos.x, pos.y);
	tileInfo.block = isBlocked;
	tile->SetTileInfo(pos.x, pos.y, tileInfo);
}

