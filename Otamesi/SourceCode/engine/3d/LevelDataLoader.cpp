#include "LevelDataLoader.h"
#include <fstream>

const std::string LevelDataLoader::directoryPath = "Resources/levels/";

LevelDataLoader* LevelDataLoader::Create(const std::string& filename)
{
	//複数撃破UIのインスタンスを生成
	LevelDataLoader* jsonLoader = new LevelDataLoader();
	if (jsonLoader == nullptr) {
		return nullptr;
	}

	//ファイル読み込み
	jsonLoader->LoadFile(filename);

	return jsonLoader;
}

void LevelDataLoader::Update()
{
	//オブジェクト更新
	for (const std::unique_ptr<ObjObject3d>& object : objects) {
		object->Update();
	}
}

void LevelDataLoader::Draw()
{
	//オブジェクト描画
	for (const std::unique_ptr<ObjObject3d>& object : objects) {
		object->Draw();
	}
}

void LevelDataLoader::DrawLightCameraView()
{
	//オブジェクト描画
	for (const std::unique_ptr<ObjObject3d>& object : objects) {
		object->DrawLightCameraView();
	}
}

void LevelDataLoader::InsertModel(const std::string& filename, ObjModel* model)
{
	//モデルをコンテナに格納
	models.insert(std::make_pair(filename, model));
}

void LevelDataLoader::CreateLevelDataObjects()
{
	//レベルデータ格納用インスタンスを生成
	levelData = new LevelData();

	//"objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {
		ParseSceneRecursive(object);
	}

	//レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		ObjModel* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		//3Dオブジェクトを生成
		std::unique_ptr<ObjObject3d> newObject;
		newObject.reset(ObjObject3d::Create(model));
		//平行移動
		newObject->SetPosition(objectData.translation);
		//回転角
		newObject->SetRotation(objectData.rotation);
		//スケーリング
		newObject->SetScale(objectData.scaling);
		//シャドウマップでの影描画を可能にしておく
		newObject->SetIsShadowMap(true);

		//配列に登録
		objects.push_back(std::move(newObject));
	}
}

void LevelDataLoader::LoadFile(const std::string& filename)
{
	//連結してフルパスを得る
	const std::string fullpath = directoryPath + filename;

	//ファイルストリーム
	std::ifstream file;

	//ファイルを開く
	file.open(fullpath);
	//ファイルオープン失敗をチェック
	if (file.fail()) {
		assert(0);
	}

	//解凍
	file >> deserialized;

	//正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"を文字列として取得
	std::string name =
		deserialized["name"].get<std::string>();
	//正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);
}

void LevelDataLoader::ParseSceneRecursive(const nlohmann::json& object)
{
	//"objects"の全オブジェクトを走査
	assert(object.contains("type"));

	//種別を取得
	std::string type = object["type"].get<std::string>();

	//種類ごとの処理
	//MESH
	if (type.compare("MESH") == 0) {
		//要素追加
		levelData->objects.emplace_back(LevelData::ObjectData{});
		//今追加した要素の参照を得る
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (object.contains("file_name")) {
			//ファイル名
			objectData.fileName = object["file_name"];
		}

		//トランスフォームのパラメータ読み込み
		nlohmann::json transform = object["transform"];
		//平行移動
		objectData.translation.x = (float)transform["translation"][1];
		objectData.translation.y = (float)transform["translation"][2];
		objectData.translation.z = -(float)transform["translation"][0];
		//回転角
		objectData.rotation.x = -(float)transform["rotation"][1];
		objectData.rotation.y = -(float)transform["rotation"][2];
		objectData.rotation.z = (float)transform["rotation"][0];
		//回転角
		objectData.scaling.x = (float)transform["scaling"][1];
		objectData.scaling.y = (float)transform["scaling"][2];
		objectData.scaling.z = (float)transform["scaling"][0];
	}

	//子ノードがあれば再帰処理
	if (object.contains("children")) {
		for (const nlohmann::json& child : object["children"]) {
			ParseSceneRecursive(child);
		}
	}
}
