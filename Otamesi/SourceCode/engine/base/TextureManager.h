#pragma once
#include "Texture.h"
#include <string>

class TextureManager
{
public: //静的メンバ関数	
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	/// <param name="texture">読み込んだテクスチャを保存する場所</param> 
	/// <param name="directoryPath">パス</param>
	/// <param name="filename">ファイル名</param>
	static void LoadTexture(Texture& texture, const std::string& directoryPath, const std::string& filename);

	//setter
	static void SetDevice(ID3D12Device* device) { TextureManager::dev = device; };

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
};