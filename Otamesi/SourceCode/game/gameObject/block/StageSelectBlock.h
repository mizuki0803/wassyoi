#pragma once
#include "Block.h"

/// <summary>
/// ステージ選択用ブロック
/// </summary>
class StageSelectBlock : public Block
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="_blockType">モデル番号</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <param name="mapSize">マップの大きさ</param>
	/// <param name="parent">中心点用の親オブジェクト</param>
	/// <returns>ステージ選択用ブロック</returns>
	static StageSelectBlock* Create(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_blockType">モデル番号</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <param name="mapSize">マップの大きさ</param>
	/// <param name="parent">中心点用の親オブジェクト</param>
	/// <returns>成否</returns>
	bool Initialize(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent);
};