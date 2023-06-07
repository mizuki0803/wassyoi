#pragma once
#include "Sprite.h"
#include <memory>
#include <vector>

/// <summary>
/// ステージクリアUI
/// </summary>
class StageClearUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージクリアUI</returns>
	static StageClearUI* Create();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	void CreateNewSprite(const Texture& texture, const Vector2& position, const Vector2& texSize, const float sizeRatio);

private: //メンバ変数
	//操作方法スプライト
	std::vector<std::unique_ptr<Sprite>> sprites;
};