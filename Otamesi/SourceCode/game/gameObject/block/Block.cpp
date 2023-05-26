#include "Block.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const XMINT3& mapChipNum)
{
	//�C���X�^���X����
	Block* instance = new Block();

	//����������
	if (!instance->Initialize(model, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//�u���b�N�̎�ނ��u�u���b�N�v�ɐݒ�
	instance->blockType = BlockType::Block;

	return instance;
}

bool Block::Initialize(ObjModel* model, const XMINT3& mapChipNum)
{
	//���f�����Z�b�g
	assert(model);
	this->model = model;

	//object3D�̏�����
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//���W���Z�b�g
	position = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
	//�傫�����Z�b�g
	scale = { blockSize, blockSize, blockSize };

	return true;
}
