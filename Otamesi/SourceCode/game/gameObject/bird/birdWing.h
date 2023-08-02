#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 鳥(翼)
/// </summary>
class BirdWing : public ObjObject3d
{
public: //enum
	//翼のタイプ
	enum WingType
	{
		LeftWing,	//左翼
		RightWing,	//右翼
	};


public: //メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="parent">親オブジェクト(鳥胴体)</param>
	/// <param name="wingType">翼のタイプ</param>
	/// <param name="position">座標</param>
	/// <returns>鳥(翼)</returns>
	static BirdWing* Create(ObjModel* model, ObjObject3d* parent, WingType wingType, const Vector3& position);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 羽ばたき
	/// </summary>
	void Flap();

private: //メンバ変数
	//翼のタイプ
	WingType wingType;
	//翼の回転スピード
	float rotateSpeed = 0;
	//翼を上げているか
	bool isWingUp = true;
};