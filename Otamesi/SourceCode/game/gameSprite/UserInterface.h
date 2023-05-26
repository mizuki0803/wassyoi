#pragma once
#include "Menu.h"
#include <array>

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
	bool GetMenuFlag() { return menuFlag_; }

private: //メンバ変数
	// イージング進行
	float easeTimer_ = 0.0f;
	// メニュー用の背景
	std::unique_ptr<Sprite> menuBackScreen_;
	// メニュー用の枠組み
	std::array<std::unique_ptr<Menu>, 3> menuframe_;
	// 行動フラグ
	bool menuFlag_ = false;
};

