#include "DebugText.h"

using namespace DirectX;
using namespace Microsoft::WRL;

DebugText* DebugText::GetInstance()
{
	static DebugText instance;

	return &instance;
}

DebugText::~DebugText()
{
	//�S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++) {
		delete sprites[i];
	}
}

void DebugText::Initialize(const Texture& texture)
{
	//�S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(sprites); i++) {
		//�X�v���C�g�𐶐�����(�A���J�[�|�C���g��{ 0, 0 }�ɐݒ�)
		sprites[i] = Sprite::Create(texture, { 0, 0 });
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
	//�S�Ă̕����ɂ���
	for (int i = 0; i < text.size(); i++) {
		//�ő啶�����𒴉߂��Ă����甲����
		if (spriteIndex >= maxCharCount) { break; }

		//1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		//ASCII�R�[�h��1�i����΂����ԍ����v�Z
		const int ASCIISkipNum = 32;
		int fontIndex = character - ASCIISkipNum;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//���W�v�Z
		sprites[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y });
		sprites[spriteIndex]->SetTexLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		sprites[spriteIndex]->SetTexSize({ (float)fontWidth, (float)fontHeight });
		sprites[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });
		//�X�V
		sprites[spriteIndex]->Update();
		//������1�i�߂�
		spriteIndex++;
	}
}

//�܂Ƃ߂ĕ`��
void DebugText::DrawAll()
{
	//�S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++) {
		//�X�v���C�g�`��
		sprites[i]->Draw();
	}

	//�����������Z�b�g���Ă���
	spriteIndex = 0;
}
