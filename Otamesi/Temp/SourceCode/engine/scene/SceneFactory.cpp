#include "SceneFactory.h"
#include "TitleScene.h"
#include "StageSelectScene.h"
#include "GameScene.h"

BaseScene* SceneFactory::CreateScene(const std::string& sceneName)
{
	//���̃V�[���𐶐�
	BaseScene* newScene = nullptr;

	if (sceneName == "TITLE") { newScene = new TitleScene(); }
	else if (sceneName == "STAGESELECT") { newScene = new StageSelectScene(); }
	else if (sceneName == "GAME") { newScene = new GameScene(); }

	return newScene;
}
