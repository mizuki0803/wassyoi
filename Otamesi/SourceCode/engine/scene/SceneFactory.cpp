#include "SceneFactory.h"
#include "GameScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//���̃V�[���𐶐�
	BaseScene* newScene = nullptr;

	 if (sceneName == "GAME") { newScene = new GameScene(); }

	return newScene;
}
