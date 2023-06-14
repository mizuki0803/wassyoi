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

	/// <summary>
	/// 影用光源ライトから見た視点での描画
	/// </summary>
	void DrawLightCameraView();

private:

	//objモデル
	std::unique_ptr<ObjModel> model;

	//描画用クラス
	std::unique_ptr<InstanceObject> block[3];

	//オブジェクトの状態
	std::vector<std::array<DirectX::XMFLOAT3, int(transform::size)>> info;
};

