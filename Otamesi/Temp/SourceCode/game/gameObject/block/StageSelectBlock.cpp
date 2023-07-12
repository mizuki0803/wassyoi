#include "StageSelectBlock.h"

StageSelectBlock* StageSelectBlock::Create(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos, ObjObject3d* parent)
{
	//�C���X�^���X����
	StageSelectBlock* instance = new StageSelectBlock();

	//����������
	if (!instance->Initialize(_blockType, mapChipNum, shiftPos, parent)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageSelectBlock::Initialize(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos, ObjObject3d* parent)
{
	//�u���b�N�̎�ނ��u�u���b�N�v�ɐݒ�
	blockType = BROCK_TYPE(_blockType);

	//���W���Z�b�g
	Vector3 pos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
	pos -= shiftPos;
	position = pos;
	//�傫�����Z�b�g
	scale = { blockSize, blockSize, blockSize };
	//�e�I�u�W�F�N�g���Z�b�g
	this->parent = parent;

	return true;
}
