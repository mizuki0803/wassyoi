#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/// <summary>
/// シーン管理
/// </summary>
class SceneManager final
{
private: //シングルトン化
	//コンストラクタを隠蔽
	SceneManager() = default;
	//デストラクタを隠蔽
	~SceneManager();
public:
	//コピーコンストラクタを無効化
	SceneManager(const SceneManager& sceneManager) = delete;
	//代入演算子を無効化
	void operator = (const SceneManager& sceneManager) = delete;


public: //メンバ関数
	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>シーンマネージャー</returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite();

	/// <summary>
	/// 3Dシーン描画
	/// </summary>
	void Draw3D();

	/// <summary>
	/// 3Dシーン描画(ライトから見た視点)
	/// </summary>
	void Draw3DLightView();

	/// <summary>
	/// 前景スプライト描画
	/// </summary>
	void DrawFrontSprite();

	/// <summary>
	/// 次のシーンをセット
	/// </summary>
	/// <param name="sceneName">次のシーン</param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// シーン工場をセット
	/// </summary>
	/// <param name="sceneFactory">シーン工場</param>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { this->sceneFactory = sceneFactory; }

private: //メンバ変数
	//今のシーン
	BaseScene* scene = nullptr;
	//次のシーン
	BaseScene* nextScene = nullptr;
	//シーン工場
	AbstractSceneFactory* sceneFactory = nullptr;
};

