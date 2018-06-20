#pragma once

class Scene {
private:

public:
	Scene();
	~Scene();

	virtual void Init();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};