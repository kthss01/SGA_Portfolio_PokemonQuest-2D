#pragma once

#include "Scene/Scene.h"

class MainScene : public Scene
{
private:
	class Camera* mainCamera;

	class Rect* bg;
	class Rect* btn[2];

	int sceneSwitch;
	float deltaTime;
public:
	MainScene();
	~MainScene();

	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};