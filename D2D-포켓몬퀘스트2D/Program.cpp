#include "stdafx.h"
#include "Program.h"

#include "Scene\TestScene.h"

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
		SOUND->AddSound("Test", "sounds/����3.wav", true, true);
	}
	// TEXTURE Setting
	{
		TEXTURE->AddTexture(L"Mario", L"Textures/mario_all.png");
		TEXTURE->AddTexture(L"Snow", L"./Textures/Snow.png");
	}

	// SPRITE Setting
	{
		D3DXCreateSprite(D2D::GetDevice(), &sprite);
	}

	// SCENE Setting
	{
		TestScene * test = new TestScene;
		SCENE->AddScene("Test", test);
	}
	
	// ��Ÿ Setting
	{
		//mainCamera = new Camera;
		//rect = new Rect;
		//rect->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1));
		//rect->SetTexture(TEXTURE->GetTexture(L"Mario"));
		//rect->SetCamera(mainCamera);
		
		//SOUND->Play("Test");
		SCENE->ChangeScene("Test");

		isDebug = false;
	}
	// JSON Setting
	{
		root = new Json::Value();
		readJson = new Json::Value();
		float time = FRAME->GetNowRealTimeSec();
		Json::SetValue(*root, "test", time);

		WriteJsonData(L"Test.Json", root);
		ReadJsonData(L"Test.Json", readJson);

		float temp;
		Json::GetValue(*readJson, "test", temp);
	}

	// Particle Setting
	{
		particle = new ParticleEmitter;

		VEC_COLOR colors;
		colors.push_back(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		colors.push_back(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		colors.push_back(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

		VEC_SCALE scales;
		scales.push_back(1.0f);
		scales.push_back(5.0f);

		particle->Init(
			100, 10.0f, 5.0f, 6.0f,
			Vector2(0, 5), Vector2(1, 6),
			Vector2(0, 5), Vector2(1, 10),
			30, 50, 0.5f, 0.8f, 1, 1,
			colors, scales, 10.0f, 20.0f,
			TEXTURE->GetTexture(L"Snow"), false);

		particle->StartEmission();
	}
}

Program::~Program()
{
	//rect->Release();
	//SAFE_DELETE(rect);
	//SAFE_DELETE(mainCamera);
	
	TEXTURE->Release();
	SCENE->Release();

	SAFE_DELETE(readJson);
	SAFE_DELETE(root);

	SAFE_DELETE(particle);
}

void Program::Update()
{
	//mainCamera->UpdateCamToDevice();
	//rect->Update();

	SCENE->Update();

	particle->Update();
	particle->GetTransform()->DefaultControl2();

	if (INPUT->GetKeyDown(VK_F11))
		isDebug = !isDebug;
}

void Program::Render()
{
	//rect->Render();
	SCENE->Render();

	//sprite->Begin(D3DXSPRITE_ALPHABLEND);
	//{
	//	RECT rc = { 0, 0, 100, 100 };
	//	sprite->Draw(TEXTURE->GetTexture(L"Mario"),
	//		&rc, // ����ϰ� ���� ũ�� �� ����)
	//		&D3DXVECTOR3(100 / 2, 100 / 2, 0), // ȸ���� �Ͼ�� �߽���
	//		&D3DXVECTOR3(WINSIZE_X / 2, WINSIZE_Y / 2, 0), // �׷����� ��ġ
	//		0xffffffff);// �÷���
	//}
	//sprite->End();

	//Vector2 center = Vector2(0, 0);

	//GIZMO->Circle(
	//	center, 100.0f, 0xFF808000
	//);

	particle->Render();

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