#include "HowToPlayUI.h"
#include "SpriteTextureLoader.h"

HowToPlayUI* HowToPlayUI::Create(bool isDrawPause)
{
	//�C���X�^���X����
	HowToPlayUI* instance = new HowToPlayUI();

	//����������
	instance->Initialize(isDrawPause);

	return instance;
}

void HowToPlayUI::Initialize(bool isDrawPause)
{
	//UI����
	Vector2 leftTopPos = { 20, 20 };
	const float sizeRatio = 0.4f;	//�e�N�X�`���T�C�Y�ɑ΂���傫���̔䗦
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), leftTopPos, { 465, 84 }, sizeRatio);	//�ړ�
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), leftTopPos, { 545, 84 }, sizeRatio);	//�J����
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayUndo), leftTopPos, { 573, 84 }, sizeRatio);	//�A���h�D
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayRedo), leftTopPos, { 574, 84 }, sizeRatio);	//���h�D

	//�|�[�Y�̕`������Ȃ��Ȃ甲����
	if (!isDrawPause) { return; }
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayReset), leftTopPos, { 400, 84 }, sizeRatio);	//���Z�b�g
	CreateNewSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), leftTopPos, { 674, 84 }, sizeRatio);		//�|�[�Y
}

void HowToPlayUI::Update()
{
	//�X�v���C�g�X�V
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Update();
	}
}

void HowToPlayUI::Draw()
{
	//�X�v���C�g�`��
	for (const std::unique_ptr<Sprite>& sprite : sprites) {
		sprite->Draw();
	}
}

void HowToPlayUI::CreateNewSprite(const Texture& texture, Vector2& position, const Vector2& texSize, const float sizeRatio)
{
	//�X�v���C�g����
	std::unique_ptr<Sprite> newSprite;
	newSprite.reset(Sprite::Create(texture, { 0, 0 }));
	newSprite->SetPosition(position);
	newSprite->SetTexSize(texSize);
	newSprite->SetSize(texSize * sizeRatio);
	newSprite->SetColor({ 1, 1, 0, 1 });
	sprites.push_back(std::move(newSprite));

	//���̃X�v���C�g�̍�����W���Z�b�g���Ă���
	position.y += texSize.y * sizeRatio;
}
