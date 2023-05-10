#pragma once
#include "ObjObject3d.h"
#include "GameCamera.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player : public ObjObject3d
{
public: //enum
	/// <summary>
	/// 行動フェーズ
	/// </summary>
	enum class ActionPhase
	{
		None,	//何もしない
		MovePos,	//座標移動
	};

	/// <summary>
	/// 上下左右前後、ブロックのどの面移動するかフェーズ
	/// </summary>
	enum class MoveSurfacePhase
	{
		Upward,			//上向き
		Downward,		//下向き
		FacingLeft,		//左向き
		FacingRight,	//右向き
		FacingForward,	//手前向き
		FacingAway,		//奥向き
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="mapChipNumberPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="gameCamera">ゲームカメラ</param>
	/// <returns>プレイヤー</returns>
	static Player* Create(ObjModel* model, const XMINT3& mapChipNumberPos, GameCamera* gameCamera);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//getter
	const bool GetIsGoal() { return isGoal; }

private: //メンバ関数
	/// <summary>
	/// 座標移動開始
	/// </summary>
	void MovePosStart();

	/// <summary>
	/// 座標移動
	/// </summary>
	void MovePos();

	/// <summary>
	/// 次元切り替え開始
	/// </summary>
	void ChanegeDimensionStart();

	/// <summary>
	/// ステージクリアのチェック
	/// </summary>
	void StageClearCheck();

	/// <summary>
	/// マップ番号の座標を取得
	/// </summary>
	Vector3 GetMapChipPos(const XMINT3& mapChipNumberPos);

private: //静的メンバ変数
	//プレイヤーの大きさ
	static const float playerSize;

private: //メンバ変数
	//プレイヤー位置を表すマップ番号(Z,Y,Xの順番になっている)
	XMINT3 mapChipNumberPos;
	//ゲームカメラ
	GameCamera* gameCamera = nullptr;
	//アクション
	ActionPhase actionPhase = ActionPhase::None;
	//アクション用タイマー
	int32_t actionTimer = 0;
	//ブロックのどの面移動するかフェーズ
	MoveSurfacePhase moveSurfacePhase = MoveSurfacePhase::Upward;
	//移動前座標
	Vector3 moveBeforePos;
	//移動目標座標
	Vector3 moveAfterPos;
	//ゴールしたか
	bool isGoal = false;
};