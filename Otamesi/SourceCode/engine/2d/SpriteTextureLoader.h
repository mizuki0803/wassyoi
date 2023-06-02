#pragma once
#include "Texture.h"

/// <summary>
/// スプライトテクスチャ読み込みクラス
/// </summary>
class SpriteTextureLoader
{
public: //テクスチャ名
	enum TextureName {
		DebugFont,		//デバッグ用フォント
		White,			//真っ白
		Block,			//ブロック
		MenuBackScreen, //メニュー画面用の背景
		MenuFrame,		//メニューの枠組み
		ParanomaSky,	// スカイドーム用

		SpriteTextureNum,	//スプライトテクスチャの枚数
	};


public: //静的メンバ関数
	/// <summary>
	/// テクスチャ全読み込み
	/// </summary>
	static void TextureLoad();

	//getter
	static Texture GetTexture(const TextureName textureName) { return textures[textureName]; };

private: //静的メンバ変数
	//テクスチャ
	static Texture textures[SpriteTextureNum];
};