#include "Menu.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

std::unique_ptr<Menu> Menu::Create(const Vector2 pos, const Vector2 size, Sprite* text)
{
	Menu* temp = new Menu;

	temp->Initialize(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuFrame), pos, { 0.5f, 0.5f }, false, false);

	temp->frameSize_ = size;

	temp->menuText_ = std::unique_ptr<Sprite>(text);

	temp->textSize_ = temp->menuText_->GetSize();

	temp->Update();

	return std::unique_ptr<Menu>(temp);
}

bool Menu::Initialize(const Texture& texture, const Vector2& position, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	//�X�v���C�g������
	if (!Sprite::Initialize(texture, position, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	//��ʑS�̂𕢂��̂ɕK�v�ȏ����Z�b�g
	size = { 0.0f, 0.0f };
	texSize = { 256, 256 };

	menuEaseData_ = std::make_unique<EaseData>(15);

	return true;
}

void Menu::Update()
{
	size = { Easing::OutBack(0.0f, frameSize_.x, menuEaseData_->GetTimeRate()), Easing::OutBack(0.0f, frameSize_.y, menuEaseData_->GetTimeRate()) };
	menuText_->SetSize(Vector2(Easing::OutBack(0.0f, textSize_.x, menuEaseData_->GetTimeRate()), Easing::OutBack(0.0f, textSize_.y, menuEaseData_->GetTimeRate())));
	menuEaseData_->Update();

	menuText_->Update();
	//�X�v���C�g�X�V
	Sprite::Update();
}

void Menu::Draw()
{
	Sprite::Draw();
	menuText_->Draw();
}

void Menu::Reset()
{
	size = { 0.0f, 0.0f };
	menuText_->SetSize(Vector2());
	menuEaseData_->Reset();
	Sprite::Update();
	menuText_->Update();
}
