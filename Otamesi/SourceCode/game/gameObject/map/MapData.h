#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <string>

/// <summary>
/// マップデータ
/// </summary>
class MapData
{
protected: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	//getter
	const XMINT3& GetMapSize() { return mapSize; }
	const std::vector<std::vector<std::vector<int>>>& GetMapChipNum() { return mapChipNum; }

protected: //メンバ関数
	/// <summary>
	/// マップ生成
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	bool CreateMap(const int selectStageNum);

	/// <summary>
	/// マップデータ読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	bool LoadMapData(const std::string& fileName);

	/// <summary>
	/// マップブロック生成
	/// </summary>
	virtual void CreateMapBlock() = 0;

protected: //メンバ変数
	//マップのサイズ
	XMINT3 mapSize;
	//3次元マップ用の3次元配列
	std::vector<std::vector<std::vector<int>>> mapChipNum;
};