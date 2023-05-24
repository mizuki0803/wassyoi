#pragma once
#include <array>

/// <summary>
/// ステージ管理
/// </summary>
class StageManager
{
public: //enum
	/// <summary>
	/// ステージ名
	/// </summary>
	enum StageName
	{
		Stage1,
		Stage2,
		Stage3,

		StageNum,	//ステージ数
	};

public: //静的メンバ関数
	/// <summary>
	/// 指定したステージがクリアしているか
	/// </summary>
	/// <param name="stageName">ステージ名</param>
	/// <returns>ステージがクリアしているか</returns>
	static bool GetIsClear(const StageName stageName);

	/// <summary>
	/// 指定したステージをクリア状態にする
	/// </summary>
	/// <param name="stageName">ステージ名</param>
	static void StageClear(const StageName stageName);

	//getter
	static StageName GetSelectStage() { return StageManager::selectStage; }

	//setter
	static void SetSelectStage(const StageName stageName) { StageManager::selectStage = selectStage; }

private: //静的メンバ変数
	//選択中のステージ名
	static StageName selectStage;
	//ステージクリアしているか
	static std::array<bool, StageNum> isClear;
};