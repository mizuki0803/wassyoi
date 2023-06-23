#include "Skydome.h"

const float Skydome::size = 10.0f;
const float Skydome::rotSpeed = 4.3f;

Skydome* Skydome::Create(ObjModel* model)
{
	//�C���X�^���X�𐶐�
	Skydome* instance = new Skydome();

	//���f�����Z�b�g
	assert(model);
	instance->model = model;

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//�傫�����Z�b�g
	instance->scale = { size, size, size };

	return instance;
}

void Skydome::Update()
{
	//��]
	Rotate();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void Skydome::Rotate()
{
	//��]��ԂłȂ���Δ�����
	if (!isRotate) { return; }

	rotation.y += -rotSpeed;
}
