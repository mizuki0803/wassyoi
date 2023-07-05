#pragma once
#include "Menu.h"
#include "DrawerSprite.h"
#include <array>
#include <vector>
#include <functional>

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

	/// <summary>
	/// メニューの全体
	/// </summary>
	enum class MenuPhase
	{
		Start,		//開始
		Selection,	//選択
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
	// 
	void MenuReset();
	//
	void MenuOpen();
	//
	void MenuSelection();

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
	// 行動をさせない
	bool notMove_ = false;
	// メニュー選択
	int selectionNumber_ = 0;
	// 選択可能か
	bool selectionFlag_ = false;
	// 関数の管理
	std::vector<std::function<void()>> menuFunc_;
	// 関数の番号
	size_t menuPhase_ = static_cast<int>(MenuPhase::Start);
	// 音の大きさ
	float soundVolume_ = 0.1f;
};

