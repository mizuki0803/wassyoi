#include "BirdManager.h"

BirdManager* BirdManager::Create(ObjModel* bodyModel, ObjModel* wingModel, const int32_t& createInterval)
{
	//�C���X�^���X����
	BirdManager* instance = new BirdManager();

	//���f�����Z�b�g
	assert(bodyModel);
	instance->bodyModel = bodyModel;
	assert(wingModel);
	instance->wingModel = wingModel;

	//�����Ԋu���Z�b�g
	instance->createInterval = createInterval;

	return instance;
}

void BirdManager::Update()
{
	//���S��Ԃ̒����폜
	birds.remove_if([](std::unique_ptr<Bird>& bird) {
		return bird->GetIsDead();
		});

	//�����^�C�}�[�X�V
	createTimer++;
	//������
	CreateBird();

	//���X�V
	for (const std::unique_ptr<Bird>& bird : birds) {
		bird->Update();
	}
}

void BirdManager::Draw()
{
	//���`��
	for (const std::unique_ptr<Bird>& bird : birds) {
		bird->Draw();
	}
}

void BirdManager::CreateBird()
{
	//�^�C�}�[�������Ԋu�܂œ��B���Ă��Ȃ���Δ�����
	if (createTimer < createInterval) { return; }
	
	//�V���Ȓ��̂��߂ɍ��W�ƈړ����x��ݒ肵�Ă���
	Vector3 pos;
	Vector3 vel;

	//�X�e�[�W�̑O�㍶�E�ǂ����Ԃ̂���ݒ�
	const FlyPosition flyPosition = static_cast<FlyPosition>((rand() % PosNum));
	//+������-�����ǂ���ɔ�s���ݒ�
	const bool isFlyDirectionPlus = static_cast<bool>(rand() % 2);

	//��s�����ɍ��킹�č��W���Z�b�g
	const float stageCenterDistance = 100.0f; //�X�e�[�W��������̋���
	if (flyPosition == FlyPosition::Forward) { pos.z = -stageCenterDistance; }
	else if (flyPosition == FlyPosition::Away) { pos.z = stageCenterDistance; }
	else if (flyPosition == FlyPosition::Left) { pos.x = -stageCenterDistance; }
	else if (flyPosition == FlyPosition::Right) { pos.x = stageCenterDistance; }	

	//�J�n�n�_��ݒ�
	float createLine = 200;	//�������̔�s�J�n���C��
	//+�����ɔ�s����ꍇ�̓}�C�i�X�ɂ���
	if (isFlyDirectionPlus) { createLine *= -1; }

	//��s�����ɍ��킹�đ��x��ݒ�
	float flySpeed = 0.2f;
	//-�����ɔ�s����ꍇ�̓}�C�i�X�ɂ���
	if (!isFlyDirectionPlus) { flySpeed *= -1; }

	//�ݒ肵���J�n�n�_�Ɣ�s���x���Z�b�g
	if (flyPosition == FlyPosition::Forward || flyPosition == FlyPosition::Away) { pos.x = createLine; vel.x = flySpeed; }
	else if (flyPosition == FlyPosition::Left || flyPosition == FlyPosition::Right) { pos.z = createLine; vel.z = flySpeed; }

	//Y���̍����������_���Őݒ�
	const float posYRandRange = 50.0f;	//�����_���͈̔�
	const float posYBase = 20.0f;	//�ŏ��l
	pos.y = (float)rand() / RAND_MAX * posYRandRange + posYBase;

	//�V���Ȓ��𐶐�
	std::unique_ptr<Bird> newBird;
	newBird.reset(Bird::Create(bodyModel, wingModel, pos, vel));
	birds.push_back(std::move(newBird));

	//�^�C�}�[�����Z�b�g
	createTimer = 0;
}
