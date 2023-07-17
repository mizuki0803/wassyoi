#pragma once
#include "Texture.h"
#include <array>

/// <summary>
/// ヒント画像読み込み
/// </summary>
class HintTextureLoader
{
public: //メンバ関数
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	static void LoadTextures(const int stageNum);

	//getter
	static Texture& GetTexture(const int num) { return textures[num]; }


private: //メンバ変数
	//テクスチャ
	static std::array<Texture, 2> textures;
};