#include "UserInterface.h"
#include "WindowApp.h"
#include "SpriteTextureLoader.h"
#include "Input.h"

std::unique_ptr<UserInterface> UserInterface::Create()
{
	UserInterface* temp = new UserInterface;
	temp->Initialize();
	return std::unique_ptr<UserInterface>(temp);
}

void UserInterface::Initialize()
{
	//�����p�����o���X�v���C�g����
	const float	stickoutNum = 50.0f; //��ʓ��ɂ͂ݏo����
	drawerSprites[HowToPlayMenu].reset(DrawerSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DrawerSprite::Left, 0, stickoutNum, true)); //���j���[��ʈڍs esc
	drawerSprites[Hint1].reset(DrawerSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayUndo), DrawerSprite::Right, 50, stickoutNum)); //�q���g1
	drawerSprites[Hint2].reset(DrawerSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayRedo), DrawerSprite::Right, 200, stickoutNum)); //�q���g2
	drawerSprites[HowToPlayPlayer].reset(DrawerSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), DrawerSprite::Left, 500, stickoutNum)); //�v���C���[�������
	drawerSprites[HowToPlayCamera].reset(DrawerSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), DrawerSprite::Right, 500, stickoutNum)); //�J�����������


	//���j���[�p�X�v���C�g����
	for (int i = 0; i < menuframe_.size(); i++)
	{
		menuframe_[i] = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
	}
	menuBackScreen_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuBackScreen), { 0.0f, 0.0f }, false, false));
	menuBackScreen_->SetSize(Vector2(WindowApp::window_width, (WindowApp::window_height)));
}

void UserInterface::Update()
{
	if (notMove_) { return; }

	//�����p�����o���X�v���C�g�X�V
	DrawerSpriteMoveStartKey();
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Update();
	}

	//���j���[�X�v���C�g�X�V
	MenuUpdate();
	menuBackScreen_->Update();
}

void UserInterface::Draw()
{
	if (notMove_) { return; }

	//�����p�����o���X�v���C�g�`��
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Draw();
	}

	//���j���[�X�v���C�g�`��
	if (menuFlag_)
	{
		menuBackScreen_->Draw();
		for (auto& menu : menuframe_)
		{
			menu->Draw();
		}
	}
}

void UserInterface::MenuUpdate()
{
	if (!menuFlag_)
	{
		for (auto& menu : menuframe_)
		{
			menu->Reset();
		}
		easeTimer_ = 0.0f;
		return;
	}

	int count = 0;
	easeTimer_++;

	for (auto& menu : menuframe_)
	{
		if (easeTimer_ <= 5 * count)
		{
			break;
		}

		menu->Update();
		count++;
	}
}

void UserInterface::DrawerSpriteReset()
{
	//�����o���X�v���C�g�̊J��Ԃ����Z�b�g
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Reset();
	}
}

void UserInterface::DrawerSpriteMoveStartKey()
{
	//���j���[���J���Ă����ԂȂ甲����
	if (menuFlag_) { return; }

	//�L�[���͂ɂ������o���X�v���C�g�ړ��J�n
	if (Input::GetInstance()->GetInstance()->TriggerKey(DIK_1)) { drawerSprites[Hint1]->MoveStart(); }
	if (Input::GetInstance()->GetInstance()->TriggerKey(DIK_2)) { drawerSprites[Hint2]->MoveStart(); }
	if (Input::GetInstance()->GetInstance()->TriggerKey(DIK_3)) { drawerSprites[HowToPlayPlayer]->MoveStart(); }
	if (Input::GetInstance()->GetInstance()->TriggerKey(DIK_4)) { drawerSprites[HowToPlayCamera]->MoveStart(); }
}
