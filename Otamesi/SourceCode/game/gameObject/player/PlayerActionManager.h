#pragma once
#include "Player.h"
#include "GameCamera.h"
#include <vector>

/// <summary>
/// プレイヤー行動可能判定管理
/// </summary>
class PlayerActionManager
{
public: //静的メンバ関数
	//setter
	static void SetMapChipNum(const std::vector<std::vector<std::vector<int>>>& mapChipNum) { PlayerActionManager::mapChipNum = mapChipNum; }

	/// <summary>
	/// プレイヤーの3D移動可能判定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <param name="cameraXPosPhase">ステージから見ての横軸カメラ位置フェーズ</param>
	/// <param name="cameraYPosPhase">ステージから見ての縦軸カメラ位置フェーズ</param>
	/// <returns>成否</returns>
	static bool PlayerMoveCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase);

	/// <summary>
	/// プレイヤーの2D移動可能判定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <param name="cameraXPosPhase">ステージから見ての横軸カメラ位置フェーズ</param>
	/// <returns>成否</returns>
	static bool PlayerMoveCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase);

	/// <summary>
	/// 2D視点で一番手前にプレイヤーがいた場合、どのブロックよりも手前にいる状態にする
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	static void PlayerFrontmost2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 現在のカメラ方向で見た場合にプレイヤーの上方向にブロックが存在するか判定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <returns>成否</returns>
	static bool DirectionForwardBlockCheck(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 現在のカメラ方向で見た場合にプレイヤーの下方向にブロックが存在するか判定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <returns>成否</returns>
	static bool DirectionAwayBlockCheck(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 3次元に戻るときの足場となるブロックに接するマップ番号を設定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	static void PlayerScaffoldReturn3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 3次元状態のプレイヤーがゴールしたのかを判定
	/// </summary>
	///  <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <returns>成否</returns>
	static bool PlayerGoalCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 2次元状態のプレイヤーがゴールしたのかを判定
	/// </summary>
	///  <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <returns>成否</returns>
	static bool PlayerGoalCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

private: //静的メンバ関数
	/// <summary>
	/// キー入力に対するプレイヤー3D移動の方向を設定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <param name="cameraXPosPhase">ステージから見ての横軸カメラ位置フェーズ</param>
	/// <param name="cameraYPosPhase">ステージから見ての縦軸カメラ位置フェーズ</param>
	static void PlayerMoveDirection3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase);

	/// <summary>
	/// キー入力に対するプレイヤー2D移動の方向を設定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <param name="cameraXPosPhase">ステージから見ての横軸カメラ位置フェーズ</param>
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	static void PlayerMoveDirection2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase);

	/// <summary>
	/// 3D移動可能なブロック状況かを判定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param> 
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <returns>成否</returns>
	static bool PlayerMoveBlockCheck3D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 2D移動可能なブロック状況かを判定
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param> 
	/// <param name="moveSurfacePhase">ブロックのどの面移動するかフェーズ</param>
	/// <returns>成否</returns>
	static bool PlayerMoveBlockCheck2D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 3D移動のブロック間にハリボテが存在せず、移動可能かを判定
	/// </summary>
	/// <param name="mapChipNumberPlayerBeforePos">移動前プレイヤー位置を表すマップ番号</param>
	/// <param name="mapChipNumberPlayerAfterPos">移動後プレイヤー位置を表すマップ番号</param>
	/// <returns>成否</returns>
	static bool PlayerMovePlaneCheck3D(const XMINT3& mapChipNumberPlayerBeforePos, const XMINT3& mapChipNumberPlayerAfterPos);

	/// <summary>
	/// プレイヤーの位置を表すマップ番号が範囲内かをチェック
	/// </summary> 
	/// <param name="mapChipNumberPlayerPos">プレイヤー位置を表すマップ番号</param>
	/// <returns>成否</returns>
	static bool PlayerMoveMapChipNumWithinRangeCheck(const XMINT3& mapChipNumberPlayerPos);

private: //静的メンバ変数
	//3次元マップ用の3次元配列
	static std::vector<std::vector<std::vector<int>>> mapChipNum;
};