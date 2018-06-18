#pragma once

#include "Scene.h"

class TestScene : public Scene
{
private:
	class Camera* mainCamera;
	class Rect* rect;

public:
	TestScene();
	~TestScene();

	void Init() override;
	void Release() override;
	void Update() override;
	void Render() override;
};