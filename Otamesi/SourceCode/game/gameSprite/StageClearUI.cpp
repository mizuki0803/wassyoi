#include "StageClearUI.h"
#include "SpriteTextureLoader.h"
#include "WindowApp.h"

StageClearUI* StageClearUI::Create()
{
	//�C���X�^���X����
	StageClearUI* instance = new StageClearUI();

	//����������
	instance->Initialize();

	return instance;
}

void StageClearUI::Initialize()
{
	//UI����
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::StageClear), { WindowApp::window_width / 2, 300 }, { 513, 84 }, 2.0f);	//�X�e�[�W�N���A
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::PressSpace), { WindowApp::window_width / 2, 660 }, { 550, 85 }, 0.5f);	//PRESS SPACE
}

void StageClearUI::Update()
{
	//�X�v���C�g�X�V
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Update();
	}
}

void StageClearUI::Draw()
{
	//�X�v���C�g�`��
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Draw();
	}
}

void StageClearUI::CreateNewSprite(const Texture& texture, const Vector2& position, const Vector2& texSize, const float sizeRatio)
{
	//�X�v���C�g����
	std::unique_ptr<Sprite> newSprite;
	newSprite.reset(Sprite::Create(texture));
	newSprite->SetPosition(position);
	newSprite->SetTexSize(texSize);
	newSprite->SetSize(texSize * sizeRatio);
	sprites.push_back(std::move(newSprite));
}
