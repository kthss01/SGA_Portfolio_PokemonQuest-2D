#pragma once

class ProgressBar 
{
private:
	class Rect* _front;
	class Rect* _back;

	class Camera* camera;

	float width;
public:

	HRESULT Init(wstring front, wstring back);
	void Release();
	void Update();
	void Render();

	void SetCamera(class Camera* camera) { this->camera = camera; }

	void SetPosition(Vector2 pos);
	void SetScale(Vector2 scale);
	void SetFrontScale(float ratio);

	Rect* GetFront() { return _front; }
};