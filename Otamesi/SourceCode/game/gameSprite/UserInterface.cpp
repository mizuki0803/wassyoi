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
	const float	stickoutNum = 50.0f; //引き出しスプライトが画面内にはみ出す量

	//メニュー用スプライト生成
	if (GamePhase::Title == gamePhase)
	{
		for (int i = 0; i < 2; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//次元変更可能確認スプライト生成
		isChangeDimenisonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));

		//説明用引き出しスプライト生成
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //メニュー画面移行 esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), DIK_3, DrawerSprite::Left, 500, stickoutNum, true); //プレイヤー操作説明
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), DIK_4, DrawerSprite::Right, 500, stickoutNum, true); //カメラ操作説明

		//子供スプライト生成
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), drawerSprites[HowToPlayMenu].get(), {});	//メニュー画面説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), drawerSprites[HowToPlayPlayer].get(), {});	//プレイヤー操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), drawerSprites[HowToPlayPlayer].get(), {});	//プレイヤー操作説明の画像
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[HowToPlayCamera].get(), {});	//カメラ操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[HowToPlayCamera].get(), {});	//カメラ操作説明の画像
	}
	else if (GamePhase::Selection == gamePhase)
	{
		for (int i = 0; i < 2; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//説明用引き出しスプライト生成
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //メニュー画面移行 esc

		//子供スプライト生成
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), drawerSprites[HowToPlayMenu].get(), {});	//メニュー画面説明の文字
	}
	else if (GamePhase::Game == gamePhase)
	{
		for (int i = 0; i < 3; i++)
		{
			std::unique_ptr<Menu> temp = Menu::Create(Vector2(WindowApp::window_width / 2, (WindowApp::window_height / 3) + static_cast<float>(i * 128)));
			menuframe_.push_back(std::move(temp));
		}

		//次元変更可能確認スプライト生成
		isChangeDimenisonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayChengeDemension), { 650, WindowApp::window_width / 2 }));

		//説明用引き出しスプライト生成
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), DIK_ESCAPE, DrawerSprite::Left, 0, stickoutNum, true); //メニュー画面移行 esc
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), DIK_3, DrawerSprite::Left, 500, stickoutNum, false); //プレイヤー操作説明
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), DIK_4, DrawerSprite::Right, 500, stickoutNum, false); //カメラ操作説明
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Hint1), DIK_1, DrawerSprite::Right, 50, stickoutNum, false); //ヒント1
		CreateDrawerSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Hint2), DIK_2, DrawerSprite::Right, 247, stickoutNum, false); //ヒント2

		//子供スプライト生成
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), drawerSprites[HowToPlayMenu].get(), {});	//メニュー画面説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), drawerSprites[HowToPlayPlayer].get(), {});	//プレイヤー操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayMove), drawerSprites[HowToPlayPlayer].get(), {});	//プレイヤー操作説明の画像
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[HowToPlayCamera].get(), {});	//カメラ操作説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[HowToPlayCamera].get(), {});	//カメラ操作説明の画像
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[Hint1].get(), {});	//ヒント1説明の文字
		CreateChildSprite(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayCamera), drawerSprites[Hint2].get(), {});	//ヒント2説明の文字

		//ヒントスプライト生成
		hintSprites[0].reset(HintSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), drawerSprites[Hint1].get(), {}, {}));
		hintSprites[1].reset(HintSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::HowToPlayStageSelect), drawerSprites[Hint2].get(), {}, {}));
	}

	gamePhase_ = gamePhase;

	menuBackScreen_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuBackScreen), { 0, 0 }, { 0.0f, 0.0f }, false, false));
	menuBackScreen_->SetSize(Vector2(WindowApp::window_width, (WindowApp::window_height)));

	menuFunc_.push_back([this] { return MenuOpen(); });
	menuFunc_.push_back([this] { return MenuSelection(); });

	//音量変更用スプライト生成
	soundVolumeBar = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumeBar),
		{ menuframe_[0]->GetPosition().x + 150, menuframe_[0]->GetPosition().y }, { 0.0f, 0.5f }, false, false));
	const float soundVolumeStartPercentage = soundVolume_ / soundMaxVolume_; //最大音量と比較時の音量割合
	soundVolumePointer = std::unique_ptr<SoundVolumePointer>(SoundVolumePointer::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::SoundVolumePointer),
		soundVolumeBar->GetPosition(), soundVolumeBar->GetSize().x, soundVolumeStartPercentage));
}

void UserInterface::Update()
{
	if (notMove_) { return; }

	//次元変更可能確認スプライト更新
	if (isChangeDimenisonSprite) {
		isChangeDimenisonSprite->Update();
	}

	//説明用引き出しスプライト更新
	DrawerSpriteMoveStartKey();
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Update();
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
}

void UserInterface::Draw()
{
	if (notMove_) { return; }

	//次元変更可能確認スプライト描画
	if (isChangeDimenisonSprite) {
		isChangeDimenisonSprite->Draw();
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

	//メニュースプライト描画
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
	if (isChangeDimension) { spriteVividness = 1.0f; }
	//次元変更可能でない場合はスプライトの色を暗くする
	else { spriteVividness = 0.3f; }

	//鮮やかさをセット
	isChangeDimenisonSprite->SetColor({ spriteVividness, spriteVividness, spriteVividness,1 });
}

void UserInterface::DrawerSpriteReset()
{
	//引き出しスプライトの開閉状態をリセット
	for (const std::unique_ptr<DrawerSprite>& drawerSprite : drawerSprites) {
		drawerSprite->Reset();
	}
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
