#pragma once
#include "ObjObject3d.h"
#include "EaseData.h"

/// <summary>
/// マップ用ブロック
/// </summary>
class Block : public ObjObject3d
{
public: //enum
	/// <summary>
	/// ゲームの全体
	/// </summary>
	enum class GamePhase
	{
		Strat,	//開始
		None,	//何もしない
	};

	/// <summary>
	/// ブロックの種類
	/// </summary>
	enum class BlockType
	{
		Block,
		Plane,
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="model">モデル</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <returns>ブロック</returns>
	static Block* Create(ObjModel* model, const XMINT3& mapChipNum);

	//getter
	static float GetBlockSize() { return Block::blockSize; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* model, const XMINT3& mapChipNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// ゲーム開始時
	/// </summary>
	void PlayStratMove();

	/// <summary>
	/// イージングが終わったか
	/// </summary>
	/// <returns>終了フラグ</returns>
	bool IsEaseEndFlag() { return easeData_->GetEndFlag(); }
	/// <summary>
	/// イージングデータの設定
	/// </summary>
	/// <param name="count">フレームのカウント</param>
	void SetEaseData(const int count);
	/// <summary>
	/// イージングを開始する時の位置の保存
	/// </summary>
	/// <param name="position"></param>
	void SetBlockStratPos(const Vector3& position) { blockStratPos_ = position; }
	/// <summary>
	/// イージングを終了する時の位置の保存
	/// </summary>
	/// <param name="position"></param>
	void SetBlockEndPos(const Vector3& position) { blockEndPos_ = position; }
	/// <summary>
	/// 関数の設定
	/// </summary>
	void CreateAct();
	/// <summary>
	/// どの行動をしているかの取得
	/// </summary>
	/// <returns></returns>
	int GetActPhase() { return static_cast<int>(phase_); }

protected: //静的メンバ変数
	//ブロック一つの大きさ
	static const float blockSize;

protected: // メンバ変数
	//ブロックの種類
	BlockType blockType;
	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_ = static_cast<int>(GamePhase::Strat);

#pragma region
	// 開始位置
	Vector3 blockStratPos_;
	// 終了位置
	Vector3 blockEndPos_;
	// イージングデータ
	std::unique_ptr<EaseData> easeData_;
#pragma endregion イージング関係
};