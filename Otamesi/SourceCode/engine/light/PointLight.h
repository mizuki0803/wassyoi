#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

/// <summary>
/// 点光源
/// </summary>
class PointLight
{
private://エイリアス
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vector3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		unsigned int active;
	};

public://メンバ関数

	//getter
	inline const Vector3& GetLightPos() { return lightpos; }
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	inline const XMFLOAT3& GetLightAtten() { return lightatten; }
	inline const bool GetActive() { return active; }

	//setter
	inline void SetLightPos(const Vector3& lightpos) { this->lightpos = lightpos; }
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	inline void SetLightAtten(const XMFLOAT3& lightatten) { this->lightatten = lightatten; }
	inline void SetActive(bool active) { this->active = active; }

private://メンバ変数
	//ライト座標(ワールド座標系)
	Vector3 lightpos = { 0, 0, 0 };
	//ライト色
	XMFLOAT3 lightcolor = { 1, 1, 1 };
	//ライト距離減衰係数
	XMFLOAT3 lightatten = { 1.0f, 1.0f, 1.0f };
	//有効フラグ
	bool active = false;
};