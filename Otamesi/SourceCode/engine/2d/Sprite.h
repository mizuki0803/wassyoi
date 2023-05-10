#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "SpriteCommon.h"
#include "Vector3.h"
#include "Vector2.h"

/// <summary>
/// スプライト
/// </summary>
class Sprite
{
protected: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	struct VertexPosUv
	{
		Vector3 pos;	//xyz座標
		Vector2 uv;	//uv座標
	};

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	//色(RGBA)
		XMMATRIX mat;	//3変換行列
	};

public: //メンバ関数
	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>スプライト</returns>
	static Sprite* Create(const Texture& texture, const Vector2& anchorpoint = { 0.5f, 0.5f }, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>成否</returns>
	virtual bool Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// 頂点バッファの転送
	/// </summary>
	void TransferVertexBuffer();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	const Vector2& GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const Vector2& GetSize() { return size; }
	const Vector2& GetTexSize() { return texSize; }
	const Vector2& GetTexLeftTop() { return texLeftTop; }
	const XMFLOAT4& GetColor() { return color; }
	const Vector2& GetAnchorpoint() { return anchorpoint; }
	const bool GetIsFlipX() { return isFlipX; }
	const bool GetIsFlipY() { return isFlipY; }

	//setter
	void SetTexture(const Texture& texture) { this->texture = texture; }
	void SetPosition(const Vector2& position) { this->position = position; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetSize(const Vector2& size) { this->size = size; }
	void SetTexSize(const Vector2& texSize) { this->texSize = texSize; }
	void SetTexLeftTop(const Vector2& texLeftTop) { this->texLeftTop = texLeftTop; }
	void SetColor(const XMFLOAT4& color) { this->color = color; }
	void SetAnchorpoint(const Vector2& anchorpoint) { this->anchorpoint = anchorpoint; }
	void SetIsFlipX(bool isFlipX) { this->isFlipX = isFlipX; }
	void SetIsFlipY(bool isFlipY) { this->isFlipY = isFlipY; }


protected: //メンバ変数
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//Z軸まわりの回転を回転角
	float rotation = 0.0f;
	//座標
	Vector2 position = { 0, 0 };
	//ワールド行列
	XMMATRIX matWorld;
	//色(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//テクスチャ
	Texture texture;
	//大きさ
	Vector2 size = { 100, 100 };
	//アンカーポイント
	Vector2 anchorpoint = { 0.5f, 0.5f };
	//左右反転
	bool isFlipX = false;
	//上下反転
	bool isFlipY = false;
	//テクスチャ左上座標
	Vector2 texLeftTop = { 0, 0 };
	//テクスチャ切り出しサイズ
	Vector2 texSize = { 100, 100 };
};