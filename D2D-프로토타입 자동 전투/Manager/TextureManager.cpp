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
	//	D3DX_DEFAULT_NONPOW2, // �̹��� ���� ũ��
	//	D3DX_DEFAULT_NONPOW2, // �̹��� ���� ũ�� �ڵ����� �̹��� ũ�� ���� �ҷ���)
	//	1, // Miplevels // 3D �󿡼� Ȯ�� ��ҽ� ���, ������ ���� ����
	//	0, // Usage // ��� �뵵 ��� �뵵�δ� 0���� ����
	//	D3DFMT_A8R8G8B8, // �̹��� ���� ���� // �� 8bit ��� �̹���
	//	D3DPOOL_MANAGED, // pool // �̹��� ���� ���̷�Ʈ�� ���� ����
	//	D3DX_FILTER_NONE, // Filter // Ȯ�� ��ҽ� ����ϴ� ����
	//	D3DX_FILTER_NONE, // Filter // Ȯ�� ��ҽ� ����ϴ� ����
	//	NULL, // �÷�Ű ����
	//	&info, // �ҷ��� �̹����� ������ ������ ����ü
	//	NULL, // �ȷ�Ʈ ���� // 8bit�� ��� ����ǥ
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
