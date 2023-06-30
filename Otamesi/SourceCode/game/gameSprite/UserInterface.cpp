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
	CreateNewDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DIK_M, DrawerSprite::Left, 0, stickoutNum, true); //���j���[��ʈڍs esc
	CreateNewDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayUndo), DIK_1, DrawerSprite::Right, 50, stickoutNum); //�q���g1
	CreateNewDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayRedo), DIK_2, DrawerSprite::Right, 200, stickoutNum); //�q���g2
	CreateNewDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), DIK_3, DrawerSprite::Left, 500, stickoutNum); //�v���C���[�������
	CreateNewDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), DIK_4, DrawerSprite::Right, 500, stickoutNum); //�J�����������


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
	//�����p�����o���X�v���C�g�X�V
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Update();
	}

	//���j���[�X�v���C�g�X�V
	MenuUpdate();
	menuBackScreen_->Update();
}

void UserInterface::Draw()
{
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

void UserInterface::CreateNewDrawerSprite(const Texture& texture, BYTE keyNumber, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//�����o���X�v���C�g����
	std::unique_ptr<DrawerSprite> newSprite;
	newSprite.reset(DrawerSprite::Create(texture, keyNumber, hidePlace, posY, stickoutNum, isOpenDrawer));
	drawerSprites.push_back(std::move(newSprite));
}
