#pragma once
#include "BaseBlock.h"
#include "ObjObject3d.h"
#include "EaseData.h"

/// <summary>
/// マップ用ブロック
/// </summary>
class Block
{
public: //enum
	/// <summary>
	/// ブロックの種類
	/// </summary>
	enum class BROCK_TYPE {
		block,
		plane = 10,
		goal,
		player,
		size,
	};

public: //静的メンバ関数

	/// <summary>
	/// 一回のみの初期化
	/// </summary>
	static void StaticInitialize();

	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="_blockType">モデル番号</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <returns>ブロック</returns>
	static Block* Create(const int _blockType, const XMINT3& mapChipNum);

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

	/// <summary>
/// frameごとの初期化
/// </summary>
	void FrameReset();

	//setter
	void SetRotation(const XMFLOAT3& _rotation) { rotation = _rotation; }
	void SetColor(const XMFLOAT4& _color) { color = _color; }

	//getter
	static float GetBlockSize() { return Block::blockSize; }

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_blockType">モデル</param>
	/// <param name="mapChipNum">マップチップの番号</param>
	/// <returns>成否</returns>
	bool Initialize(const int _blockType, const XMINT3& mapChipNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ブロックタイプの取得
	/// </summary>
	/// <returns>ブロックタイプ</returns>
	BROCK_TYPE GetBlockType() { return blockType; }

protected: //静的メンバ変数
	//ブロック一つの大きさ
	static const float blockSize;
	//ブロック描画用
	static std::unique_ptr<BaseBlock> instanceBlock;

protected: // メンバ変数
	//ブロックの種類
	BROCK_TYPE blockType;
	//大きさ
	XMFLOAT3 scale = { 1, 1, 1 };
	//回転角
	XMFLOAT3 rotation = { 0, 0, 0 };
	//座標
	Vector3 position = { 0, 0, 0 };
	//色
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//親オブジェクト
	ObjObject3d* parent;
};