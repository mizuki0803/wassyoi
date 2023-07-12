#pragma once
#include <DirectXTex.h>

/// <summary>
/// 平行光源
/// </summary>
class DirectionalLight
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;		//ライトへの方向を表すベクトル
		XMFLOAT3 lightcolor;	//ライトの色
		unsigned int active;	//有効フラグ
	};


public: //メンバ関数

	//getter
	inline const XMVECTOR& GetLightDir() { return lightdir; }
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	inline const bool GetActive() { return active; }

	//setter
	inline void SetLightDir(const XMVECTOR& lightdir) { this->lightdir = DirectX::XMVector3Normalize(lightdir); }
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	inline void SetActive(bool active) { this->active = active; }

private: // メンバ変数
	//ライト光線方向(単位ベクトル)
	XMVECTOR lightdir = { 1,0,0,0 };
	//ライト色
	XMFLOAT3 lightcolor = { 1,1,1 };
	//有効フラグ
	bool active = false;
};