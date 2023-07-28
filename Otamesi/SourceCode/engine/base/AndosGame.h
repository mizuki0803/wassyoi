﻿#pragma once
#include "FrameWork.h"

/// <summary>
/// ゲーム固有のクラス
/// </summary>
class AndosGame : public FrameWork
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;
};