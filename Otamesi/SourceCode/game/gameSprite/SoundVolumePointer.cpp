#include "SoundVolumePointer.h"

SoundVolumePointer* SoundVolumePointer::Create(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage)
{
	//�C���X�^���X����
	SoundVolumePointer* instance = new SoundVolumePointer();

	//������
	if (!instance->Initialize(texture, barLeftPos, barLength, startPercentage)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool SoundVolumePointer::Initialize(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage)
{
	//�X�v���C�g����
	if (!Sprite::Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//�o�[�̍��[���W���Z�b�g
	this->barLeftPos = barLeftPos;
	position = barLeftPos;
	//�o�[�̒������Z�b�g
	this->barLength = barLength;

	//�������̃p�[�Z���e�[�W�ɍ��킹�č��W���Z�b�g
	SetPercentage(startPercentage);

	return true;
}

void SoundVolumePointer::SetPercentage(float percentage)
{
	//�����ɉ����č��W��ύX
	position.x = barLeftPos.x + (barLength * percentage);
}
