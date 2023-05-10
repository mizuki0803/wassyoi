#include "Block.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const Vector3& position)
{
    //�C���X�^���X����
    Block* instance = new Block();

	//���f�����Z�b�g
	assert(model);
	instance->model = model;

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	instance->position = position;
	//�傫�����Z�b�g
	instance->scale = { blockSize, blockSize, blockSize };

    return instance;
}
