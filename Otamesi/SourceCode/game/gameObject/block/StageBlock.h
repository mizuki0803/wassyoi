#pragma once
#include "Block.h"
#include "EaseData.h"
#include <memory>

/// <summary>
/// ステージ用ブロック
/// </summary>
class StageBlock : public Block
{
public: //enum
	/// <summary>
	/// ゲームの全体
	/// </summary>
	enum class GamePhase
	{
		Start,	//開始
		Move,	//移動
		Delete,	//削除
		ReStart,//再度生成
		None,	//何もしない
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="_blockType">モデル番号</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <param name="shiftPos">マップを中心にずらす値</param>
	/// <returns>ステージ用ブロック</returns>
	static StageBlock* Create(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_blockType">モデル番号</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <param name="shiftPos">マップを中心にずらす値</param>
	/// <returns>成否</returns>
	bool Initialize(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ゲーム開始時
	/// </summary>
	void PlayStratMove();
	/// <summary>
	/// 再生成時の移動
	/// </summary>
	void ReStratMove();
	/// <summary>
	/// 削除時の移動
	/// </summary>
	void DeleteMove();
	/// <summary>
	/// 再生成時の追加したブロックの移動
	/// </summary>
	void ReCreateMove();

	/// <summary>
	/// 再生成の設定
	/// </summary>
	/// <param name="phase">行動タイプ</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	void ReCreate(const GamePhase phase, const XMINT3& mapChipNum);

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

	/// <summary>
	/// 削除フラグの取得
	/// </summary>
	/// <returns>削除フラグ</returns>
	bool GetDeleteFlag() { return deleteFlag_; }

	/// <summary>
	/// 行動タイプの設定
	/// </summary>
	/// <param name="phase">行動タイプ</param>
	void SetGamePhase(GamePhase phase) { phase_ = static_cast<int>(phase); }

	/// <summary>
	/// プレイヤーからのコールバック
	/// </summary>
	void ReAction();

	/// <summary>
	/// ランダムの計算
	/// </summary>
	/// <param name="a">最小値</param>
	/// <param name="b">最大値</param>
	const float RandCalculate(float a, float b);

protected: // メンバ変数
	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_;
	//削除用のフラグ
	bool deleteFlag_ = false;

#pragma region
	// 開始位置
	Vector3 blockStratPos_;
	// 終了位置
	Vector3 blockEndPos_;
	// 移動量
	Vector3 vec_ = {};
	// イージングデータ
	std::unique_ptr<EaseData> easeData_;
	// 移動量用イージングデータ
	std::unique_ptr<EaseData> vecEaseData_;
#pragma endregion イージング関係
};