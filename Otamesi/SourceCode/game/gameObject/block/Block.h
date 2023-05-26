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

protected: //静的メンバ変数
	//ブロック一つの大きさ
	static const float blockSize;

protected: // メンバ変数
	//ブロックの種類
	BlockType blockType;
};