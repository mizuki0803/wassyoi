#include "SceneFactory.h"
#include "GameScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//次のシーンを生成
	BaseScene* newScene = nullptr;

	 if (sceneName == "GAME") { newScene = new GameScene(); }

	return newScene;
}
