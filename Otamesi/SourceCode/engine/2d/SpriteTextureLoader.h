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
		MenuTitle,		//メニュー用テキスト（Title）
		MenuVolume,		//メニュー用テキスト（Volume）
		MenuStageSelect,//メニュー用テキスト（Select）
		MenuExit,		//メニュー用テキスト（Exit）
		SoundVolumeBar,	//音量変更用バー
		SoundVolumePointer,	//音量変更用ポインター
		MenuIconDef,	// メニューアイコン
		MenuIconArrow,	// メニューアイコン
		Husen,	//ノーマル付箋
		HusenL,	//左向き付箋
		HusenR,	//右向き付箋
		HusenH,	//ヒント付箋
		HowToPlaySystem,	//操作方法(システム)
		HowToPlayPlayer,	//操作方法(プレイヤー)
		HowToPlayCamera,//操作方法(カメラ)
		Hint1Text,//操作方法(ヒント1)
		Hint2Text,	//操作方法(ヒント2)
		Hint1,	//ヒント1
		Hint2,	//ヒント2
		ZoomIcon,	//ヒント拡大説明
		HowToPlayChengeDemension,//操作方法(次元変更)
		HowToPlayUndo,	//操作方法(Undo)
		HowToPlayRedo,	//操作方法(Redo)
		HowToPlayReset, //操作方法(reset)
		HowToPlayMenu,	//操作方法(メニュー)
		StageClear,		//ステージクリア
		ClearBack,		//ステージクリアの背景
		PressSpace,		//PRESS SPACE
		ParanomaSky,	// スカイドーム用
		StageSelectArrow,//ステージセレクト
		SpriteTextureNum,	//スプライトテクスチャの枚数
	};


public: //静的メンバ関数
	/// <summary>
	/// テクスチャ全読み込み
	/// </summary>
	static void TextureLoad();

	//getter
	static Texture& GetTexture(const TextureName textureName) { return textures[textureName]; };

private: //静的メンバ変数
	//テクスチャ
	static Texture textures[SpriteTextureNum];
};