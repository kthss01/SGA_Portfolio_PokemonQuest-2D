#include "stdafx.h"
#include "SceneManager.h"
#include "Scene\Scene.h"

SceneManager* SceneManager::instance = NULL;

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	this->Release();
}

Scene* SceneManager::_currentScene = NULL;

SceneManager * SceneManager::Get()
{
	if (instance == NULL)
		instance = new SceneManager;
	return instance;
}

void SceneManager::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

void SceneManager::Init()
{
	return;
}

void SceneManager::Release()
{
	map<string, Scene*>::iterator iter;
	iter = _mSceneList.begin();
	for (; iter != _mSceneList.end();) {
		// ����
		if (iter->second != NULL) {
			if (iter->second == _currentScene) iter->second->Release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else {
			++iter;
		}
	}
	_mSceneList.clear();
}

void SceneManager::Update()
{
	if (_currentScene) _currentScene->Update();
}

void SceneManager::Render()
{
	if (_currentScene) _currentScene->Render();
}

Scene * SceneManager::AddScene(string sceneName, Scene * scene)
{
	// ���� ���� ���
	if (!scene) return NULL;
	// ������ �̸� �����ϴ� ���
	if (_mSceneList.count(sceneName) > 0) return NULL;

	_mSceneList[sceneName] = scene;

	return _mSceneList[sceneName];
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	// �� ���� ���� Ȯ��
	if (_mSceneList.count(sceneName) < 1) return E_FAIL;
	if (_mSceneList[sceneName] == _currentScene) return E_FAIL;

	// ������� ������ �� ����
	_mSceneList[sceneName]->Init();

	// ���� ���� ���� �ϸ� ���� �� ������
	if (_currentScene) _currentScene->Release();
	_currentScene = _mSceneList[sceneName];

	return S_OK;
}
