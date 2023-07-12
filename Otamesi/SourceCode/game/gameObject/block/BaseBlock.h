#pragma once
#include "InstanceObject.h"
#include <array>

class BaseBlock
{
private:

	enum class BROCK_MODEL_TYPE {
		block,
		plane,
		goal,
		player,
		size,
	};

	enum class BROCK_TYPE {
		block,
		plane = 10,
		goal,
		player,
		size,
	};

	//objモデル
	std::array<std::unique_ptr<ObjModel>, int(BROCK_MODEL_TYPE::size)> model;

	//描画用クラス
	std::array<std::unique_ptr<InstanceObject>, int(BROCK_TYPE::size)> block;

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void StaticInitialize();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="_blockType">ブロック種類</param>
	/// <param name="_pos">座標</param>
	/// <param name="_scale">大きさ</param>
	/// <param name="_rotation">回転角</param>
	/// <param name="_color">色</param>
	void Update(const int _blockType, const XMFLOAT3& _pos, const XMFLOAT3& _scale,
		const XMFLOAT3& _rotation, const XMFLOAT4& _color = { 1.0f,1.0f,1.0f,1.0f }, const XMMATRIX* _parentWorld = nullptr);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

};

