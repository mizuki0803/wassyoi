#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 鳥(胴体)
/// </summary>
class BirdBody : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">座標</param>
	/// <param name="velocity">移動速度</param>
	/// <returns>鳥(胴体)</returns>
	static BirdBody* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 飛行移動
	/// </summary>
	void FlyMove();

private: //メンバ変数
	//移動速度
	Vector3 velocity;
};