#include "stdafx.h"
#include "ProgressBar.h"

#include "Common\Camera.h"
#include "GameObject\Rect.h"

HRESULT ProgressBar::Init(const wchar_t* front, const wchar_t* back)
{
	_front = new Rect;
	_front->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1), 
		Vector2(40, 7), Vector2(40, 0));
	//_front->GetTransform()->MovePositionSelf(Vector2(-35, 0));
	_front->SetTexture(TEXTURE->GetTexture(front));
	_front->SetCamera(camera);

	_back = new Rect;
	_back->Init(L"./Shader/ColorTexture.fx", Vector2(1, 1), 
		Vector2(70, 10));
	_back->SetTexture(TEXTURE->GetTexture(back));
	_back->SetCamera(camera);

	return S_OK;
}

void ProgressBar::Release()
{
	_back->Release();
	_front->Release();

	SAFE_DELETE(_back);
	SAFE_DELETE(_front);
}

void ProgressBar::Update()
{
	_front->Update();
	//_back->Update();
}

void ProgressBar::Render()
{
	_back->Render();
	_front->Render();
}

void ProgressBar::SetPosition(Vector2 pos)
{
	//_front->GetTransform()->SetWorldPosition(pos);
	_back->GetTransform()->SetWorldPosition(pos);
	_front->GetTransform()->SetWorldPosition(
		_back->GetTransform()->GetWorldPosition() 
		- Vector2(36 * _back->GetTransform()->GetScale().x, 0));
}

void ProgressBar::SetScale(Vector2 scale)
{
	_front->GetTransform()->SetScale(Vector2(scale.x * 1.3f, scale.y));
	_back->GetTransform()->SetScale(scale);
}
