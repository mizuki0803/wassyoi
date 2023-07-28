﻿#pragma once
#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// テクスチャ
/// </summary>
struct Texture
{
private: //エイリアス
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ変数
	//テクスチャリソース(テクスチャバッファ)
	ComPtr<ID3D12Resource> texBuff;
	//テクスチャ番号
	UINT texNumber = 0;
};