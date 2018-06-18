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
		// 삭제
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
	// 씬이 없는 경우
	if (!scene) return NULL;
	// 동일한 이름 존재하는 경우
	if (_mSceneList.count(sceneName) > 0) return NULL;

	_mSceneList[sceneName] = scene;

	return _mSceneList[sceneName];
}

HRESULT SceneManager::ChangeScene(string sceneName)
{
	// 씬 존재 여부 확인
	if (_mSceneList.count(sceneName) < 1) return E_FAIL;
	if (_mSceneList[sceneName] == _currentScene) return E_FAIL;

	// 여기까지 왔으면 씬 변경
	_mSceneList[sceneName]->Init();

	// 기존 씬이 존재 하면 기존 씬 릴리즈
	if (_currentScene) _currentScene->Release();
	_currentScene = _mSceneList[sceneName];

	return S_OK;
}
