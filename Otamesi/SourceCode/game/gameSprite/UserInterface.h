#pragma once
#include "Menu.h"
#include "DrawerSprite.h"
#include <array>
#include <vector>

class UserInterface
{
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

	//フラグの設定、取得
	void SetMenuFlag(bool flag) { menuFlag_ = flag; }
	void SetNotMove(bool flag) { notMove_ = flag; }

	bool GetMenuFlag() { return menuFlag_; }

private: //メンバ関数
	/// <summary>
	/// 引き出しスプライト生成
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="keyNumber">開閉キー割り当て</param>
	/// <param name="hidePlace">引き出しで隠れる場所</param>
	/// <param name="posY">Y座標</param>
	/// <param name="stickoutNum">閉じている場合に画面内にはみ出す量</param>
	/// <param name="isOpenDrawer">引き出しを開いている状態か</param>
	/// <returns>引き出し移動スプライト</returns>
	void CreateNewDrawerSprite(const Texture& texture, BYTE keyNumber, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer = false);

private: //メンバ変数
	//説明用引き出しスプライト
	std::vector<std::unique_ptr<DrawerSprite>> drawerSprites;
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

