#include "UserInterface.h"
#include "WindowApp.h"
#include "SpriteTextureLoader.h"
#include "Input.h"
#include "Audio.h"
#include "HintTextureLoader.h"
#include "Easing.h"

float UserInterface::soundVolume_ = 1.0f;
const float UserInterface::soundMaxVolume_ = 2.0f;

std::unique_ptr<UserInterface> UserInterface::Create(GamePhase gamePhase)
{
	UserInterface* temp = new UserInterface;
	temp->Initialize(gamePhase);

	

	return std::unique_ptr<UserInterface>(temp);
}

void UserInterface::Common(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList)
{
	//nullptrチェック
	assert(dev);
	assert(cmdList);
	ImageUIRenderer::Common(dev, cmdList);

}

void UserInterface::Initialize(GamePhase gamePhase)
{
	const float	stickoutNum = 50.0f; //引き出しスプライトが画面内にはみ出す量
	const float drawerHandleSize = 45.0f;	//引き出しスプライトの取っ手のサイズ


	//メニュー用スプライト生成
	if (GamePhase::Title == gamePhase)
	{
		// UI用レンダラー
		imageUiRendere_.reset(ImageUIRenderer::Create());

		std::unique_ptr<Menu> tempMenu = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 2) + static_cast<float>(0 * 128)), Vector2(680.0f, 96.0f), Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuVolume), Vector2(WindowApp::window_width / 2 - 130.0f, (WindowApp::window_height / 2) + static_cast<float>(0 * 128)), { 0.5f, 0.5f }, false, false));
		menuframe_.push_back(std::move(tempMenu));
		tempMenu = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 2) + static_cast<float>(1 * 128)), Vector2(256.0f, 96.0f), Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuExit), Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 2) + static_cast<float>(1 * 128)), { 0.5f, 0.5f }, false, false));
		menuframe_.push_back(std::move(tempMenu));

		//次元変更可能確認スプライト生成
		float size = 1.0f;
		isChangeDimenisonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));
		for (auto& i : ChangeDimenisonSpriteEffect) {
			i.inst.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));
			i.inst->SetColor({ 0.005f,0.25f,0.25f,0.5f });
			i.inst->SetScale(size);
			i.isSizeUp = true;
			size += 0.002f * 50.0f;
		}
		SpriteEffectCount = 0;
		isChangeDimenison = false;
		unDoReDo.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::UnDoReDo), { WindowApp::window_width / 2 , WindowApp::window_height - 50 }));

		//説明用引き出しスプライト生成
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Husen), DIK_ESCAPE, DrawerSprite::Left, 20, stickoutNum, true); //メニュー画面移行 esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenL), DIK_3, DrawerSprite::Left, 460, stickoutNum, true); //プレイヤー操作説明
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenR), DIK_4, DrawerSprite::Right, 460, stickoutNum, true); //カメラ操作説明

		//子供スプライト生成
		//メニュー画面説明の文字
		CreateChildSprite(
			SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMenu), 
			drawerSprites[HowToPlayMenu].get(),
			{-85,10}, 
			drawerSprites[HowToPlayMenu]->GetAnchorpoint());

		// プレイヤー操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayPlayer),
			drawerSprites[HowToPlayPlayer].get(),
			{ -11,55},
			drawerSprites[HowToPlayPlayer]->GetAnchorpoint());
		
		// プレイヤー操作説明の画像
		CreateChildSprite(
			imageUiRendere_->GetTex(ImageUIRenderer::TexName::Move),
			drawerSprites[HowToPlayPlayer].get(),
			{ -(drawerSprites[HowToPlayPlayer]->GetSize().x / 2 + 25/*+ drawerHandleSize / 2*/),
			drawerSprites[HowToPlayPlayer]->GetSize().y / 2 }, { 0.5f, 0.5f });

		// カメラ操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera),
			drawerSprites[HowToPlayCamera].get(),
			{ 11,50 },
			drawerSprites[HowToPlayCamera]->GetAnchorpoint());

		// カメラ操作説明の画像
		CreateChildSprite(
			imageUiRendere_->GetTex(ImageUIRenderer::TexName::Camera),
			drawerSprites[HowToPlayCamera].get(),
			drawerSprites[HowToPlayCamera]->GetSize() / 2 + Vector2({ drawerHandleSize / 2, 0 }), { 0.5f, 0.5f });
	}
	else if (GamePhase::Selection == gamePhase)
	{

		std::unique_ptr<Menu> tempMenu = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(0 * 128) + 50.0f), Vector2(680.0f, 96.0f), Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuVolume), Vector2(WindowApp::window_width / 2 - 130.0f, (WindowApp::window_height / 3) + static_cast<float>(0 * 128) + 50.0f), { 0.5f, 0.5f }, false, false));
		menuframe_.push_back(std::move(tempMenu));
		tempMenu = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(1 * 128) + 50.0f), Vector2(256, 96.0f), Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuTitle), Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(1 * 128) + 50.0f), { 0.5f, 0.5f }, false, false));
		menuframe_.push_back(std::move(tempMenu));

		//説明用引き出しスプライト生成
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Husen), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //メニュー画面移行 esc

		//子供スプライト生成
		CreateChildSprite(
			SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMenu),
			drawerSprites[HowToPlayMenu].get(),
			{ -85,10 },
			drawerSprites[HowToPlayMenu]->GetAnchorpoint());	//メニュー画面説明の文字
		
		// ステージセレクトアロー
		stageSelectArrowSprite.reset(StageSelectArrow::Create());
	}
	else if (GamePhase::Game == gamePhase)
	{
		// UI用レンダラー
		imageUiRendere_.reset(ImageUIRenderer::Create());

		std::unique_ptr<Menu> tempMenu = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(0 * 128)), Vector2(680.0f, 96.0f), Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuVolume), Vector2(WindowApp::window_width / 2 - 130.0f, (WindowApp::window_height / 3) + static_cast<float>(0 * 128)), { 0.5f, 0.5f }, false, false));
		menuframe_.push_back(std::move(tempMenu));
		tempMenu = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(1 * 128)), Vector2(450.0f, 96.0f), Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuStageSelect), Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(1 * 128)), { 0.5f, 0.5f }, false, false));
		menuframe_.push_back(std::move(tempMenu));
		tempMenu = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(2 * 128)), Vector2(256, 96.0f), Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuTitle), Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(2 * 128)), { 0.5f, 0.5f }, false, false));
		menuframe_.push_back(std::move(tempMenu));
	
		//次元変更可能確認スプライト生成
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
		unDoReDo.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::UnDoReDo), { WindowApp::window_width / 2, WindowApp::window_height - 50 }));

		//説明用引き出しスプライト生成

		//説明用引き出しスプライト生成
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Husen), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //メニュー画面移行 esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenL), DIK_3, DrawerSprite::Left, 460, stickoutNum, false); //プレイヤー操作説明
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenR), DIK_4, DrawerSprite::Right, 460, stickoutNum, false); //カメラ操作説明
		
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenH), DIK_1, DrawerSprite::Right, 0, stickoutNum, false); //ヒント1
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HusenH), DIK_2, DrawerSprite::Right, 230, stickoutNum, false); //ヒント2

		//子供スプライト生成
		CreateChildSprite(
			SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMenu),
			drawerSprites[HowToPlayMenu].get(),
			{ -85,10 },
			drawerSprites[HowToPlayMenu]->GetAnchorpoint());

		// プレイヤー操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayPlayer),
			drawerSprites[HowToPlayPlayer].get(),
			{ -11,55 },
			drawerSprites[HowToPlayPlayer]->GetAnchorpoint());

		// プレイヤー操作説明の画像
		CreateChildSprite(
			imageUiRendere_->GetTex(ImageUIRenderer::TexName::Move),
			drawerSprites[HowToPlayPlayer].get(),
			{ -(drawerSprites[HowToPlayPlayer]->GetSize().x / 2 + 25/*+ drawerHandleSize / 2*/),
			drawerSprites[HowToPlayPlayer]->GetSize().y / 2 }, { 0.5f, 0.5f });


		// カメラ操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera),
			drawerSprites[HowToPlayCamera].get(),
			{ 11,50 },
			drawerSprites[HowToPlayCamera]->GetAnchorpoint());
		// カメラ操作説明の画像
		CreateChildSprite(
			imageUiRendere_->GetTex(ImageUIRenderer::TexName::Camera),
			drawerSprites[HowToPlayCamera].get(),
			drawerSprites[HowToPlayCamera]->GetSize() / 2 + Vector2({ drawerHandleSize / 2, 0 }), { 0.5f, 0.5f });

		// ヒント1説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(
			SpriteTextureLoader::Hint1Text),
			drawerSprites[Hint1].get(),
			{ 12,65 },
			drawerSprites[Hint1]->GetAnchorpoint());

		// ヒント2説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(
			SpriteTextureLoader::Hint2Text), 
			drawerSprites[Hint2].get(),
			{ 12,65 },
			drawerSprites[Hint2]->GetAnchorpoint());

		//ヒントスプライト生成
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

	//音量変更用スプライト生成
	soundVolumeBar = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumeBar),
		{ menuframe_[0]->GetPosition().x - 30, menuframe_[0]->GetPosition().y }, { 0.0f, 0.5f }, false, false));

	soundVolumeBar->SetSize({ 262.0f, 38.0f });
	const float soundVolumeStartPercentage = soundVolume_ / soundMaxVolume_; //最大音量と比較時の音量割合
	soundVolumePointer = std::unique_ptr<SoundVolumePointer>(SoundVolumePointer::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumePointer),
		soundVolumeBar->GetPosition(), soundVolumeBar->GetSize().x, soundVolumeStartPercentage));
}

void UserInterface::Update()
{
	if (notMove_) { return; }

	//次元変更可能確認スプライト更新
	if (isChangeDimenisonSprite) {
		SpaceEffect();
		isChangeDimenisonSprite->Update();
		unDoReDo->Update();
	}

	//説明用引き出しスプライト更新
	DrawerSpriteMoveStartKey();
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Update();
	}

	// ステージセレクトあろー更新
	if (stageSelectArrowSprite) {
		stageSelectArrowSprite->Update();
	}

	//メニュースプライト更新
	MenuUpdate();
	menuBackScreen_->Update();

	//音量変更用スプライト更新
	soundVolumeBar->Update();
	soundVolumePointer->Update();

	//子供スプライト更新
	for (const std::unique_ptr<Sprite>& childSprite : childSprites) {
		childSprite->Update();
	}
	//ヒントスプライト更新
	HintSpriteSizeChange();	//ヒントスプライトの大きさ変更
	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		if (!hintSprite) { continue; }
		hintSprite->Update();
	}

	if (imageUiRendere_ != nullptr)
	{
		imageUiRendere_->Update(
			(drawerSprites[DrawerSpriteName::HowToPlayPlayer]->GetIsOpenDrawer() ||
				drawerSprites[DrawerSpriteName::HowToPlayPlayer]->GetIsMoveDrawer()),
			(drawerSprites[DrawerSpriteName::HowToPlayCamera]->GetIsOpenDrawer() ||
				drawerSprites[DrawerSpriteName::HowToPlayCamera]->GetIsMoveDrawer())
		);
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

	//次元変更可能確認スプライト描画
	if (isChangeDimenisonSprite) {
		isChangeDimenisonSprite->Draw();
		unDoReDo->Draw();
	}

	//ステージセレクトアロー更新
	if (stageSelectArrowSprite) {
		stageSelectArrowSprite->Draw();
	}
	//説明用引き出しスプライト描画
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Draw();
	}

	//子供スプライト描画
	for (const std::unique_ptr<Sprite>& childSprite : childSprites) {
		childSprite->Draw();
	}

	//ヒントスプライト描画
	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		if (!hintSprite) { continue; }
		hintSprite->Draw();
	}

	//かさんごーせー
	SpriteCommon::GetInstance()->DrawPrev("Add");
	AddDraw();

	SpriteCommon::GetInstance()->DrawPrev();

	//メニュースプライト描画
	//hint描画
	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		if (!hintSprite) { continue; }
		hintSprite->Draw();
	}

	//メニュー描画
	if (menuFlag_)
	{
		menuBackScreen_->Draw();
		for (auto& menu : menuframe_)
		{
			menu->Draw();
		}

		//音量変更用スプライト描画
		soundVolumeBar->Draw();
		soundVolumePointer->Draw();
	}
}

void UserInterface::DrawMenuImage()
{
	if (drawerSprites[DrawerSpriteName::HowToPlayPlayer]->GetIsOpenDrawer() ||
		drawerSprites[DrawerSpriteName::HowToPlayPlayer]->GetIsMoveDrawer())
	{
		imageUiRendere_->DrawMoveDescription();
	}
	if (drawerSprites[DrawerSpriteName::HowToPlayCamera]->GetIsOpenDrawer() ||
		drawerSprites[DrawerSpriteName::HowToPlayCamera]->GetIsMoveDrawer())
	{
		imageUiRendere_->DrawCameraDescription();
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
	if (easeTimer_ == 0) {
		Audio::GetInstance()->PlayWave(Audio::SoundName::menu);
	}

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
	

	soundVolumeBar->SetSize({ Easing::OutBack(0.0f, 262.0f, menuframe_[0]->GetTimeRate()), Easing::OutBack(0.0f, 38.0f, menuframe_[0]->GetTimeRate()) });
	soundVolumePointer->SetSize({ Easing::OutQuart(0.0f, 32.0f, menuframe_[0]->GetTimeRate()), Easing::OutQuart(0.0f, 38.0f, menuframe_[0]->GetTimeRate()) });

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
		Audio::GetInstance()->PlayWave(Audio::SoundName::menu_move);

		selectionNumber_--;

		if (selectionNumber_ < 0)
		{
			selectionNumber_ = static_cast<int>(menuframe_.size() - 1);
		}
	}
	else if (Input::GetInstance()->GetInstance()->TriggerKey(DIK_DOWN))
	{
		Audio::GetInstance()->PlayWave(Audio::SoundName::menu_move);

		selectionNumber_++;

		if (selectionNumber_ >= menuframe_.size())
		{
			selectionNumber_ = 0;
		}
	}

	if (selectionNumber_ == 0)
	{
		//音量変更キー入力がある場合のみ判定
		if (Input::GetInstance()->GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->GetInstance()->PushKey(DIK_RIGHT))
		{
			const float soundVolumeChangeSpeed = 0.01f;	//音量変更量

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
			//音量変更用ポインターの割合座標を変更
			const float soundVolumeStartPercentage = soundVolume_ / soundMaxVolume_; //最大音量と比較時の音量割合
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
	float spriteVividness{}; //スプライトの色の鮮やかさ

	//次元変更可能の場合はスプライトの色を明るくする
	if (isChangeDimension) {
		spriteVividness = 1.0f;
		isChangeDimenison = true;
	}
	//次元変更可能でない場合はスプライトの色を暗くする
	else {
		spriteVividness = 0.3f;
		isChangeDimenison = false;
	}

	//鮮やかさをセット
	isChangeDimenisonSprite->SetColor({ spriteVividness, spriteVividness, spriteVividness,1 });
}

void UserInterface::StageChangeUpdate()
{
	//引き出しスプライトの開閉状態をリセット
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Reset();
	}

	//ヒントスプライトのテクスチャを次のステージ用に更新
	hintSprites[0]->SetTexture(HintTextureLoader::GetTexture(0));
	hintSprites[1]->SetTexture(HintTextureLoader::GetTexture(1));
}

bool UserInterface::GetIsHintViewMode()
{
	//どれか一つでもヒントスプライトが大きい状態 または 大きさ変更中ならtrue
	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		if (!hintSprite) { continue; }
		if (hintSprite->GetIsSizeLarger() || hintSprite->GetIsSizeChange()) { return true; }
	}

	//全てのヒントスプライトが小さい状態ならfalse
	return false;
}

void UserInterface::CreateDrawerSprite(const Texture& texture, BYTE drawerKey, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//引き出しスプライト生成
	std::unique_ptr<DrawerSprite> newSprite;
	newSprite.reset(DrawerSprite::Create(texture, drawerKey, hidePlace, posY, stickoutNum, isOpenDrawer));
	drawerSprites.push_back(std::move(newSprite));
}

void UserInterface::CreateChildSprite(const Texture& texture, Sprite* parent, const Vector2& position, const Vector2& anchorpoint)
{
	//子供スプライト生成
	std::unique_ptr<Sprite> newSprite;
	newSprite.reset(Sprite::Create(texture, position, anchorpoint));
	newSprite->SetParent(parent);
	childSprites.push_back(std::move(newSprite));
}

void UserInterface::DrawerSpriteMoveStartKey()
{
	//メニューが開いている状態なら抜ける
	if (menuFlag_) { return; }
	//ヒントスプライトを拡大表示している場合は抜ける
	if (GetIsHintViewMode()) { return; }

	//キー入力による引き出しスプライト移動開始
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		//開閉に使用するキーが入力されていなければ飛ばす
		if (!(Input::GetInstance()->GetInstance()->TriggerKey(drawerSprite->GetDrawerKey()))) { continue; }
		//エスケープキーの説明だけはキーではなく特殊な方法で開閉するので飛ばす
		Audio::GetInstance()->PlayWave(Audio::SoundName::ui_inout);

		if (drawerSprite == drawerSprites[HowToPlayMenu]) { continue; }

		//開閉開始
		drawerSprite->MoveStart();

		//そもそもヒントがなければ飛ばす
		if (drawerSprites.size() <= Hint1) { continue; }

		//開閉させるスプライトがヒント1で、開く状態を開始する場合はヒント2を全て閉じる
		if (drawerSprite == drawerSprites[Hint1] && drawerSprite->GetIsOpenDrawer()) {
			if (drawerSprites[Hint2]->GetIsOpenDrawer()) {
				drawerSprites[Hint2]->MoveStart();
			}
		}
		//開閉させるスプライトがヒント2で、開く状態を開始する場合はヒント1を全て閉じる
		if (drawerSprite == drawerSprites[Hint2] && drawerSprite->GetIsOpenDrawer()) {
			if (drawerSprites[Hint1]->GetIsOpenDrawer()) {
				drawerSprites[Hint1]->MoveStart();
			}
		}
	}
}

void UserInterface::HintSpriteSizeChange()
{
	//メニューが開いている場合は抜ける
	if (menuFlag_) { return; }
	//キー入力がされていなければ抜ける
	if (!Input::GetInstance()->TriggerKey(DIK_RETURN)) { return; }

	for (const std::unique_ptr<HintSprite>& hintSprite : hintSprites) {
		//インスタンスがなければ飛ばす
		if (!hintSprite) { continue; }
		//親の引き出しが開いている かつ 親の引き出しが移動中でない　の条件を満たせていなければ飛ばす
		if (!(hintSprite->GetParentStorage()->GetIsOpenDrawer() && !hintSprite->GetParentStorage()->GetIsMoveDrawer())) { continue; }
		//ヒントスプライトが大きさを変更中なら飛ばす
		if (hintSprite->GetIsSizeChange()) { continue; }

		//ヒントスプライトの大きさを変更
		hintSprite->SizeChangeStart();
	}
}

void UserInterface::SpaceEffect()
{
	//メニューが開いている場合は抜ける
	if (menuFlag_) { return; }

	//spaceの反応演出制御
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
