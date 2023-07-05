#pragma once
#include "Menu.h"
#include "DrawerSprite.h"
#include <array>

class UserInterface
{
public: //enum
	/// <summary>
	/// 引き出しスプライトの役割名
	/// </summary>
	enum DrawerSpriteName
	{
		HowToPlayMenu,		//メニュー
		Hint1,				//ヒント1
		Hint2,				//ヒント2
		HowToPlayPlayer,	//プレイヤー操作
		HowToPlayCamera,	//カメラ操作

		DrawerSpriteNum,	//引き出しスプライト数
	};


public: //メンバ関数
	/// <summary>
	/// 生成
	/// </summary>
	static std::unique_ptr<UserInterface> Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 奥の描画
	/// </summary>
	void Draw();

	// メニュー関係の処理
	void MenuUpdate();

	/// <summary>
	/// 引き出しスプライトの開閉状態をリセット
	/// </summary>
	void DrawerSpriteReset();

	//フラグの設定、取得
	void SetMenuFlag(bool flag) { menuFlag_ = flag; drawerSprites[HowToPlayMenu]->MoveStart(); }
	void SetNotMove(bool flag) { notMove_ = flag; }

	bool GetMenuFlag() { return menuFlag_; }

private: //メンバ関数
	/// <summary>
	/// キー入力による引き出しスプライト移動開始
	/// </summary>
	void DrawerSpriteMoveStartKey();

private: //メンバ変数
	//説明用引き出しスプライト
	std::array<std::unique_ptr<DrawerSprite>, DrawerSpriteNum> drawerSprites;
	// イージング進行
	float easeTimer_ = 0.0f;
	// メニュー用の背景
	std::unique_ptr<Sprite> menuBackScreen_;
	// メニュー用の枠組み
	std::array<std::unique_ptr<Menu>, 3> menuframe_;
	// 行動フラグ
	bool menuFlag_ = false;
	//
	bool notMove_ = false;
};

