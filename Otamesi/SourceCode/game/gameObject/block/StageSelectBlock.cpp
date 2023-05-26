#include "StageSelectBlock.h"

StageSelectBlock* StageSelectBlock::Create(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent)
{
	//�C���X�^���X����
	StageSelectBlock* instance = new StageSelectBlock();

	//����������
	if (!instance->Initialize(_blockType, mapChipNum, mapSize, parent)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageSelectBlock::Initialize(const int _blockType, const XMINT3& mapChipNum, const XMINT3& mapSize, ObjObject3d* parent)
{
	//�u���b�N�̎�ނ��u�u���b�N�v�ɐݒ�
	blockType = BROCK_TYPE(_blockType);

	//���W���Z�b�g
	Vector3 pos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
	pos -= { blockSize / 2 * (mapSize.x - 1), blockSize / 2 * (mapSize.y - 1), blockSize / 2 * (mapSize.z - 1) };
	position = pos;
	//�傫�����Z�b�g
	scale = { blockSize, blockSize, blockSize };
	//�e�I�u�W�F�N�g���Z�b�g
	this->parent = parent;

	return true;
}
