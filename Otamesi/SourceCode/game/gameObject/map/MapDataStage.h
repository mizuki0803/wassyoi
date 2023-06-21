#pragma once
#include "MapData.h"
#include "StageBlock.h"

/// <summary>
/// ステージ用マップデータ
/// </summary>
class MapDataStage : public MapData
{
public:
	/// <summary>
	/// ゲームフェーズ
	/// </summary>
	enum class GamePhase
	{
		GamePlay,	//ゲーム
		Start,		//開始
		ReStart,	//再開始
	};

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

	/// <summary>
	/// ゲーム中
	/// </summary>
	void PlayGame();
	/// <summary>
	/// ゲーム開始
	/// </summary>
	void GameStart();
	/// <summary>
	/// ゲーム再開始
	/// </summary>
	void GameReStart();
	/// <summary>
	/// 関数の設定
	/// </summary>
	void CreateAct();

	/// <summary>
	/// マップブロック再生成
	/// </summary>
	void ReCreateMapBlock(const int selectStageNum);

	//getter
	const XMINT3& GetPlayerCreateMapChipNum() { return playerCreateMapChipNum; }
	const bool GetIsReCreateEnd() { return isReCreateEnd; }

	//setter
	void SetIsReCreateEnd(const bool isReCreateEnd) { this->isReCreateEnd = isReCreateEnd; }

protected: //メンバ関数
	/// <summary>
	/// マップブロック生成
	/// </summary>
	void CreateMapBlock() override;

	/// <summary>
	/// ブロックを上げる番号の生成
	/// </summary>
	void BlockCountCreate();

	bool BlockCount();

protected: //メンバ変数
	//ステージ用ブロック
	std::vector<std::unique_ptr<StageBlock>> blocks;

	//プレイヤーを生成するマップ番号
	XMINT3 playerCreateMapChipNum;
	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_ = static_cast<int>(GamePhase::Start);
	// 上げるブロックの番号の保存
	std::vector<int> rndcount;
	// ブロックを上げるタイマー
	int blockActTimer_ = 100;
	//マップ再生成が終了したか
	bool isReCreateEnd = false;
};