#pragma once
#include "NumberSprite.h"
#include <array>
#include <memory>

/// <summary>
/// ステージ番号表示UI
/// </summary>
class StageNumberUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="stageNum">ステージ番号</param>
	/// <returns>ステージ番号表示UI</returns>
	static StageNumberUI* Create(const Vector2& position, const float size, const int stageNum);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="stageNum">ステージ番号</param>
	/// <returns>成否</returns>
	bool Initialize(const Vector2& position, const float size, const int stageNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ステージ番号の変更
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	void ChengeStageNum(const int stageNum);

private: //静的メンバ変数
	//桁数
	static const int digitNum = 2;

private: //メンバ変数
	//数字スプライト
	std::array<std::unique_ptr<NumberSprite>, digitNum> numbers;
};