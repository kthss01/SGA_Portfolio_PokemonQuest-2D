#pragma once

class Scene;
class SceneManager
{
private:
	typedef map<string, Scene*> mSceneList;
private:
	static Scene* _currentScene; // ���� �÷��� ��
	mSceneList _mSceneList; // �� ���
private:
	SceneManager();
	~SceneManager();

	static SceneManager* instance;
public:
	static SceneManager* Get();
	static void Delete();

	void Init();
	void Release();
	void Update();
	void Render();

	Scene* AddScene(string sceneName, Scene* scene);
	HRESULT ChangeScene(string sceneName);
};

#define SCENE SceneManager::Get()
