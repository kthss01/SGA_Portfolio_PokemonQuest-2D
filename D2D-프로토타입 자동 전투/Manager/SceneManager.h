#pragma once

class Scene;
class SceneManager
{
private:
	typedef map<string, Scene*> mSceneList;
private:
	static Scene* _currentScene; // ÇöÀç ÇÃ·¹ÀÌ ¾À
	mSceneList _mSceneList; // ¾À ¸ñ·Ï
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
