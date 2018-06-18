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
