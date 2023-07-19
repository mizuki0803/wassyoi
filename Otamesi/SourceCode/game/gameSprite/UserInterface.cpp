#include "UserInterface.h"
#include "WindowApp.h"
#include "SpriteTextureLoader.h"
#include "Input.h"
#include "Audio.h"
#include "HintTextureLoader.h"

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
	const float drawerHandleSize = 45.0f;	//�����o���X�v���C�g�̎����̃T�C�Y

	//���j���[�p�X�v���C�g����
	if (GamePhase::Title == gamePhase)
	{
		for (int i = 0; i < 2; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//�����ύX�\�m�F�X�v���C�g����
		isChangeDimenisonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));
		float size = 1.0f;
		for (auto& i : ChangeDimenisonSpriteEffect) {
			i.inst.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));
			i.inst->SetColor({ 0.005f,0.25f,0.25f,1.0f });
			i.inst->SetScale(size);
			i.isSizeUp = true;
			size += 0.002f * 50.0f;
		}
		SpriteEffectCount = 0;
		isChangeDimenison = false;

		//�����p�����o���X�v���C�g����
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Husen), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //���j���[��ʈڍs esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenL), DIK_3, DrawerSprite::Left, 500, stickoutNum, true); //�v���C���[�������
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenR), DIK_4, DrawerSprite::Right, 500, stickoutNum, true); //�J�����������

		//�q���X�v���C�g����
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMenu), drawerSprites[HowToPlayMenu].get(),
			{}, drawerSprites[HowToPlayMenu]->GetAnchorpoint());	//���j���[��ʐ����̕���
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayPlayer), drawerSprites[HowToPlayPlayer].get(),
			{}, drawerSprites[HowToPlayPlayer]->GetAnchorpoint());	//�v���C���[��������̕���
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuFrame), drawerSprites[HowToPlayPlayer].get(),
			{ -(drawerSprites[HowToPlayPlayer]->GetSize().x / 2 + drawerHandleSize / 2), drawerSprites[HowToPlayPlayer]->GetSize().y / 2 }, { 0.5f, 0.5f });	//�v���C���[��������̉摜
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[HowToPlayCamera].get(),
			{}, drawerSprites[HowToPlayCamera]->GetAnchorpoint());	//�J������������̕���
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuFrame), drawerSprites[HowToPlayCamera].get(),
			drawerSprites[HowToPlayCamera]->GetSize() / 2 + Vector2({ drawerHandleSize / 2, 0 }), { 0.5f, 0.5f });	//�J������������̉摜
	}
	else if (GamePhase::Selection == gamePhase)
	{
		for (int i = 0; i < 2; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//�����p�����o���X�v���C�g����
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Husen), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //���j���[��ʈڍs esc

		//�q���X�v���C�g����
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMenu), drawerSprites[HowToPlayMenu].get(),
			{}, drawerSprites[HowToPlayMenu]->GetAnchorpoint());	//���j���[��ʐ����̕���
	}
	else if (GamePhase::Game == gamePhase)
	{
		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//�����ύX�\�m�F�X�v���C�g����
		float size = 1.0f;
		isChangeDimenisonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));
		for (auto& i : ChangeDimenisonSpriteEffect) {
			i.inst.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));
			i.inst->SetColor({ 0.005f,0.25f,0.25f,1.0f });
			i.inst->SetScale(size);
			i.isSizeUp = true;
			size += 0.002f * 50.0f;
		}
		SpriteEffectCount = 0;
		isChangeDimenison = false;

		//�����p�����o���X�v���C�g����
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Husen), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //���j���[��ʈڍs esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenL), DIK_3, DrawerSprite::Left, 500, stickoutNum, false); //�v���C���[�������
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenR), DIK_4, DrawerSprite::Right, 500, stickoutNum, false); //�J�����������
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenR), DIK_1, DrawerSprite::Right, 50, stickoutNum, false); //�q���g1
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenR), DIK_2, DrawerSprite::Right, 247, stickoutNum, false); //�q���g2

		//�q���X�v���C�g����
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMenu), drawerSprites[HowToPlayMenu].get(),
			{}, drawerSprites[HowToPlayMenu]->GetAnchorpoint());	//���j���[��ʐ����̕���
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayPlayer), drawerSprites[HowToPlayPlayer].get(),
			{}, drawerSprites[HowToPlayPlayer]->GetAnchorpoint());	//�v���C���[��������̕���
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuFrame), drawerSprites[HowToPlayPlayer].get(),
			{ -(drawerSprites[HowToPlayPlayer]->GetSize().x / 2 + drawerHandleSize / 2), drawerSprites[HowToPlayPlayer]->GetSize().y / 2 }, { 0.5f, 0.5f });	//�v���C���[��������̉摜
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[HowToPlayCamera].get(),
			{}, drawerSprites[HowToPlayCamera]->GetAnchorpoint());	//�J������������̕���
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuFrame), drawerSprites[HowToPlayCamera].get(),
			drawerSprites[HowToPlayCamera]->GetSize() / 2 + Vector2({ drawerHandleSize / 2, 0 }), { 0.5f, 0.5f });	//�J������������̉摜
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Hint1Text), drawerSprites[Hint1].get(),
			{}, drawerSprites[Hint1]->GetAnchorpoint());	//�q���g1�����̕���
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Hint2Text), drawerSprites[Hint2].get(),
			{}, drawerSprites[Hint2]->GetAnchorpoint());	//�q���g2�����̕���

		//�q���g�X�v���C�g����
		hintSprites[0].reset(HintSprite::Create(HintTextureLoader::GetTexture(0), drawerSprites[Hint1].get(),
			drawerSprites[Hint1]->GetSize() / 2 + Vector2({ drawerHandleSize / 2, 0 }), { 0.5f, 0.5f }));
		hintSprites[1].reset(HintSprite::Create(HintTextureLoader::GetTexture(1), drawerSprites[Hint2].get(),
			drawerSprites[Hint2]->GetSize() / 2 + Vector2({ drawerHandleSize / 2, 0 }), { 0.5f, 0.5f }));
	}

	gamePhase_ = gamePhase;

	menuBackScreen_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuBackScreen), { 0, 0 }, { 0.0f, 0.0f }, false, false));
	menuBackScreen_->SetSize(Vector2(WindowApp::window_width, (WindowApp::window_height)));

	menuFunc_.push_back([this] { return MenuOpen(); });
	menuFunc_.push_back([this] { return MenuSelection(); });

	//���ʕύX�p�X�v���C�g����
	soundVolumeBar = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumeBar),
		{ menuframe_[0]->GetPosition().x + 150, menuframe_[0]->GetPosition().y }, { 0.0f, 0.5f }, false, false));
	const float soundVolumeStartPercentage = soundVolume_ / soundMaxVolume_; //�ő剹�ʂƔ�r���̉��ʊ���
	soundVolumePointer = std::unique_ptr<SoundVolumePointer>(SoundVolumePointer::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumePointer),
		soundVolumeBar->GetPosition(), soundVolumeBar->GetSize().x, soundVolumeStartPercentage));
}

void UserInterface::Update()
{
	if (notMove_) { return; }

	//�����ύX�\�m�F�X�v���C�g�X�V
	if (isChangeDimenisonSprite) {
		SpaceEffect();
		isChangeDimenisonSprite->Update();
	}

	//�����p�����o���X�v���C�g�X�V
	DrawerSpriteMoveStartKey();
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Update();
	}

	//���j���[�X�v���C�g�X�V
	MenuUpdate();
	menuBackScreen_->Update();

	//���ʕύX�p�X�v���C�g�X�V
	soundVolumeBar->Update();
	soundVolumePointer->Update();

	//�q���X�v���C�g�X�V
	for (const std::unique_ptr<Sprite>& childSprite : childSprites) {
		childSprite->Update();
	}
	//�q���g�X�v���C�g�X�V
	HintSpriteSizeChange();	//�q���g�X�v���C�g�̑傫���ύX
	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		if (!hintSprite) { continue; }
		hintSprite->Update();
	}
}

void UserInterface::AddDraw()
{
	if (notMove_) { return; }

	if (isChangeDimenison) {
		for (auto& i : ChangeDimenisonSpriteEffect) {
			if (!i.isSizeUp) { continue; }
			i.inst->Draw();
		}
	}
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

	//�q���X�v���C�g�`��
	for (const std::unique_ptr<Sprite>& childSprite : childSprites) {
		childSprite->Draw();
	}
	//�q���g�X�v���C�g�`��
	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		if (!hintSprite) { continue; }
		hintSprite->Draw();
	}

	//�����񂲁[���[
	SpriteCommon::GetInstance()->DrawPrev("Add");
	AddDraw();

	SpriteCommon::GetInstance()->DrawPrev();

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
	if (isChangeDimension) {
		spriteVividness = 1.0f;
		isChangeDimenison = true;
	}
	//�����ύX�\�łȂ��ꍇ�̓X�v���C�g�̐F���Â�����
	else {
		spriteVividness = 0.3f;
		isChangeDimenison = false;
	}

	//�N�₩�����Z�b�g
	isChangeDimenisonSprite->SetColor({ spriteVividness, spriteVividness, spriteVividness,1 });
}

void UserInterface::StageChangeUpdate()
{
	//�����o���X�v���C�g�̊J��Ԃ����Z�b�g
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Reset();
	}

	//�q���g�X�v���C�g�̃e�N�X�`�������̃X�e�[�W�p�ɍX�V
	hintSprites[0]->SetTexture(HintTextureLoader::GetTexture(0));
	hintSprites[1]->SetTexture(HintTextureLoader::GetTexture(1));
}

bool UserInterface::GetIsHintViewMode()
{
	//�ǂꂩ��ł��q���g�X�v���C�g���傫����� �܂��� �傫���ύX���Ȃ�true
	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		if (!hintSprite) { continue; }
		if (hintSprite->GetIsSizeLarger() || hintSprite->GetIsSizeChange()) { return true; }
	}

	//�S�Ẵq���g�X�v���C�g����������ԂȂ�false
	return false;
}

void UserInterface::CreateDrawerSprite(const Texture& texture, BYTE drawerKey, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//�����o���X�v���C�g����
	std::unique_ptr<DrawerSprite> newSprite;
	newSprite.reset(DrawerSprite::Create(texture, drawerKey, hidePlace, posY, stickoutNum, isOpenDrawer));
	drawerSprites.push_back(std::move(newSprite));
}

void UserInterface::CreateChildSprite(const Texture& texture, Sprite* parent, const Vector2& position, const Vector2& anchorpoint)
{
	//�q���X�v���C�g����
	std::unique_ptr<Sprite> newSprite;
	newSprite.reset(Sprite::Create(texture, position, anchorpoint));
	newSprite->SetParent(parent);
	childSprites.push_back(std::move(newSprite));
}

void UserInterface::DrawerSpriteMoveStartKey()
{
	//���j���[���J���Ă����ԂȂ甲����
	if (menuFlag_) { return; }
	//�q���g�X�v���C�g���g��\�����Ă���ꍇ�͔�����
	if (GetIsHintViewMode()) { return; }

	//�L�[���͂ɂ������o���X�v���C�g�ړ��J�n
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		//�J�Ɏg�p����L�[�����͂���Ă��Ȃ���Δ�΂�
		if (!(Input::GetInstance()->GetInstance()->TriggerKey(drawerSprite->GetDrawerKey()))) { continue; }
		//�G�X�P�[�v�L�[�̐��������̓L�[�ł͂Ȃ�����ȕ��@�ŊJ����̂Ŕ�΂�
		if (drawerSprite == drawerSprites[HowToPlayMenu]) { continue; }

		Audio::GetInstance()->PlayWave(Audio::SoundName::ui_inout);

		//�J�J�n
		drawerSprite->MoveStart();

		//���������q���g���Ȃ���Δ�΂�
		if (drawerSprites.size() <= Hint1) { continue; }

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

void UserInterface::HintSpriteSizeChange()
{
	//���j���[���J���Ă���ꍇ�͔�����
	if (menuFlag_) { return; }
	//�L�[���͂�����Ă��Ȃ���Δ�����
	if (!Input::GetInstance()->TriggerKey(DIK_RETURN)) { return; }

	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		//�C���X�^���X���Ȃ���Δ�΂�
		if (!hintSprite) { continue; }
		//�e�̈����o�����J���Ă��� ���� �e�̈����o�����ړ����łȂ��@�̏����𖞂����Ă��Ȃ���Δ�΂�
		if (!(hintSprite->GetParentStorage()->GetIsOpenDrawer() && !hintSprite->GetParentStorage()->GetIsMoveDrawer())) { continue; }
		//�q���g�X�v���C�g���傫����ύX���Ȃ��΂�
		if (hintSprite->GetIsSizeChange()) { continue; }

		//�q���g�X�v���C�g�̑傫����ύX
		hintSprite->SizeChangeStart();
	}
}

void UserInterface::SpaceEffect()
{
	//���j���[���J���Ă���ꍇ�͔�����
	if (menuFlag_) { return; }

	//space�̔������o����
	bool frameUp = false;
	if (isChangeDimenison && SpriteEffectCount % 50 == 0) {
		for (auto& i : ChangeDimenisonSpriteEffect) {
			if (i.isSizeUp || frameUp) { continue; }
			i.isSizeUp = true;
			frameUp = true;
		}
	}
	for (auto& i : ChangeDimenisonSpriteEffect) {
		if (!i.isSizeUp) { continue; }
		float size = i.inst->GetScale();
		size += 0.002f;
		if (size > 1.15f) {
			size = 1.0f;
		}
		i.inst->SetScale(size);
		i.inst->Update();
	}
	SpriteEffectCount++;
	if (SpriteEffectCount > 100) {
		SpriteEffectCount = 0;
	}
}
