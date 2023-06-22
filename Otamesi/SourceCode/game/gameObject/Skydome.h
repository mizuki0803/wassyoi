#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 天球
/// </summary>
class Skydome : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>天球</returns>
	static Skydome* Create(ObjModel* model);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//setter
	void SetIsRotate(const bool isRotate) { this->isRotate = isRotate; }

private: //メンバ関数
	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

private: //静的メンバ変数
	//サイズ
	static const float size;
	//回転スピード
	static const float rotSpeed;

private: //メンバ変数
	//回転するか
	bool isRotate = false;
};