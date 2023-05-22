#pragma once
#include "Block.h"

/// <summary>
/// マップ用ハリボテ(ブロック)
/// </summary>
class PlaneBlock : public Block
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="model">モデル</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <param name="rotation">傾ける角度</param>
	/// <returns>ブロック</returns>
	static PlaneBlock* Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& rotation);

public:
	/// <summary>
	/// くそ関数
	/// </summary>
	void aaaa();

	//getter
	static float GetBlockSize() { return Block::blockSize; }

};