#include "stdafx.h"
#include "MainScene.h"

#include "Common\Camera.h"
#include "Common\Transform.h"

#include "GameObject\Rect.h"

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::Init()
{
	mainCamera = new Camera;
	mainCamera->UpdateCamToDevice();

	bg = new Rect;
	bg->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	bg->SetCamera(mainCamera);

	bg->SetTexture(TEXTURE->GetTexture(L"ui_bg"));
	bg->GetTransform()->SetScale(Vector2(10.3f, 7.7f));

	for (int i = 0; i < 2; i++) {
		btn[i] = new Rect;
		btn[i]->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
		btn[i]->SetCamera(mainCamera);
	}

	//btn[0]->SetTexture(TEXTURE->GetTexture(L"ui_poketball_1"));
	//btn[1]->SetTexture(TEXTURE->GetTexture(L"ui_poketball_14"));
	btn[0]->SetTexture(TEXTURE->GetTexture(L"ui_start"));
	btn[1]->SetTexture(TEXTURE->GetTexture(L"ui_tool"));

	btn[0]->GetTransform()->SetScale(Vector2(0.9f, 0.9f));
	btn[0]->GetTransform()->SetWorldPosition(Vector2(332, -313));
	btn[1]->GetTransform()->SetScale(Vector2(0.9f, 0.9f));
	btn[1]->GetTransform()->SetWorldPosition(Vector2(451, -313));
}

void MainScene::Release()
{
	SAFE_RELEASE(bg);
	SAFE_DELETE(bg);

	for (int i = 0; i < 2; i++) {
		SAFE_RELEASE(btn[i]);
		SAFE_DELETE(btn[i]);
	}

	SAFE_DELETE(mainCamera);
}

void MainScene::Update()
{
	mainCamera->UpdateCamToDevice();

	//bg->Update();
	//btn[0]->Update();
	//btn[1]->Update();

	// ui position test
	//{
	//	//Transform* trnasform = bg->GetTransform();
	//	Transform* trnasform = btn[0]->GetTransform();
	//	//Transform* trnasform = btn[1]->GetTransform();
	//	int temp = 0;
	//}

	if (INPUT->GetKeyDown(VK_LBUTTON)) {
		if (btn[0]->IsMouseCollision()) {
			SCENE->ChangeScene("Explore");
			return;
		}
		if (btn[1]->IsMouseCollision()) {
			SCENE->ChangeScene("MapTool");
			return;
		}
	}
}

void MainScene::Render()
{
	bg->Render();
	
	for (int i = 0; i < 2; i++) {
		btn[i]->Render();
	}
}
