#pragma once
#include "MapData.h"
#include "StageSelectBlock.h"
#include "StageSelectBlockManager.h"

/// <summary>
/// ステージ選択用マップデータ
/// </summary>
class MapDataStageSelect : public MapData
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="selectStageNum">選択中のステージ番号</param>
	/// <param name="positionPhase">ブロック管理の場所フェーズ</param>
	/// <returns>ステージ選択用マップデータ</returns>
	static MapDataStageSelect* Create(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="selectStageNum">選択中のステージ番号</param>
	/// <param name="positionPhase">ブロック管理の場所フェーズ</param>
	/// <returns>成否</returns>
	bool Initialize(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	//getter
	StageSelectBlockManager* GetStageSelectBlockManager() { return blockManager.get(); }

private: //メンバ関数
	/// <summary>
	/// マップブロック生成
	/// </summary>
	void CreateMapBlock() override;

private: //メンバ変数
	//プレイヤーモデル
	std::unique_ptr<ObjModel> modelPlayer;

	//ステージ選択用ブロック
	std::vector<std::unique_ptr<StageSelectBlock>> blocks;
	//ステージ選択用ブロック管理
	std::unique_ptr<StageSelectBlockManager> blockManager;
};