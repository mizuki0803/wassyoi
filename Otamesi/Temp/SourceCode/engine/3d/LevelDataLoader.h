#pragma once
#include "ObjObject3d.h"
#include "Vector3.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <json.hpp>

/// <summary>
/// レベルデータ読み込み
/// </summary>
class LevelDataLoader
{
	/// <summary>
	/// レベルデータ
	/// </summary>
	struct LevelData
	{
		//オブジェクト1個分のデータ
		struct ObjectData {
			std::string fileName;	//ファイル名
			Vector3 translation;	//平行移動
			Vector3 rotation;		//回転角
			Vector3 scaling;		//スケーリング
			bool isParent;
			std::vector<ObjectData> children;
		};

		//オブジェクトのコンテナ
		std::vector<ObjectData> objects;
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <returns>JSONファイル読み込み</returns>
	static LevelDataLoader* Create(const std::string& filename);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 影用光源ライトから見た視点での描画
	/// </summary>
	void DrawLightCameraView();

	/// <summary>
	/// モデルを格納
	/// </summary>
	/// <param name="filename">ファイル名</param>
	/// <param name="model">モデル</param>
	void InsertModel(const std::string& filename, ObjModel* model);

	/// <summary>
	/// 読み込んだレベルデータオブジェクト生成
	/// </summary>
	void CreateLevelDataObjects();


private: //メンバ関数
	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void LoadFile(const std::string& filename);

	/// <summary>
	/// シーンを再帰的に解析する
	/// </summary>
	/// <param name="object">オブジェクト</param>
	void ParseSceneRecursive(const nlohmann::json& object);


private: //静的メンバ変数
	//ディレクトリパス
	const static std::string directoryPath;

private: //メンバ変数
	//モデルデータコンテナ
	std::map<std::string, ObjModel*> models;
	//オブジェクトデータコンテナ
	std::vector<std::unique_ptr<ObjObject3d>> objects;
	//JSON文字列から解凍するデータ
	nlohmann::json deserialized;
	//レベルデータ格納用インスタンス
	LevelData* levelData;
};