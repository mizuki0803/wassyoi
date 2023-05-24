#include "BaseGameScene.h"
#include "SceneManager.h"
#include "SceneChangeEffect.h"

void BaseGameScene::SceneChangeStart(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime, const std::string& nextSceneName)
{
	//���ɃV�[���ύX��ԂȂ甲����
	if (isSceneChange) { return; }

	//�V�[���ύX�ɂ����鎞�Ԃ��Z�b�g
	sceneChangeTime = deepenTime + waitTime;
	//�V�[���ύX��Ԃɂ���
	isSceneChange = true;
	//���̃V�[�������Z�b�g
	this->nextSceneName = nextSceneName;

	//�V�[���ύX�p���o����
	SceneChangeEffect::CreateSceneChangeEffect(color, deepenTime, waitTime, returnTime);
}

void BaseGameScene::SceneChangeMode()
{
	//�V�[���ύX��ԂłȂ���Δ�����
	if (!isSceneChange) { return; }

	//�V�[���ύX�^�C�}�[���X�V
	sceneChangeTimer++;
	//�^�C�}�[���w�肵�����Ԉȉ��Ȃ甲����
	if (sceneChangeTimer < sceneChangeTime) { return; }

	//�V�[���؂�ւ�
	SceneManager::GetInstance()->ChangeScene(nextSceneName);
}
