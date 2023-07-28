#include "birdWing.h"

BirdWing* BirdWing::Create(ObjModel* model, ObjObject3d* parent, WingType wingType, const Vector3& position)
{
	//�C���X�^���X����
	BirdWing* instance = new BirdWing();

	//���f�����Z�b�g
	assert(model);
	instance->model = model;

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//�e�I�u�W�F�N�g���Z�b�g
	instance->parent = parent;
	//���̃^�C�v���Z�b�g
	instance->wingType = wingType;
	//���W���Z�b�g
	instance->position = position;

	//�����Ȃ甼��]������
	if (wingType == LeftWing) {	instance->rotation.z = 180;	}

	return instance;
}

void BirdWing::Update()
{
	//�H�΂���
	Flap();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void BirdWing::Flap()
{
	//��]�̍ő�X�s�[�h
	const float rotateMaxSpeed = 1.0f;
	//��]�̉����x
	const float rotateSpeedAccel = 0.1f;
	//���̐܂�Ԃ���]�p
	const float wingFoldRotate = 45.0f;

	//�����グ��ꍇ
	if (isWingUp) {
		//��]�X�s�[�h�ɉ����x�����Z���ĉ���������
		rotateSpeed += rotateSpeedAccel;
		rotateSpeed = min(rotateSpeed, rotateMaxSpeed);

		if (wingType == WingType::LeftWing) {
			//z���ɉ�]�X�s�[�h�����Z����
			rotation.z += rotateSpeed;

			//�܂�Ԃ��n�_�܂œ��B������܂�Ԃ�
			if (rotation.z >= 180 + wingFoldRotate) {
				isWingUp = false;
			}
		}
		else if (wingType == WingType::RightWing) {
			//z���ɉ�]�X�s�[�h�����Z����
			rotation.z += -rotateSpeed;

			//�܂�Ԃ��n�_�܂œ��B������܂�Ԃ�
			if (rotation.z <= -wingFoldRotate) {
				isWingUp = false;
			}
		}
	}
	//����������ꍇ
	else {
		//��]�X�s�[�h�ɉ����x�����Z���ĉ���������
		rotateSpeed -= rotateSpeedAccel;
		rotateSpeed = max(rotateSpeed, -rotateMaxSpeed);

		if (wingType == WingType::LeftWing) {
			//z���ɉ�]�X�s�[�h�����Z����
			rotation.z += rotateSpeed;

			//�܂�Ԃ��n�_�܂œ��B������܂�Ԃ�
			if (rotation.z <= 180 - wingFoldRotate) {
				isWingUp = true;
			}
		}
		else if (wingType == WingType::RightWing) {
			//z���ɉ�]�X�s�[�h�����Z����
			rotation.z += -rotateSpeed;

			//�܂�Ԃ��n�_�܂œ��B������܂�Ԃ�
			if (rotation.z >= wingFoldRotate) {
				isWingUp = true;
			}
		}
	}
}
