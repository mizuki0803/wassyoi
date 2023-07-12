#include "UserInterface.h"
#include "WindowApp.h"
#include "SpriteTextureLoader.h"
#include "Input.h"
#include "Audio.h"

float UserInterface::soundVolume_ = 1.0f;
const float UserInterface::soundMaxVolume_ = 2.0f;

std::unique_ptr<UserInterface> UserInterface::Create(GamePhase gamePhase)
{
	UserInterface* temp = new UserInterface;
	temp->Initialize(gamePhase);
	return std::unique_ptr<UserInterface>(temp);
}

void UserInterface::Initialize(GamePhase gamePhase)
{
	const float	stickoutNum = 50.0f; //�����o���X�v���C�g����ʓ��ɂ͂ݏo����

	//���j���[�p�X�v���C�g����
	if (GamePhase::Title == gamePhase)
	{
		for (int i = 0; i < 2; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//�����ύX�\�m�F�X�v���C�g����
		isChangeDimenisonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension)));
		isChangeDimenisonSprite->SetPosition({ 650, WindowApp::window_width / 2 });

		//�����p�����o���X�v���C�g����
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //���j���[��ʈڍs esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), DIK_3, DrawerSprite::Left, 500, stickoutNum, true); //�v���C���[�������
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), DIK_4, DrawerSprite::Right, 500, stickoutNum, true); //�J�����������

		/*childSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Number)));
		childSprite->SetParent(isChangeDimenisonSprite.get());
		childSprite->SetPosition({ 200, -50 });*/
	}
	else if (GamePhase::Selection == gamePhase)
	{
		for (int i = 0; i < 2; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//�����p�����o���X�v���C�g����
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //���j���[��ʈڍs esc
	}
	else if (GamePhase::Game == gamePhase)
	{
		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//�����ύX�\�m�F�X�v���C�g����
		isChangeDimenisonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension)));
		isChangeDimenisonSprite->SetPosition({ 650, WindowApp::window_width / 2 });

		//�����p�����o���X�v���C�g����
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //���j���[��ʈڍs esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Tips1), DIK_1, DrawerSprite::Right, 50, stickoutNum, false); //�q���g1
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Tips2), DIK_2, DrawerSprite::Right, 247, stickoutNum, false); //�q���g2
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), DIK_3, DrawerSprite::Left, 500, stickoutNum, false); //�v���C���[�������
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), DIK_4, DrawerSprite::Right, 500, stickoutNum, false); //�J�����������
	}

	gamePhase_ = gamePhase;

	menuBackScreen_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuBackScreen), { 0.0f, 0.0f }, false, false));
	menuBackScreen_->SetSize(Vector2(WindowApp::window_width, (WindowApp::window_height)));

	menuFunc_.push_back([this] { return MenuOpen(); });
	menuFunc_.push_back([this] { return MenuSelection(); });

	//���ʕύX�p�X�v���C�g����
	soundVolumeBar = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumeBar), { 0.0f, 0.5f }, false, false));
	soundVolumeBar->SetPosition({ menuframe_[0]->GetPosition().x + 150, menuframe_[0]->GetPosition().y });
	const float soundVolumeStartPercentage = soundVolume_ / soundMaxVolume_; //�ő剹�ʂƔ�r���̉��ʊ���
	soundVolumePointer = std::unique_ptr<SoundVolumePointer>(SoundVolumePointer::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumePointer),
		soundVolumeBar->GetPosition(), soundVolumeBar->GetSize().x, soundVolumeStartPercentage));
}

void UserInterface::Update()
{
	if (notMove_) { return; }

	//�����ύX�\�m�F�X�v���C�g�X�V
	if (isChangeDimenisonSprite) {
		isChangeDimenisonSprite->Update();
	}

	//�����p�����o���X�v���C�g�X�V
	DrawerSpriteMoveStartKey();
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Update();
	}
	//�����o���̎q���X�v���C�g�X�V
	//childSprite->Update();

	//���j���[�X�v���C�g�X�V
	MenuUpdate();
	menuBackScreen_->Update();

	//���ʕύX�p�X�v���C�g�X�V
	soundVolumeBar->Update();
	soundVolumePointer->Update();
}

void UserInterface::Draw()
{
	if (notMove_) { return; }

	//�����ύX�\�m�F�X�v���C�g�`��
	if (isChangeDimenisonSprite) {
		isChangeDimenisonSprite->Draw();
	}

	//�����p�����o���X�v���C�g�`��
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Draw();
	}
	//�����o���̎q���X�v���C�g�`��
	//childSprite->Draw();

	//���j���[�X�v���C�g�`��
	if (menuFlag_)
	{
		menuBackScreen_->Draw();
		for (auto& menu : menuframe_)
		{
			menu->Draw();
		}

		//���ʕύX�p�X�v���C�g�`��
		soundVolumeBar->Draw();
		soundVolumePointer->Draw();
	}
}

void UserInterface::MenuUpdate()
{
	if (!menuFlag_)
	{
		MenuReset();
		return;
	}

	for (int i = 0; i < menuframe_.size(); i++)
	{
		if (i == selectionNumber_)
		{
			menuframe_[i]->SetColor({ 1,0,0,1 });
		}
		else
		{
			menuframe_[i]->SetColor({ 1,1,1,1 });
		}
	}

	menuFunc_[menuPhase_]();
}

void UserInterface::MenuReset()
{
	for (auto& menu : menuframe_)
	{
		menu->Reset();
	}
	easeTimer_ = 0.0f;
	selectionNumber_ = 0;
	menuPhase_ = static_cast<int>(MenuPhase::Start);
}

void UserInterface::MenuOpen()
{
	int count = 0;
	easeTimer_++;
	selectionNumber_ = 0;

	for (auto& menu : menuframe_)
	{
		if (easeTimer_ <= 5 * count)
		{
			break;
		}

		menu->Update();
		count++;
	}

	bool hitFlag = false;
	for (auto& menu : menuframe_)
	{
		if (!menu->EaseEnd())
		{
			hitFlag = true;
		}
	}

	if (!hitFlag)
	{
		menuPhase_ = static_cast<int>(MenuPhase::Selection);
	}
}

void UserInterface::MenuSelection()
{
	if (Input::GetInstance()->GetInstance()->TriggerKey(DIK_UP))
	{
		selectionNumber_--;

		if (selectionNumber_ < 0)
		{
			selectionNumber_ = static_cast<int>(menuframe_.size() - 1);
		}
	}
	else if (Input::GetInstance()->GetInstance()->TriggerKey(DIK_DOWN))
	{
		selectionNumber_++;

		if (selectionNumber_ >= menuframe_.size())
		{
			selectionNumber_ = 0;
		}
	}

	if (selectionNumber_ == 0)
	{
		//���ʕύX�L�[���͂�����ꍇ�̂ݔ���
		if (Input::GetInstance()->GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->GetInstance()->PushKey(DIK_RIGHT))
		{
			const float soundVolumeChangeSpeed = 0.01f;	//���ʕύX��

			if (Input::GetInstance()->GetInstance()->PushKey(DIK_LEFT))
			{
				soundVolume_ -= soundVolumeChangeSpeed;
				soundVolume_ = max(soundVolume_, 0.0f);
			}
			else if (Input::GetInstance()->GetInstance()->PushKey(DIK_RIGHT))
			{
				soundVolume_ += soundVolumeChangeSpeed;
				soundVolume_ = min(soundVolume_, soundMaxVolume_);
			}
			//���ʕύX�p�|�C���^�[�̊������W��ύX
			const float soundVolumeStartPercentage = soundVolume_ / soundMaxVolume_; //�ő剹�ʂƔ�r���̉��ʊ���
			soundVolumePointer->SetPercentage(soundVolumeStartPercentage);

			Audio::GetInstance()->ChangeVolume(soundVolume_);
		}
	}

	for (auto& menu : menuframe_)
	{
		menu->Update();
	}
}

void UserInterface::IsChangeDimensionCheck(bool isChangeDimension)
{
	float spriteVividness{}; //�X�v���C�g�̐F�̑N�₩��

	//�����ύX�\�̏ꍇ�̓X�v���C�g�̐F�𖾂邭����
	if (isChangeDimension) { spriteVividness = 1.0f; }
	//�����ύX�\�łȂ��ꍇ�̓X�v���C�g�̐F���Â�����
	else { spriteVividness = 0.3f; }

	//�N�₩�����Z�b�g
	isChangeDimenisonSprite->SetColor({ spriteVividness, spriteVividness, spriteVividness,1 });
}

void UserInterface::DrawerSpriteReset()
{
	//�����o���X�v���C�g�̊J��Ԃ����Z�b�g
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Reset();
	}
}

void UserInterface::CreateDrawerSprite(const Texture& texture, BYTE drawerKey, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//�����o���X�v���C�g����
	std::unique_ptr<DrawerSprite> newSprite;
	newSprite.reset(DrawerSprite::Create(texture, drawerKey, hidePlace, posY, stickoutNum, isOpenDrawer));
	drawerSprites.push_back(std::move(newSprite));
}

void UserInterface::DrawerSpriteMoveStartKey()
{
	//���j���[���J���Ă����ԂȂ甲����
	if (menuFlag_) { return; }

	//�L�[���͂ɂ������o���X�v���C�g�ړ��J�n
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		//�J�Ɏg�p����L�[�����͂���Ă��Ȃ���Δ�΂�
		if (!(Input::GetInstance()->GetInstance()->TriggerKey(drawerSprite->GetDrawerKey()))) { continue; }
		//�G�X�P�[�v�L�[�̐��������̓L�[�ł͂Ȃ�����ȕ��@�ŊJ����̂Ŕ�΂�
		if (drawerSprite == drawerSprites[HowToPlayMenu]) { continue; }

		//�J�J�n
		drawerSprite->MoveStart();

		//�J������X�v���C�g���q���g1�ŁA�J����Ԃ��J�n����ꍇ�̓q���g2��S�ĕ���
		if (drawerSprite == drawerSprites[Hint1] && drawerSprite->GetIsOpenDrawer()) {
			if (drawerSprites[Hint2]->GetIsOpenDrawer()) {
				drawerSprites[Hint2]->MoveStart();
			}
		}
		//�J������X�v���C�g���q���g2�ŁA�J����Ԃ��J�n����ꍇ�̓q���g1��S�ĕ���
		if (drawerSprite == drawerSprites[Hint2] && drawerSprite->GetIsOpenDrawer()) {
			if (drawerSprites[Hint1]->GetIsOpenDrawer()) {
				drawerSprites[Hint1]->MoveStart();
			}
		}
	}
}
