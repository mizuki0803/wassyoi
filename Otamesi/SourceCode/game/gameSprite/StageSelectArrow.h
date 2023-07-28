#pragma once
#include "Sprite.h"
#include <array>
#include <memory>

/// <summary>
/// ステージ選択シーン用の左右矢印
/// </summary>
class StageSelectArrow
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージ選択シーン用の左右矢印</returns>
	static StageSelectArrow* Create();

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

	/// <summary>
	/// 選択中のステージで矢印スプライトを描画するか判定
	/// </summary>
	void SelectStageCheckDraw();

private: //メンバ変数
	//矢印スプライト
	std::array<std::unique_ptr<Sprite>, 2> arrows;
	//描画フラグ
	std::array<bool, 2> isDraws;
};
