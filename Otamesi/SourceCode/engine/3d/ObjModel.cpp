#include "ObjModel.h"
#include "DescHeapSRV.h"
#include "TextureManager.h"
#include <cassert>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <DirectXTex.h>

using namespace DirectX;
using namespace Microsoft::WRL;

using namespace std;

//静的メンバ変数の実体
ID3D12Device* ObjModel::dev = nullptr;
Texture ObjModel::shadowMapTexture;

ObjModel* ObjModel::LoadFromOBJ(const std::string& modelname, const bool smoothing)
{
	//新たなObjModel型のインスタンスのメモリを確保
	ObjModel* model = new ObjModel();

	//objファイルからデータ読み込み
	model->LoadFromOBJInternal(modelname, smoothing);

	//バッファ生成
	model->CreateBuffers();

	return model;
}

void ObjModel::LoadFromOBJInternal(const std::string& modelname, const bool smoothing)
{
	//objファイルからデータを読み込む
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/model/" + modelname + "/";

	//ファイルストリーム
	std::ifstream file;
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	int indexCountTex = 0;

	vector<XMFLOAT3> positions;	//頂点座標
	vector<XMFLOAT3> normals;	//法線ベクトル
	vector<XMFLOAT2> texcoords;	//テクスチャUV
	//1行ずつ読み込む
	string line;
	while (getline(file, line)) {

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib") {
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		//先頭文字列がvなら頂点座標
		if (key == "v") {
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt") {
			//U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			//U,V成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}

		//先頭文字列がfならポリゴン(三角形)
		if (key == "f") {
			//四角形ポリゴンか数える用変数
			int faceCount = 0;

			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
				index_stream >> indexNormal;

				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);

				//エッジ平滑化用のデータを追加
				if (smoothing) {
					//vキー(座標データ)の番号と、全て合成した頂点のインデックスをセットで登録する
					smoothData[indexPosition].emplace_back((unsigned short)GetVertexCount() - 1);
				}

				//四角形ポリゴン対応
				if (faceCount >= 3) {
					//インデックスデータの追加
					indices.emplace_back((unsigned short)indexCountTex - 1);
					indices.emplace_back((unsigned short)indexCountTex);
					indices.emplace_back((unsigned short)indexCountTex - 3);
				}
				else {
					//インデックスデータの追加
					indices.emplace_back((unsigned short)indexCountTex);

				}
				faceCount++;
				indexCountTex++;
			}
		}
	}
	//ファイルを閉じる
	file.close();

	//頂点法線の平均によるエッジの平滑化
	if (smoothing) {
		auto itr = smoothData.begin();
		for (; itr != smoothData.end(); ++itr) {
			//各面用の共通頂点コレクション
			std::vector<unsigned short>& v = itr->second;
			//全頂点の法線を平均する
			XMVECTOR normal = {};
			for (unsigned short index : v) {
				normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
			}
			normal = XMVector3Normalize(normal / (float)v.size());
			//共通法線を使用する全ての頂点データに書き込む
			for (unsigned short index : v) {
				vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
			}
		}
	}
}

void ObjModel::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//1行ずつ読み込む
	string line;
	while (getline(file, line)) {
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);

		//半角スペース区切りで行分の文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭のタブ文字は無視する
		if (key[0] == '\t') {
			key.erase(key.begin());	//先頭の文字を削除
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl") {
			//マテリアル名読み込み
			line_stream >> material.name;
		}

		//先頭文字列がKaならアンビエント色
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}

		//先頭文字列がKsならスペキュラー色
		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}

		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd") {
			//テクスチャファイル名読み込み
			line_stream >> material.textureFilename;
			//テクスチャ読み込み
			TextureManager::LoadTexture(texture, directoryPath, material.textureFilename);
		}
	}
	//ファイルを閉じる
	file.close();
}

void ObjModel::CreateBuffers()
{
	HRESULT result;

	//頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	//インデックスデータ全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	//頂点バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),	//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	//インデックスバッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),	//アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}


	//頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}


	//インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);

		indexBuff->Unmap(0, nullptr);
	}


	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));

	//定数バッファへのデータ転送
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	if (SUCCEEDED(result)) {
		constMap1->ambient = material.ambient;
		constMap1->diffuse = material.diffuse;
		constMap1->specular = material.specular;
		constMap1->alpha = material.alpha;
		constBuffB1->Unmap(0, nullptr);
	}
}

void ObjModel::AddSmootgData(const unsigned short indexPosition, const unsigned short indexVertex)
{
	smoothData[indexPosition].emplace_back(indexVertex);
}

void ObjModel::CalculateSmoothedVertexNormals()
{
	auto itr = smoothData.begin();
	for (; itr != smoothData.end(); ++itr) {
		//各面用の共通頂点コレクション
		std::vector<unsigned short>& v = itr->second;
		//全頂点の法線を平均する
		XMVECTOR normal = {};
		for (unsigned short index : v) {
			normal += XMVectorSet(vertices[index].normal.x, vertices[index].normal.y, vertices[index].normal.z, 0);
		}
		normal = XMVector3Normalize(normal / (float)v.size());
		//共通法線を使用する全ての頂点データに書き込む
		for (unsigned short index : v) {
			vertices[index].normal = { normal.m128_f32[0], normal.m128_f32[1], normal.m128_f32[2] };
		}
	}
}

void ObjModel::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial, UINT _instanceDrawNum)
{
	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定
	cmdList->IASetIndexBuffer(&ibView);

	//定数バッファビューをセット(マテリアル)
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());

	if (material.textureFilename.size() > 0) {
		//シェーダリソースビューをセット
		DescHeapSRV::SetGraphicsRootDescriptorTable(2, texture.texNumber);

		DescHeapSRV::SetGraphicsRootDescriptorTable(3, shadowMapTexture.texNumber);
	}

	//描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), _instanceDrawNum, 0, 0, 0);
}

void ObjModel::DrawLightCameraView(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial, UINT _instanceDrawNum)
{
	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定
	cmdList->IASetIndexBuffer(&ibView);

	//定数バッファビューをセット(マテリアル)
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());

	if (material.textureFilename.size() > 0) {
		//シェーダリソースビューをセット
		DescHeapSRV::SetGraphicsRootDescriptorTable(2, texture.texNumber);
	}

	//描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), _instanceDrawNum, 0, 0, 0);
}

void ObjModel::DrawAdd(ID3D12GraphicsCommandList *cmdList, UINT rootParamIndexMaterial, UINT _instanceDrawNum)
{
	//頂点バッファビューの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファビューの設定
	cmdList->IASetIndexBuffer(&ibView);

	//定数バッファビューをセット(マテリアル)
	cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());

	if (material.textureFilename.size() > 0) {
		//シェーダリソースビューをセット
		DescHeapSRV::SetGraphicsRootDescriptorTable(2, texture.texNumber);

		DescHeapSRV::SetGraphicsRootDescriptorTable(3, shadowMapTexture.texNumber);
	}

	//描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), _instanceDrawNum, 0, 0, 0);
}
