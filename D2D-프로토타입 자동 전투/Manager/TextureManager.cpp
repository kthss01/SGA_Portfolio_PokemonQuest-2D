#include "stdafx.h"
#include "TextureManager.h"

TextureManager* TextureManager::instance = NULL;

TextureManager::TextureManager()
{
}


TextureManager::~TextureManager()
{
}

TextureManager * TextureManager::Get()
{
	if (instance == NULL)
		instance = new TextureManager;

	return instance;
}

void TextureManager::Delete()
{
	if (instance != NULL)
		SAFE_DELETE(instance);
}

void TextureManager::AddTexture(wstring keyName, wstring fileName)
{
	LPDIRECT3DTEXTURE9 tex = GetTexture(keyName);

	if (tex) return;

	HRESULT hr = D3DXCreateTextureFromFile(
		DEVICE,
		fileName.c_str(),
		&tex
	);
	assert(SUCCEEDED(hr));

	//D3DXIMAGE_INFO info;
	//HRESULT hr = D3DXCreateTextureFromFileEx(
	//	DEVICE, fileName.c_str(),
	//	D3DX_DEFAULT_NONPOW2, // 이미지 가로 크기
	//	D3DX_DEFAULT_NONPOW2, // 이미지 세로 크기 자동으로 이미지 크기 맞춰 불러옴)
	//	1, // Miplevels // 3D 상에서 확대 축소시 사용, 깨지는 현상 방지
	//	0, // Usage // 사용 용도 출력 용도로는 0으로 지정
	//	D3DFMT_A8R8G8B8, // 이미지 색상 포멧 // 각 8bit 사용 이미지
	//	D3DPOOL_MANAGED, // pool // 이미지 관리 다이렉트가 직접 관리
	//	D3DX_FILTER_NONE, // Filter // 확대 축소시 사용하는 필터
	//	D3DX_FILTER_NONE, // Filter // 확대 축소시 사용하는 필터
	//	NULL, // 컬러키 설정
	//	&info, // 불러온 이미지의 정보를 저장할 구조체
	//	NULL, // 팔레트 설정 // 8bit인 경우 색상표
	//	&tex);
	//assert(SUCCEEDED(hr));

	textures[keyName] = tex;
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture(wstring keyName)
{
	if (textures.count(keyName) < 1) return NULL;

	return textures[keyName];
}

void TextureManager::Release()
{
	map<wstring, LPDIRECT3DTEXTURE9>::iterator iter = textures.begin();

	for (; iter != textures.end();) {
		if (iter->second != NULL) {
			SAFE_RELEASE(iter->second);
			iter = textures.erase(iter);
		}
		else
			iter++;
	}

	textures.clear();
}
