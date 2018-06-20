#include "stdafx.h"
#include "TestScene.h"

#include "GameObject\Rect.h"
#include "./Common/Camera.h"

TestScene::TestScene() {

}

TestScene::~TestScene() {

}

void TestScene::Init() {
	mainCamera = new Camera;
	rect = new Rect;
	rect->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
	rect->SetTexture(TEXTURE->GetTexture(L"Mario"));
	rect->SetCamera(mainCamera);
}

void TestScene::Release() {
	rect->Release();
	SAFE_DELETE(rect);
	SAFE_DELETE(mainCamera);
}

void TestScene::Update() {
	mainCamera->UpdateCamToDevice();
	rect->Update();
}

void TestScene::Render() {
	rect->Render();
}
