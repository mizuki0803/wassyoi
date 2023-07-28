#pragma once
#include "LightCamera.h"

/// <summary>
/// ゲームに使用する光源カメラ
/// </summary>
class GameLightCamera : public LightCamera
{
public: //静的メンバ変数
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="centerDistance">中心からの距離</param>
	/// <returns>ゲームに使用する光源カメラ</returns>
	static GameLightCamera* Create(float centerDistance);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="centerDistance">中心からの距離</param>
	void Initialize(float centerDistance);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 中心を軸に回転
	/// </summary>
	void Around();

private: //メンバ変数
	//中心からの距離
	float centerDistance;
	//回転に仕様する回転角
	float aroundRotate = 0;
};