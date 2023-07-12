#include "TextureManager.h"
#include "DescHeapSRV.h"
#include "DirectXBase.h"
#include <cassert>
#include <DirectXTex.h>

using namespace DirectX;
using namespace Microsoft::WRL;

using namespace std;

ID3D12Device* TextureManager::dev = nullptr;

void TextureManager::LoadTexture(Texture& texture, const std::string& directoryPath, const std::string& filename)
{
	HRESULT result;

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//ファイルパスを結合
	string filepath = directoryPath + filename;

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1,
		wfilepath, _countof(wfilepath));

	//ファイル名と拡張子を分離する
	std::string exceptExt;
	std::string fileExt;
	//区切り文字 '.'が出てくる一番最後の部分を検索
	size_t extDotPos = filepath.rfind('.');
	//検索がヒットしたら
	if (extDotPos != std::string::npos) {
		//区切り文字の後ろをファイル拡張子として保存
		fileExt = filepath.substr(extDotPos + 1, filepath.size() - extDotPos - 1);
		//区切り文字の前までを抜き出す
		exceptExt = filepath.substr(0, extDotPos);
	}
	else {
		fileExt = "";
		exceptExt = filepath;
	}

	//ddsファイルかその他形式ファイルかで切り替え
	if (fileExt == "dds") {
		result = LoadFromDDSFile(wfilepath, DDS_FLAGS_NONE, &metadata, scratchImg);
	}
	else {
		result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	}
	assert(SUCCEEDED(result));

	//生データ抽出
	const Image* img = scratchImg.GetImage(0, 0, 0);

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texture.texBuff));

	//テクスチャバッファにデータ転送
	result = texture.texBuff->WriteToSubresource(
		0,
		nullptr,	//全領域コピー
		img->pixels,	//元データアドレス
		(UINT)img->rowPitch,	//1ラインサイズ
		(UINT)img->slicePitch	//1枚サイズ
	);

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//デスクリプタヒープにSRV作成
	DescHeapSRV::CreateShaderResourceView(srvDesc, texture);
}
