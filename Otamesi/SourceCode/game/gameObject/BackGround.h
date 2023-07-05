#pragma once
#include "InstanceObject.h"

class BackGround
{
private:

	enum class transform {
		translation,
		rotation,
		scaling,
		size,
	};

public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static BackGround* Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	//objモデル
	std::unique_ptr<ObjModel> model;

	//描画用クラス
	std::unique_ptr<InstanceObject> underBlock[3];
	std::unique_ptr<InstanceObject> upperBlock[3];

	//オブジェクトの状態
	std::vector<std::array<DirectX::XMFLOAT3, int(transform::size)>> info;
};

