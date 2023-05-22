#include "PlaneBlock.h"

PlaneBlock* PlaneBlock::Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& rotation)
{
	//�C���X�^���X����
	PlaneBlock* instance = new PlaneBlock();

	//���f�����Z�b�g
	assert(model);
	instance->model = model;

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	// �C�[�W���O�f�[�^�̐ݒ�
	instance->SetEaseData(60);

	//���W���Z�b�g
	Vector3 temppos = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
	temppos.y = -100.0f;
	instance->SetBlockStratPos(temppos);
	instance->SetBlockEndPos({ mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() });
	instance->position = temppos;
	//�傫�����Z�b�g
	instance->scale = { blockSize, blockSize, blockSize };
	//�p�x���Z�b�g
	instance->rotation = rotation;

	// �֐��̐ݒ�
	instance->CreateAct();

	//�u���b�N�̎�ނ��u�͂�ڂāv�ɐݒ�
	instance->blockType = BlockType::Plane;

	return instance;
}

void PlaneBlock::aaaa()
{
	color = { 1,0,0,1 };
}
