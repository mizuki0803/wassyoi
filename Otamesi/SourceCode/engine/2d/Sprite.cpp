#include "Sprite.h"
#include "DescHeapSRV.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <d3dx12.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;


Sprite* Sprite::Create(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	//インスタンスを生成
	Sprite* instance = new Sprite();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(texture, anchorpoint, isFlipX, isFlipY)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool Sprite::Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	this->texture = texture;
	this->anchorpoint = anchorpoint;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	HRESULT result;

	//頂点データ
	VertexPosUv vertices[] = {
		{{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},	//左下
		{{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},	//左上
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},	//右下
		{{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},	//右上
	};

	//指定番号の画像が読み込み済みなら
	if (texture.texBuff.Get()) {
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = texture.texBuff.Get()->GetDesc();
		//スプライトの大きさを画像の解像度に合わせる
		size = { (float)resDesc.Width, (float)resDesc.Height };
		texSize = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//頂点バッファ生成
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));

	//頂点バッファへのデータ転送
	TransferVertexBuffer();

	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//定数バッファの生成
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));

	//定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);	//色指定(RGBA)
	//平行投影行列
	constMap->mat = spriteCommon->GetMatProjection();
	constBuff->Unmap(0, nullptr);


	return true;
}

void Sprite::TransferVertexBuffer()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	HRESULT result;

	//頂点データ
	VertexPosUv vertices[] = {
		//
		{{}, {0.0f, 1.0f}},	//左下
		{{}, {0.0f, 0.0f}},	//左上
		{{}, {1.0f, 1.0f}},	//右下
		{{}, {1.0f, 0.0f}},	//右上
	};

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;

	//左右入れ替え
	if (isFlipX) {
		left = -left;
		right = -right;
	}
	//上下反転
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	//左下　左上　右下　右上
	enum { LB, LT, RB, RT };

	vertices[LB].pos = { left, bottom,  0.0f };	//左下
	vertices[LT].pos = { left, top,     0.0f };	//左上
	vertices[RB].pos = { right, bottom, 0.0f };	//右下
	vertices[RT].pos = { right, top,    0.0f };	//右上

	//指定番号の画像が読み込み済みなら
	if (texture.texBuff.Get()) {
		//テクスチャ情報取得
		D3D12_RESOURCE_DESC resDesc = texture.texBuff.Get()->GetDesc();

		float tex_left = texLeftTop.x / resDesc.Width;
		float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
		float tex_top = texLeftTop.y / resDesc.Height;
		float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom };	//左下
		vertices[LT].uv = { tex_left, tex_top };	//左上
		vertices[RB].uv = { tex_right, tex_bottom };//右下
		vertices[RT].uv = { tex_right, tex_top };	//右上
	}

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
}

void Sprite::Update()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//ワールド行列の更新
	matWorld = XMMatrixIdentity();
	//Z軸回転
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	//平行移動
	matWorld *= XMMatrixTranslation(position.x, position.y, 0);

	//頂点バッファに反映
	TransferVertexBuffer();

	//定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * spriteCommon->GetMatProjection();
	constMap->color = color;
	constBuff->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	ID3D12GraphicsCommandList* cmdList = spriteCommon->GetCmdList();

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//ルートパラメータ0番に定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//シェーダリソースビューをセット
	DescHeapSRV::SetGraphicsRootDescriptorTable(1, texture.texNumber);

	//ポリゴンの描画(4頂点で四角形)
	cmdList->DrawInstanced(4, 1, 0, 0);
}
