#include "birdBody.h"

BirdBody* BirdBody::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
    //�C���X�^���X����
	BirdBody* instance = new BirdBody();

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
	//�ړ����x���Z�b�g
	instance->velocity = velocity;

	return instance;
}

void BirdBody::Update()
{
	//��s�ړ�
	FlyMove();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void BirdBody::FlyMove()
{
	//���W�Ɉړ����x�����Z���Ĉړ�������
	position += velocity;

	//�ړ����x�̕����������悤�ɂ���
	rotation = Vector3::VelocityRotate(velocity);
}
