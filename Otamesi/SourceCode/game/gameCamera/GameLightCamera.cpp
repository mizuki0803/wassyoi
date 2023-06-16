#include "GameLightCamera.h"

GameLightCamera* GameLightCamera::Create(float centerDistance)
{
	//�C���X�^���X����
	GameLightCamera* instance = new GameLightCamera();

	//������
	instance->Initialize(centerDistance);

	return instance;
}

void GameLightCamera::Initialize(float centerDistance)
{
	//�J����������
	Camera::Initialize();

	//���_���Z�b�g
	up = { 0, 1, 0 };

	//���S����̋������Z�b�g
	this->centerDistance = centerDistance;
}

void GameLightCamera::Update()
{
	//�J������]
	Around();

	//�J�����X�V
	Camera::Update();
}

void GameLightCamera::Around()
{
	//��]�X�s�[�h
	const float rotNum = 0.2f;
	//��]�p���X�V
	aroundRotate += rotNum;

	//���S�_����̋��������邮����v�Z
	const float angle = XMConvertToRadians(aroundRotate);
	//�A���_�[�t���[����\��������̂ŁA�����_��؂�̂Ă�
	const float divNum = 1000;
	const float roundAngle = floorf(angle * divNum) / divNum;

	//���W�A�����g�p���Asin,cos���Z�o
	const float sinfAngle = sinf(roundAngle);
	const float cosfAngle = cosf(roundAngle);

	//�v�Z���ʂ����蓖�ĂĎ��_���Z�b�g
	Vector3 aroundPos{};
	aroundPos.x = sinfAngle * centerDistance;
	aroundPos.y = cosfAngle * centerDistance;

	SetEye(aroundPos);
}
