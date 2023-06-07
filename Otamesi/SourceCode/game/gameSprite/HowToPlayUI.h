#pragma once
#include "Sprite.h"
#include <memory>
#include <vector>

/// <summary>
/// 操作方法UI
/// </summary>
class HowToPlayUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="isDrawPause">操作方法「ポーズ」の描画をするか</param>
	/// <returns>操作方法UI</returns>
	static HowToPlayUI* Create(bool isDrawPause);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="isDrawPause">操作方法「ポーズ」の描画をするか</param>
	void Initialize(bool isDrawPause);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: //メンバ関数
	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="texture">使用するテクスチャ</param>
	/// <param name="position">座標</param>
	/// <param name="texSize">テクスチャサイズ</param>
	/// <param name="sizeRatio">テクスチャサイズに対するサイズの比率</param>
	void CreateNewSprite(const Texture& texture, Vector2& position, const Vector2& texSize, const float sizeRatio);

private: //メンバ変数
	//操作方法スプライト
	std::vector<std::unique_ptr<Sprite>> sprites;
};