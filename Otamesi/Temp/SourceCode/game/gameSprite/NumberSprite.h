#pragma once
#include "Sprite.h"

/// <summary>
/// 数字スプライト
/// </summary>
class NumberSprite : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="texSize">テクスチャサイズ</param>
	/// <returns>数字スプライト</returns>
	static NumberSprite* Create(const Texture& texture, const Vector2& position, const Vector2& size, const Vector2& texSize);

public: //メンバ関数
	/// <summary>
	/// 表示する番号をセット
	/// </summary>
	/// <param name="number">表示する番号(0～9, 10は.)</param>
	void SetNumber(const int number);
};