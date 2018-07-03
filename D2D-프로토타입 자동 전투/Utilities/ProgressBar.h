#pragma once

class ProgressBar 
{
private:
	class Rect* _front;
	class Rect* _back;

	class Camera* camera;
public:

	HRESULT Init(const wchar_t* front, const wchar_t* back);
	void Release();
	void Update();
	void Render();

	void SetCamera(class Camera* camera) { this->camera = camera; }

	void SetPosition(Vector2 pos);
	void SetScale(Vector2 scale);
};