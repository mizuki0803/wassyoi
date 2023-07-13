#pragma once
#include "Sprite.h"
#include "EaseData.h"
#include <memory>

class Menu : public Sprite
{
public: //メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">位置</param>
	/// <returns></returns>
	static std::unique_ptr<Menu> Create(Vector2 pos);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="position">座標</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, const Vector2& position, const Vector2& anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	const bool EaseEnd() {return menuEaseData_->GetEndFlag(); }

private: //メンバ変数
	// イージング
	std::unique_ptr<EaseData> menuEaseData_;
};

