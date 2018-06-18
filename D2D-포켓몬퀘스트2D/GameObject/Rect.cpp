#include "stdafx.h"
#include "Rect.h"
#include "Common\Camera.h"
#include "./Animation/AnimationClip.h"

Rect::Rect() {

}

Rect::~Rect() {

}

void Rect::Init(wstring shaderFile, const Vector2 uv, const Vector2 pivot)
{
	InitShader(shaderFile);

	InitVertex(uv, pivot);

	InitBuffer();

	transform->UpdateTransform();

	Vector2 center = Vector2(0, 0);
	Vector2 halfSize = center - (vertice[1].position - pivot);

	this->collider = new RectCollider;
	this->collider->SetBound(&center, &halfSize);

	pTex = NULL;

	deltaTime = 0.0f;

	AnimationData data;
	clips = new AnimationClip;

	for (int i = 0; i < 4; i++) {
		data.keyName = L"run_" + to_wstring(i);
		data.maxFrame = Vector2(8.0f, 4.0f);
		data.currentFrame = Vector2(float(i), 0.0f);
		clips->PushAnimationData(data);

	}

	for (int i = 0; i < TEMPSIZE; i++) {
		tempTrans[i] = new Transform;
	}

	tempTrans[0]->SetWorldPosition(Vector2(200, 100));

	tempCollider = new RectCollider;
	tempCollider->SetBound(&center, &halfSize);
}

void Rect::Release()
{
	SAFE_RELEASE(ib);
	SAFE_RELEASE(vb);
	SAFE_RELEASE(pEffect);
	//SAFE_RELEASE(pTex);

	SAFE_DELETE(clips);

	for (int i = 0; i < TEMPSIZE; i++)
		SAFE_DELETE(tempTrans[i]);

	SAFE_DELETE(collider);
	SAFE_DELETE(tempCollider);
}

void Rect::Update()
{
	this->transform->DefaultControl2();
	this->DrawInterface();
	clips->Update(AniRepeatType_Loop);

	tempTrans[0]->Interpolate(this->tempTrans[0],
		transform, Frame::Get()->GetFrameDeltaSec() * 10.0f);
	for (int i = 1; i < TEMPSIZE; i++) {
		tempTrans[i]->Interpolate(
			this->tempTrans[i],
			tempTrans[i - 1],
			// 이 값이 1에 가까우면 붙어서 움직임
			Frame::Get()->GetFrameDeltaSec() * 10.0f);
	}

	Vector2 mouse;
	Util::GetMousePos(&mouse);

	check = Collision::IntersectTri(
		vertice[0].position,
		vertice[1].position,
		vertice[2].position,
		this->transform,
		mouse
	);

	colliderCheck = Collision::IsOverlap(
		this->transform,
		this->collider,
		this->tempTrans[0],
		this->tempCollider
	);
}

void Rect::Render()
{
	//D2D::GetDevice()->SetStreamSource(0, vb, 0, stride);
	//D2D::GetDevice()->SetIndices(ib);
	//D2D::GetDevice()->SetTransform(D3DTS_WORLD, &transform->GetFinalMatrix().ToDXMatrix());
	//D2D::GetDevice()->SetFVF(FVF);
	//D2D::GetDevice()->SetTexture(0, pTex);
	//D2D::GetDevice()->DrawIndexedPrimitive(
	//	D3DPT_TRIANGLELIST,
	//	0, 0, 4, 0, 2);

	// 알파값 제거
	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	D2D::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	D2D::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	this->pEffect->SetMatrix("matView", &camera->GetViewMatrix().ToDXMatrix());
	this->pEffect->SetMatrix("matProjection", &camera->GetProjection().ToDXMatrix());

	this->pEffect->SetVector("maxFrame",
		&D3DXVECTOR4(
			clips->GetCurrentData().maxFrame.x,
			clips->GetCurrentData().maxFrame.y,
			0.0f, 0.0f));
	this->pEffect->SetVector("currentFrame",
		&D3DXVECTOR4(
			clips->GetCurrentData().currentFrame.x,
			clips->GetCurrentData().currentFrame.y,
			0.0f, 0.0f));

	this->pEffect->SetTexture("tex", pTex);

	this->pEffect->SetTechnique("MyShader");
	
	this->pEffect->SetMatrix("matWorld", &transform->GetFinalMatrix().ToDXMatrix());
	this->RenderRect();

	for (int i = 0; i < TEMPSIZE; i++) {
		this->pEffect->SetMatrix("matWorld", &tempTrans[i]->GetFinalMatrix().ToDXMatrix());
		this->RenderRect();
	}

	D2D::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

	transform->RenderGizmo(true);
	collider->RenderGizmo(transform);
}

void Rect::RenderRect()
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

void Rect::InitShader(wstring shaderFile)
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

void Rect::InitVertex(Vector2 uv, Vector2 pivot)
{
	vertice[0].position = Vector2(-50 + pivot.x, 50 + pivot.y);
	vertice[1].position = Vector2(-50 + pivot.x, -50 + pivot.y);
	vertice[2].position = Vector2(50 + pivot.x, -50 + pivot.y);
	vertice[3].position = Vector2(50 + pivot.x, 50 + pivot.y);

	vertice[0].color = 0xffff0000;
	vertice[1].color = 0xffffff00;
	vertice[2].color = 0xff00ff00;
	vertice[3].color = 0xff0000ff;

	vertice[0].uv = Vector2(0, uv.y);
	vertice[1].uv = Vector2(0, 0);
	vertice[2].uv = Vector2(uv.x, 0);
	vertice[3].uv = uv;

	stride = sizeof(Vertex);
	//FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
}

void Rect::InitBuffer()
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

void Rect::DrawInterface()
{
#ifdef IMGUI_USE
	ImGui::Begin("Interface");
	{
		if (check) {
			ImGui::Text("Mouse Check True");
		}
		else {
			ImGui::Text("Mouse Check False");
		}

		if (colliderCheck) {
			ImGui::Text("Collider Check True");
		}
		else {
			ImGui::Text("Collider Check False");
		}

		transform->DrawInterface();
	}
	ImGui::End();
#endif // IMGUI_USE
}