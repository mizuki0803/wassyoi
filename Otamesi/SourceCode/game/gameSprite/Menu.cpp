#include "Menu.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

std::unique_ptr<Menu> Menu::Create(Vector2 pos)
{
	Menu* temp = new Menu;

	temp->Initialize(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuFrame), pos, { 0.5f, 0.5f }, false, false);

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
	size = { Easing::OutBack(0.0f, 256.0f, menuEaseData_->GetTimeRate()), Easing::OutBack(0.0f, 96.0f, menuEaseData_->GetTimeRate()) };
	menuEaseData_->Update();
	//�X�v���C�g�X�V
	Sprite::Update();
}

void Menu::Reset()
{
	size = { 0.0f, 0.0f };
	menuEaseData_->Reset();
	Sprite::Update();
}
