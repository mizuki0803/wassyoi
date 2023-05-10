#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

/// <summary>
/// 丸影
/// </summary>
class CircleShadow
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
		XMVECTOR dir;
		Vector3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

public://メンバ関数

	//getter
	inline const XMVECTOR& GetDir() { return dir; }
	inline const Vector3& GetCasterPos() { return casterPos; }
	inline const float GetDistanceCasterLight() { return distanceCasterLight; }
	inline const XMFLOAT3& GetAtten() { return atten; }
	inline const XMFLOAT2& GetFactorAngleCos() { return factorAngleCos; }
	inline const bool GetActive() { return active; }

	//setter
	inline void SetDir(const XMVECTOR& dir) { this->dir = DirectX::XMVector3Normalize(dir); }
	inline void SetCasterPos(const Vector3& casterPos) { this->casterPos = casterPos; }
	inline void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }
	inline void SetAtten(const XMFLOAT3& atten) { this->atten = atten; }
	inline void SetFactorAngleCos(const XMFLOAT2& factorAngleCos) {
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngleCos.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngleCos.y));
	}
	inline void SetActive(bool active) { this->active = active; }


private://メンバ変数
	//方向(単位ベクトル)
	XMVECTOR dir = { 1, 0, 0, 0 };
	//キャスターとライトの距離
	float distanceCasterLight = 100.0f;
	//キャスター座標(ワールド座標系)
	Vector3 casterPos = { 0, 0, 0 };
	//距離減衰係数
	XMFLOAT3 atten = { 0.5f, 0.6f, 0.0f };
	//減衰角度
	XMFLOAT2 factorAngleCos = { 0.2f, 0.5f };
	//有効フラグ
	bool active = false;
};