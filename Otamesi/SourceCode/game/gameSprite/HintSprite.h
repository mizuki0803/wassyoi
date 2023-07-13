#pragma once
#include "DrawerSprite.h"

/// <summary>
/// ヒントスプライト
/// </summary>
class HintSprite : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="parent">引き出し親スプライト</param>
	/// <param name="position">座標</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <returns>ヒントスプライト</returns>
	static HintSprite* Create(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="parent">引き出し親スプライト</param>
	/// <param name="position">座標</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 大きさ変更開始
	/// </summary>
	void SizeChangeStart();

	//getter
	DrawerSprite* GetParentStorage() { return parentStorage; }
	bool GetIsSizeLarger() { return isSizeLarger; }
	bool GetIsSizeChange() { return isSizeChange; }

public: //メンバ関数
	/// <summary>
	/// 大きさ変更
	/// </summary>
	void SizeChange();

private: //メンバ変数
	//親子構造を解除したりするので、保存用引き出し親オブジェクト
	DrawerSprite* parentStorage;
	//保存用座標
	Vector2 positionStorage;
	//保存用アンカーポイント
	Vector2 anchorpointStorage;
	//拡大表示中か
	bool isSizeLarger = false;
	//大きさ変更中か
	bool isSizeChange = false;
	//アクションタイマー
	int32_t actionTimer = 0;
	//大きさ変更前座標
	Vector2 sizeChangeBeforePos;
	//大きさ変更後座標
	Vector2 sizeChangeAfterPos;
};
