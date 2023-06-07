#pragma once
#include "MapData.h"
#include "StageBlock.h"

/// <summary>
/// ステージ用マップデータ
/// </summary>
class MapDataStage : public MapData
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="selectStageNum">選択中のステージ番号</param>
	/// <returns>ステージ用マップデータ</returns>
	static MapDataStage* Create(const int selectStageNum);


public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	//getter
	const XMINT3& GetPlayerCreateMapChipNum() { return playerCreateMapChipNum; }

protected: //メンバ関数
	/// <summary>
	/// マップブロック生成
	/// </summary>
	void CreateMapBlock() override;

	/// <summary>
	/// マップブロック再生成
	/// </summary>
	void ReCreateMapBlock();

	/// <summary>
	/// ブロックを上げる番号の生成
	/// </summary>
	void BlockCountCreate();

protected: //メンバ変数
	//ステージ用ブロック
	std::vector<std::unique_ptr<StageBlock>> blocks;

	//プレイヤーを生成するマップ番号
	XMINT3 playerCreateMapChipNum;

	// 上げるブロックの番号の保存
	std::vector<int> rndcount;
	// ブロックを上げるタイマー
	int blockActTimer_ = 100;
	// ブロックを上げ終わったか
	bool isBolckUp_ = false;
};