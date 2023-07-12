﻿#pragma once

#include "fbxsdk.h"
#include "FbxModel.h"

#include <d3d12.h>
#include <d3dx12.h>
#include <string>

class FbxLoader
{
private: //エイリアス
	//std::を省略
	using string = std::string;

public: //定数
	//モデル格納ルートパス
	static const string baseDirectory;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;


public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXファイルを読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	//void LoadModelFromFile(const string& modelName);
	FbxModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	/// <param name="parent">親ノード</param>
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="fbxModel">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);


	//頂点座標読み取り
	void ParseMashVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//面情報読み取り
	void ParseMashFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	//スキニング情報の読み取り
	void ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh);


	//テクスチャ読み取り
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);

	//ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

private:
	//テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;

private:
	//デバイス
	ID3D12Device* device = nullptr;
	//FBXマネージャー
	FbxManager* fbxManager = nullptr;
	//FBXインポーター
	FbxImporter* fbxImporter = nullptr;
};