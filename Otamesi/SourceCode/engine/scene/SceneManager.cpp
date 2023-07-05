#include "SceneManager.h"
#include "DescHeapSRV.h"
#include <cassert>

SceneManager::~SceneManager()
{
	//最後のシーンの終了と解放
	delete scene;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::Finalize()
{
	scene->Finalize();
}

void SceneManager::Update()
{
	//シーン切り替えがあるなら
	if (nextScene) {
		//旧シーンがあるなら
		if (scene) {
			//旧シーンの終了
			delete scene;
		}
		//シーンを切り替え
		scene = nextScene;
		//次のシーンは空にしておく
		nextScene = nullptr;

		//全シーン共通で使用するテクスチャの枚数までインデックスを戻す
		DescHeapSRV::TextureDestruction();

		//新しいシーン初期化
		scene->Initialize();
	}

	//毎フレーム更新
	scene->Update();
}

void SceneManager::DrawBackSprite()
{
	//背景スプライト描画
	scene->DrawBackSprite();
}

void SceneManager::Draw3D()
{
	//3Dシーン描画
	scene->Draw3D();
}

void SceneManager::AfterBloomDraw()
{
	scene->AfterBloomDraw();
}

void SceneManager::OutLineDraw()
{

	scene->OutLineDraw();
}

void SceneManager::Draw3DLightView()
{
	//3Dシーン描画(ライトからの視点)
	scene->Draw3DLightView();
}

void SceneManager::DrawFrontSprite()
{
	//前景スプライト描画
	scene->DrawFrontSprite();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//次のシーンをセット
	nextScene = sceneFactory->CreateScene(sceneName);
}
