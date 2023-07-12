#include "GamePostEffect.h"

std::unique_ptr<PostEffect> GamePostEffect::postEffect;

void GamePostEffect::Initialize()
{
	//�Q�[���Ŏg�p����|�X�g�G�t�F�N�g����
	postEffect.reset(PostEffect::Create());
}

void GamePostEffect::Draw()
{
	//�|�X�g�G�t�F�N�g�`��
	postEffect->Draw();
}

void GamePostEffect::DrawScenePrev()
{
	//�|�X�g�G�t�F�N�g�V�[���`��O����
	postEffect->DrawScenePrev();
}

void GamePostEffect::DrawSceneRear()
{
	//�|�X�g�G�t�F�N�g�V�[���`��㏈��
	postEffect->DrawSceneRear();
}
