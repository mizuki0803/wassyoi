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
		Number,			//数字
		TitleLogo,		//タイトルロゴ
		Block,			//ブロック
		MenuBackScreen, //メニュー画面用の背景
		MenuFrame,		//メニューの枠組み
		SoundVolumeBar,	//音量変更用バー
		SoundVolumePointer,	//音量変更用ポインター
		HowToPlayMove,	//操作方法(移動)
		HowToPlayCamera,//操作方法(カメラ)
		HowToPlayChengeDemension,//操作方法(次元変更)
		HowToPlayUndo,	//操作方法(戻る)
		HowToPlayRedo,	//操作方法(戻る戻る)
		HowToPlayReset,	//操作方法(リセット)
		HowToPlayStageSelect,	//操作方法(ステージセレクト)
		StageClear,		//ステージクリア
		ClearBack,		//ステージクリアの背景
		PressSpace,		//PRESS SPACE
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