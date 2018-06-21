#include "stdafx.h"
#include "Program.h"

#include "Scene\TestScene.h"
#include "Scene\ExploreScene\ExploreScene.h"

#include "GameObject\Rect.h"
#include "./Common/Camera.h"
#include "./Common/ParticleEmitter.h"

Program::Program()
{
	srand(time(NULL));
	D2D::GetDevice()->SetRenderState(
		// ����Ʈ ����
		D3DRS_LIGHTING,
		// ��� ����
		FALSE
	);

	// SOUND Setting
	{
		//SOUND->AddSound("Test", "sounds/����3.wav", true, true);
	}
	// TEXTURE Setting
	{
		TEXTURE->AddTexture(L"tile_forest", L"Textures/tile_Forest.png");
	}

	// SPRITE Setting
	{
		D3DXCreateSprite(D2D::GetDevice(), &sprite);
	}

	// SCENE Setting
	{
		TestScene * test = new TestScene;
		SCENE->AddScene("Test", test);
		ExploreScene * explore = new ExploreScene;
		SCENE->AddScene("Explore", explore);
	}
	
	// ��Ÿ Setting
	{
		//SOUND->Play("Test");
		//SCENE->ChangeScene("Test");
		SCENE->ChangeScene("Explore");

		isDebug = false;
	}
	// JSON Setting
	{
		root = new Json::Value();
		readJson = new Json::Value();
		float time = FRAME->GetElapsedTime();
		Json::SetValue(*root, "test", time);

		WriteJsonData(L"Test.Json", root);
		ReadJsonData(L"Test.Json", readJson);

		float temp;
		Json::GetValue(*readJson, "test", temp);
	}
}

Program::~Program()
{
	TEXTURE->Release();
	SCENE->Release();

	SAFE_DELETE(readJson);
	SAFE_DELETE(root);

	SAFE_DELETE(particle);
}

void Program::Update()
{
	SCENE->Update();

	if (INPUT->GetKeyDown(VK_F11))
		isDebug = !isDebug;
}

void Program::Render()
{
	SCENE->Render();

	//Vector2 center = Vector2(0, 0);

	//GIZMO->Circle(
	//	center, 100.0f, 0xFF808000
	//);

	//particle->Render();

	FRAME->Render();

	if (isDebug) {

	}
}

void Program::WriteJsonData(wstring fileName, Json::Value * root)
{
	ofstream stream;
	string temp = String::WStringToString(fileName);
	stream.open(temp);
	{
		Json::StyledWriter writer;
		stream << writer.write(*root);
	}
	stream.close();
}

void Program::ReadJsonData(wstring fileName, Json::Value * root)
{
	ifstream stream;
	stream.open(fileName);
	{
		Json::Reader reader;
		reader.parse(stream, *root);
	}
	stream.close();
}