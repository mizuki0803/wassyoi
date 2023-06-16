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
	backScreen_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::ClearBack), { 0.5f, 0.5f }, false, false));
	backScreen_->SetPosition(position);
	backScreen_->SetScale(0.3f);

	position.x = static_cast<float>(WindowApp::window_width) + 1000.0f;
	textSprite_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::StageClear), { 0.5f, 0.5f }, false, false));
	textSprite_->SetPosition(position);
	textSprite_->SetScale(0.3f);

	phase_ = static_cast<int>(MovePhase::None);

	easeData_ = std::make_unique<EaseData>(60);

	func_.push_back([this] { return StartMove(); });
	func_.push_back([this] { return EndMove(); });
}

void ClearStaging::Update()
{
	if (phase_ != static_cast<int>(MovePhase::None))
	{
		func_[phase_]();
	}

	backScreen_->Update();
	textSprite_->Update();
}

void ClearStaging::Draw()
{
	if (phase_ == static_cast<int>(MovePhase::None))
	{
		return;
	}

	backScreen_->Draw();
	textSprite_->Draw();
}

void ClearStaging::Reset()
{
	easeData_->Reset();
	endFlag_ = false;
}

void ClearStaging::StartMove()
{
	Vector2 position = backScreen_->GetPosition();
	// イージングの計算
	position.x = Easing::OutExpo(position.x, static_cast<float>(WindowApp::window_width / 2), easeData_->GetTimeRate());
	backScreen_->SetPosition(position);

	position = textSprite_->GetPosition();
	// イージングの計算
	position.x = Easing::OutExpo(position.x, static_cast<float>(WindowApp::window_width / 2), easeData_->GetTimeRate());
	textSprite_->SetPosition(position);


	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		easeData_->SetCount(30);
		phase_ = static_cast<int>(MovePhase::End);
	}

	// イージングデータ更新
	easeData_->Update();
}

void ClearStaging::EndMove()
{
	Vector2 position = backScreen_->GetPosition();
	// イージングの計算
	position.x = Easing::InExpo(position.x, 10000.0f, easeData_->GetTimeRate());
	backScreen_->SetPosition(position);

	position = textSprite_->GetPosition();
	// イージングの計算
	position.x = Easing::InExpo(position.x, -5000.0f, easeData_->GetTimeRate());
	textSprite_->SetPosition(position);


	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		phase_ = static_cast<int>(MovePhase::None);
		endFlag_ = true;
	}

	// イージングデータ更新
	easeData_->Update();
}
