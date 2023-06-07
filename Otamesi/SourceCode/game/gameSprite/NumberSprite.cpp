#include "NumberSprite.h"

NumberSprite* NumberSprite::Create(const Texture& texture, const Vector2& position, const Vector2& size, const Vector2& texSize)
{
	//�����X�v���C�g�̃C���X�^���X�𐶐�
	NumberSprite* numberSprite = new NumberSprite();
	if (numberSprite == nullptr) {
		return nullptr;
	}

	// ������
	if (!numberSprite->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete numberSprite;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	numberSprite->position = position;

	//�傫�����Z�b�g
	numberSprite->size = size;
	numberSprite->texSize = texSize;

	return numberSprite;
}

void NumberSprite::SetNumber(const int number)
{
	//�X�v���C�g�̍���؂�o�����W���e�N�X�`���T�C�Y * �����ŎZ�o����
	texLeftTop.x = texSize.x * number;
}
