#include "SceneChangeEffect.h"

std::list<std::unique_ptr<SceneChangeSprite>> SceneChangeEffect::sceneChangeSprites;

void SceneChangeEffect::Update()
{
	//���S��Ԃ̃V�[���ύX���o�p�X�v���C�g�̍폜
	sceneChangeSprites.remove_if([](std::unique_ptr<SceneChangeSprite>& sceneChangeSprite) {
		return sceneChangeSprite->GetIsDead();
		});

	//�V�[���ύX���o�p�X�v���C�g�X�V
	for (const std::unique_ptr<SceneChangeSprite>& sceneChangeSprite : sceneChangeSprites) {
		sceneChangeSprite->Update();
	}
}

void SceneChangeEffect::Draw()
{
	//�V�[���ύX���o�p�X�v���C�g�`��
	for (const std::unique_ptr<SceneChangeSprite>& sceneChangeSprite : sceneChangeSprites) {
		sceneChangeSprite->Draw();
	}
}

void SceneChangeEffect::CreateSceneChangeEffect(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime)
{
	//�V�����V�[���ύX���o�p�X�v���C�g�𐶐�����
	std::unique_ptr<SceneChangeSprite> newSceneChangeSprite;
	newSceneChangeSprite.reset(SceneChangeSprite::Create(color, deepenTime, waitTime, returnTime));
	sceneChangeSprites.push_back(std::move(newSceneChangeSprite));
}
