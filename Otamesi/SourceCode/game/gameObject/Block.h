#pragma once
#include "ObjObject3d.h"

/// <summary>
/// マップ用ブロック
/// </summary>
class Block : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ブロック</returns>
	static Block* Create(ObjModel* model, const Vector3& position);

	//getter
	static float GetBlockSize() { return Block::blockSize; }

private: //静的メンバ変数
	//ブロック一つの大きさ
	static const float blockSize;
};