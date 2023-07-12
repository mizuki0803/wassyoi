#pragma once
#include "Sprite.h"

/// <summary>
/// 引き出し移動スプライト
/// </summary>
class DrawerSprite : public Sprite
{
public: //enum
	//引き出しで隠れる場所
	enum HidePlace
	{
		Left,
		Right,
		Up,
		Down,
	};


public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="drawerKey">引き出しの開閉に使用するキー</param>
	/// <param name="hidePlace">引き出しで隠れる場所</param>
	/// <param name="posY">Y座標</param>
	/// <param name="stickoutNum">閉じている場合に画面内にはみ出す量</param>
	/// <param name="isOpenDrawer">引き出しを開いている状態か</param>
	/// <returns>引き出し移動スプライト</returns>
	static DrawerSprite* Create(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer = false);


public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="drawerKey">引き出しの開閉に使用するキー</param>
	/// <param name="hidePlace">引き出しで隠れる場所</param>
	/// <param name="posY">Y座標</param>
	/// <param name="stickoutNum">閉じている場合に画面内にはみ出す量</param>
	/// <param name="isOpenDrawer">引き出しを開いている状態か</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 開閉移動開始
	/// </summary>
	void MoveStart();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	//gettre
	BYTE GetDrawerKey() { return drawerKey; }

private: //メンバ関数
	/// <summary>
	/// 開閉移動
	/// </summary>
	void Move();

private: //静的メンバ変数
	//開閉移動にかかる時間
	static const int moveTime = 30;

private: //メンバ変数
	//引き出しを開いている状態か
	bool isOpenDrawer = false;
	//シーン開始時の開閉状態保存用
	bool isOpenDrawerSceneStart;
	//引き出しの開閉に使用するキー
	BYTE drawerKey;
	//開いている場合の座標
	Vector2 openStatePos;
	//閉じている場合の座標
	Vector2 closedStatePos;
	//閉じている場合に画面内にはみ出す量
	float stickoutNum;
	//開閉移動開始時座標
	Vector2 moveStartPos;
	//開閉移動用タイマー
	int32_t moveTimer = 0;
	//開閉移動中か
	bool isMoveDrawer = false;
};