#pragma once
#include "ObjObject3d.h"
#include <array>

/// <summary>
/// ステージ選択用ブロック管理オブジェクト
/// </summary>
class StageSelectBlockManager : public ObjObject3d
{
public: //enum
	/// <summary>
	/// ブロック管理の場所フェーズ
	/// </summary>
	enum BlockManagerPositionPhase
	{
		Center,	//中心
		OutscreenRight, //画面外(右)
		OutscreenLeft, //画面外(左)

		PositionNum,	//座標の数
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="positionPhase">ブロック管理の場所フェーズ</param>
	/// <returns>ステージ選択用ブロック管理オブジェクト</returns>
	static StageSelectBlockManager* Create(const BlockManagerPositionPhase& positionPhase);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="positionPhase">ブロック管理の場所フェーズ</param>
	/// <returns>成否</returns>
	bool Initialize(const BlockManagerPositionPhase& positionPhase);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 移動開始
	/// </summary>
	/// <param name="nextPositionPhase">ブロック管理の場所フェーズ</param>
	void MoveStart(const BlockManagerPositionPhase& nextPositionPhase);

	//getter
	bool GetIsDelete() { return isDelete; }
	bool GetIsMove() { return isMove; }

private: //メンバ関数
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

private: //静的メンバ変数
	//ブロック管理の場所
	static std::array<Vector3, PositionNum> managerPos;
	//一度の移動にかかる時間
	static const int32_t moveTime = 50;

private: //メンバ変数
	//場所フェーズ
	BlockManagerPositionPhase blockManagerPositionPhase;
	//次に移動する場所フェーズ
	BlockManagerPositionPhase nextBlockManagerPositionPhase;
	//削除フラグ
	bool isDelete = false;
	//移動中か
	bool isMove = false;
	//移動用タイマー
	int32_t moveTimer;
};