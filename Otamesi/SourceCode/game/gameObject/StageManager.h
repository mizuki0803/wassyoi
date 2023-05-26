#pragma once
#include <array>

/// <summary>
/// ステージ管理
/// </summary>
class StageManager
{
public: //静的メンバ関数
	/// <summary>
	/// 指定したステージがクリアしているか
	/// </summary>
	/// <param name="stageName">ステージ名</param>
	/// <returns>ステージがクリアしているか</returns>
	static bool GetIsClear(const int selectStage);

	/// <summary>
	/// 現在選択中のステージをクリア状態にする
	/// </summary>
	static void StageClear();

	/// <summary>
	/// 現在選択中のステージのgetter
	/// </summary>
	/// <returns>現在選択中のステージ</returns>
	static int GetSelectStage() { return StageManager::selectStage; }

	/// <summary>
	/// 現在選択中のステージのsetter
	/// </summary>
	static void SetSelectStage(const int selectStage);

	/// <summary>
	/// 選択中のステージを次に移動させる
	/// </summary>
	static bool NextStageSelect();

	/// <summary>
	/// 選択中のステージを前に移動させる
	/// </summary>
	static bool PrevStageSelect();

private: //静的メンバ変数(定数)
	static const int stageNum = 6;

private: //静的メンバ変数
	//選択中のステージ
	static int selectStage;
	//ステージクリアしているか
	static std::array<bool, stageNum> isClear;
};