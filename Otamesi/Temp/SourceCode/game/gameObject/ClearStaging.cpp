#include "ClearStaging.h"
#include "WindowApp.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

std::unique_ptr<ClearStaging> ClearStaging::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	ClearStaging* temp = new ClearStaging();
	if (temp == nullptr)
	{
		return nullptr;
	}

	temp->Initialize();

	return std::unique_ptr<ClearStaging>(temp);
}

void ClearStaging::Initialize()
{
	Vector2 position = { -1000.0f, static_cast<float>(WindowApp::window_height / 2) };
	backScreen_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuBackScreen), { 0.5f, 0.5f }, false, false));
	backScreen_->SetPosition(position);

	position.x = static_cast<float>(WindowApp::window_width) + 1000.0f;
	textSprite_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuBackScreen), { 0.5f, 0.5f }, false, false));
	textSprite_->SetPosition(position);

	phase_ = static_cast<int>(MovePhase::None);

	func_.push_back([this] { return StartMove(); });
	func_.push_back([this] { return Intermediate1Move(); });
	func_.push_back([this] { return Intermediate2Move(); });
	func_.push_back([this] { return EndMove(); });
}

void ClearStaging::Update()
{
	if (phase_ != static_cast<int>(MovePhase::None))
	{
		func_[phase_]();
	}

	textSprite_->Update();
	backScreen_->Update();
}

void ClearStaging::Draw()
{
	if (phase_ == static_cast<int>(MovePhase::None))
	{
		return;
	}

	textSprite_->Draw();
	backScreen_->Draw();
}

void ClearStaging::StartMove()
{
	Vector2 position = backScreen_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, static_cast<float>((WindowApp::window_width / 2) - 5.0f), easeData_->GetTimeRate());
	backScreen_->SetPosition(position);

	position = textSprite_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, static_cast<float>((WindowApp::window_width / 2) + 5.0f), easeData_->GetTimeRate());
	textSprite_->SetPosition(position);


	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		phase_ = static_cast<int>(MovePhase::intermediate1);
	}

	// イージングデータ更新
	easeData_->Update();
}

void ClearStaging::Intermediate1Move()
{
	Vector2 position = backScreen_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, static_cast<float>(WindowApp::window_width / 2), easeData_->GetTimeRate());
	backScreen_->SetPosition(position);

	position = textSprite_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, static_cast<float>(WindowApp::window_width / 2), easeData_->GetTimeRate());
	textSprite_->SetPosition(position);


	if (easeData_->GetEndFlag() && pushFlag_)
	{
		easeData_->Reset();
		pushFlag_ = false;
		phase_ = static_cast<int>(MovePhase::intermediate2);
	}

	// イージングデータ更新
	easeData_->Update();
}

void ClearStaging::Intermediate2Move()
{
	Vector2 position = backScreen_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, static_cast<float>((WindowApp::window_width / 2) + 5.0f), easeData_->GetTimeRate());
	backScreen_->SetPosition(position);

	position = textSprite_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, static_cast<float>((WindowApp::window_width / 2) - 5.0f), easeData_->GetTimeRate());
	textSprite_->SetPosition(position);


	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		phase_ = static_cast<int>(MovePhase::End);
	}

	// イージングデータ更新
	easeData_->Update();
}

void ClearStaging::EndMove()
{
	Vector2 position = backScreen_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, 1000.0f, easeData_->GetTimeRate());
	backScreen_->SetPosition(position);

	position = textSprite_->GetPosition();
	// イージングの計算
	position.x = Easing::OutQuad(position.x, -1000.0f, easeData_->GetTimeRate());
	textSprite_->SetPosition(position);


	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		phase_ = static_cast<int>(MovePhase::None);
	}

	// イージングデータ更新
	easeData_->Update();
}
