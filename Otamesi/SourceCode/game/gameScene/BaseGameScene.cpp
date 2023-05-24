#include "BaseGameScene.h"
#include "SceneManager.h"
#include "SceneChangeEffect.h"

void BaseGameScene::SceneChangeStart(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime, const std::string& nextSceneName)
{
	//既にシーン変更状態なら抜ける
	if (isSceneChange) { return; }

	//シーン変更にかかる時間をセット
	sceneChangeTime = deepenTime + waitTime;
	//シーン変更状態にする
	isSceneChange = true;
	//次のシーン名をセット
	this->nextSceneName = nextSceneName;

	//シーン変更用演出生成
	SceneChangeEffect::CreateSceneChangeEffect(color, deepenTime, waitTime, returnTime);
}

void BaseGameScene::SceneChangeMode()
{
	//シーン変更状態でなければ抜ける
	if (!isSceneChange) { return; }

	//シーン変更タイマーを更新
	sceneChangeTimer++;
	//タイマーが指定した時間以下なら抜ける
	if (sceneChangeTimer < sceneChangeTime) { return; }

	//シーン切り替え
	SceneManager::GetInstance()->ChangeScene(nextSceneName);
}
