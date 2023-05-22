#include "PlaneBlock.h"

PlaneBlock* PlaneBlock::Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& rotation)
{
	//�C���X�^���X����
	PlaneBlock* instance = new PlaneBlock();

	//����������
	if (!instance->Initialize(model, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//�u���b�N�̎�ނ��u�n���{�e�v�ɐݒ�
	instance->blockType = BlockType::Plane;

	return instance;
}

void PlaneBlock::aaaa()
{
	color = { 1,0,0,1 };
}
