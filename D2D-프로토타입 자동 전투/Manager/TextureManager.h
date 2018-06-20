#pragma once
class TextureManager
{
private:
	TextureManager();
	~TextureManager();

	map<wstring, LPDIRECT3DTEXTURE9> textures;
	static TextureManager* instance;
public:
	static TextureManager * Get();
	static void Delete();

	void AddTexture(wstring keyName, wstring fileName);
	LPDIRECT3DTEXTURE9 GetTexture(wstring keyName);

	void Release();
};

#define TEXTURE TextureManager::Get()