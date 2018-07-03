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
			data.keyName = to_wstring(i) + L"_" + to_wstring(j);
			data.maxFrame = Vector2(frameCnt[i], 1.0f);
			data.currentFrame = Vector2(j, 0.0f);
			clips[i]->PushAnimationData(data);
		}
	}

	targetTransform = new Transform;
	tempTransform = new Transform;

	// pokemonInfo init
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
	pokemonInfo.attackSpeed = 0.3f;

	CaculateAttackRange();

	pokemonInfo.curTile = startPos;
	pokemonInfo.targetTile = { -1, -1 };

	pokemonInfo.attack = 5;
	pokemonInfo.defense = 0;
	pokemonInfo.maxHp = 25;
	pokemonInfo.curHp = 25;

	// 위치 init
	Vector2 tileCenter = tile->GetTileCenterPos(
		pokemonInfo.curTile.x, pokemonInfo.curTile.y);
	Vector2 targetPos = tileCenter + Vector2(
		0 * transform->GetScale().x,
		-10.0f * transform->GetScale().y);

	transform->SetWorldPosition(targetPos);

	// a star init
	aStar = new AStar;
	aStar->SetTileMap(this->tile);

	// 기타 init
	isHurt = false;

	deltaTime = 0;

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
	if (pokemonInfo.isDied) return;

	delayTime += FRAME->GetElapsedTime();

	//this->transform->DefaultControl2();
	
	bool aniEnd = clips[pokemonInfo.state]->Update(pokemonInfo.dir);

	// 참고 탐험씬에서도 위치 조정해주고 있음 
	// pokemon update하지 않는 경우에 대해서 처리하기 위해서
	hp->SetPosition(transform->GetWorldPosition()
		+ Vector2(0, 65 * transform->GetScale().y));
	//hp->SetScale(transform->GetScale());

	hp->Update();

	switch (pokemonInfo.state)
	{
	case STATE_IDLE:
		if (isHurt)
			isHurt = false;

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
		
		//// 도착하면 현재 타일로 변경
		//if(MovePosition(pokemonInfo.targetTile))
		//	pokemonInfo.curTile = pokemonInfo.targetTile;
		//
		//if (pokemonInfo.curTile.x == pokemonInfo.targetTile.x
		//	&& pokemonInfo.curTile.y == pokemonInfo.targetTile.y) {
		//	pokemonInfo.state = STATE_IDLE;
		//	clips[pokemonInfo.state]->Play(pokemonInfo.dir);
		//}

		if (aStar->GetTargetTile() != NULL) {
			deltaTime = 0;

			pokemonInfo.targetTile = aStar->GetPosition();
			POKEMON_DIRECTION dir = this->FindDirection(
				pokemonInfo.curTile, pokemonInfo.targetTile);
			if (dir != pokemonInfo.dir) {
				pokemonInfo.dir = dir;
				clips[pokemonInfo.state]->Play(pokemonInfo.dir);
			}

			if (MovePosition(pokemonInfo.targetTile)) {
				pokemonInfo.curTile = pokemonInfo.targetTile;
				aStar->UpdateTargetTile();
			}
		}
		else {
			pokemonInfo.state = STATE_IDLE;
			pokemonInfo.curTile = pokemonInfo.targetTile;
			clips[pokemonInfo.state]->Play(pokemonInfo.dir);
		}

		break;
	case STATE_HURT:
		break;
	case STATE_ATTACK:
		enemy->SetHurt(true);
		if (delayTime > pokemonInfo.attackSpeed) {
			delayTime = 0.0f;
			enemy->SetHp(pokemonInfo.attack);
			enemy->ChangeHpBar();
			enemy->SetHurt(false);
			pokemonInfo.state = STATE_IDLE;
			clips[pokemonInfo.state]->Play(pokemonInfo.dir);
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
	if (pokemonInfo.isDied) return;

	// 알파값 제거
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

	this->pEffect->SetBool("isHurt", isHurt);

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

	//transform->RenderGizmo(true);
	//collider->RenderGizmo(transform);
	//aStar->DrawPath();
	hp->Render();
}

void Pokemon::RenderRect()
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
				// 만약에 텍스처 렌더하는 방식이면 pEffect->setTexture로
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
		L"./Shader/ColorTexture.fx",
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

	// 보간으로 이동
	transform->PositionLerp(
		transform, targetTransform,
		FRAME->GetElapsedTime() * pokemonInfo.moveSpeed);

	// 그냥 이동
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
	// 타겟에 일정거리내로 도착하면 true 아니면 false
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
	if (enemy->GetIsDied())
		return false;

	return (transform->GetWorldPosition()
		- enemy->GetTransform()->GetWorldPosition()).Length()
		<= pokemonInfo.attackRange;
}

void Pokemon::Move()
{
	//// 마우스 클릭으로 이동
	if (INPUT->GetKeyDown(VK_LBUTTON)) {
	
		Vector2 mousePos;
		Util::GetMousePosWithScreen(&mousePos);
		Vector2 tilePos = tile->GetTransform()->GetWorldPosition();
		Vector2 tileScale = tile->GetTransform()->GetScale();
	
		tilePos.x -= this->camera->GetWorldPosition().x;
		tilePos.y -= this->camera->GetWorldPosition().y;
	
		// 마우스가 타일 위에 있을 때
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
					pos.x -= this->camera->GetWorldPosition().x;
					pos.y -= this->camera->GetWorldPosition().y;
	
					currentTile.x = (pos.y - tilePos.y +
						(TILE_ROW * TILE_WIDTH / 2) * tileScale.y) /
						(TILE_WIDTH * tileScale.y);
					currentTile.y = (pos.x - tilePos.x +
						(TILE_COL * TILE_HEIGHT / 2) * tileScale.x) /
						(TILE_HEIGHT * tileScale.x);
					pokemonInfo.curTile = currentTile;
				}
				pokemonInfo.targetTile = targetTile;
			
				aStar->PathInit(
					pokemonInfo.curTile.x,
					pokemonInfo.curTile.y,
					pokemonInfo.targetTile.x,
					pokemonInfo.targetTile.y);
	
				aStar->PathFind();
	
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

	// 상대 포켓몬으로 이동
	deltaTime += FRAME->GetElapsedTime();
	if (deltaTime > 1.0f && !IsAttack()) {
		deltaTime = 0;

		POINT targetTile;
		targetTile = enemy->GetPokemonInfo().curTile;

		Vector2 tilePos = tile->GetTransform()->GetWorldPosition();
		Vector2 tileScale = tile->GetTransform()->GetScale();

		tilePos.x -= this->camera->GetWorldPosition().x;
		tilePos.y -= this->camera->GetWorldPosition().y;

		if (targetTile.x != pokemonInfo.curTile.x ||
			targetTile.y != pokemonInfo.curTile.y) {
			if (pokemonInfo.targetTile.x != targetTile.x
				|| pokemonInfo.targetTile.y != targetTile.y) {
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
				pokemonInfo.curTile = currentTile;
			}
			pokemonInfo.targetTile = targetTile;

			aStar->PathInit(
				pokemonInfo.curTile.x,
				pokemonInfo.curTile.y,
				pokemonInfo.targetTile.x,
				pokemonInfo.targetTile.y);

			aStar->PathFind();

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

			if (stateChange || dirChange)
				clips[pokemonInfo.state]->Play(pokemonInfo.dir);

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

void Pokemon::Attack()
{
	if (IsAttack() && delayTime > pokemonInfo.attackSpeed) {
		
		aStar->SetTargetTile(NULL);

		delayTime = 0;

		bool stateChange = false;
		bool dirChange = false;

		if (pokemonInfo.state != STATE_ATTACK) {
			pokemonInfo.state = STATE_ATTACK;
			stateChange = true;
		}

		POKEMON_DIRECTION dir = FindDirection(
			pokemonInfo.curTile, enemy->GetPokemonInfo().curTile);
		if (pokemonInfo.dir != dir) {
			pokemonInfo.dir = dir;
			dirChange = true;
		}

		if (stateChange || dirChange)
			clips[pokemonInfo.state]->Play(pokemonInfo.dir);
	}
}

void Pokemon::SetHp(float damage)
{
	pokemonInfo.curHp -= damage - pokemonInfo.defense;
	if (pokemonInfo.curHp <= 0) {
		pokemonInfo.curHp = 0;
		pokemonInfo.isDied = true;
	}
}

void Pokemon::DrawAttackRange()
{
	Vector2 centerPos;
	//centerPos = tile->GetTileCenterPos(
	//	pokemonInfo.curTile.x, pokemonInfo.curTile.y);
	centerPos = transform->GetWorldPosition();

	GIZMO->Circle(centerPos, pokemonInfo.attackRange,
		0xff0000ff);
	
}

void Pokemon::CaculateAttackRange()
{
	pokemonInfo.attackRange =
		(tile->GetTileCenterPos(0, 0)
			- tile->GetTileCenterPos(1, 1)).Length();
}

void Pokemon::ChangeHpBar()
{
	hp->SetFrontScale(pokemonInfo.curHp / pokemonInfo.maxHp);
}
