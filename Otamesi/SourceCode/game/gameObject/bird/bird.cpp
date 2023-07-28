#include "bird.h"
#include "Easing.h"

float Bird::deadLine = 200.0f;

void (Bird::* Bird::lifePhasefuncTable[])() = {
	&Bird::BornAction,
	&Bird::DeadCheck,
	&Bird::FadeoutAction,
};

Bird* Bird::Create(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity)
{
	//�C���X�^���X����
	Bird* instance = new Bird();

	// ������
	if (!instance->Initialize(bodyModel, wingModel, position, velocity)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool Bird::Initialize(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity)
{
	//���̐���
	body.reset(BirdBody::Create(bodyModel, position, velocity));

	//��������
	wings[0].reset(BirdWing::Create(wingModel, body.get(), BirdWing::WingType::LeftWing, {}));
	wings[1].reset(BirdWing::Create(wingModel, body.get(), BirdWing::WingType::RightWing, {}));

	return true;
}

void Bird::Update()
{
	//�s��
	(this->*lifePhasefuncTable[static_cast<size_t>(lifePhase)])();

	//���̍X�V
	body->Update();

	//�����X�V
	for (const std::unique_ptr<BirdWing>& wing : wings) {
		wing->Update();
	}
}

void Bird::Draw()
{
	//���̕`��
	body->Draw();

	//�����`��
	for (const std::unique_ptr<BirdWing>& wing : wings) {
		wing->Draw();
	}
}

void Bird::BornAction()
{
	//�^�C�}�[�X�V
	actionTimer++;
	const float bornTime = 1000;	//�Y�܂��̂ɂ����鎞��
	const float time = actionTimer / bornTime;	//�C�[�W���O�p����

	//���̂Ɨ��̑傫����傫������
	const float size = Easing::InQuad(0, 1, time);
	body->SetScale({ size, size, size });

	//�^�C�}�[���w�肵�����Ԉȉ��Ȃ甲����
	if (actionTimer < bornTime) { return; }

	//�����Ă����Ԃɂ���
	lifePhase = LifePhase::Alive;
	//�^�C�}�[������
	actionTimer = 0;
}

void Bird::DeadCheck()
{
	//���W�����o��
	const Vector3 pos = body->GetPosition();
	const float posXYZ[3] = { pos.x, pos.y, pos.z };

	//x,y,z�̂��ꂼ��𔻒�
	for (int i = 0; i < 3; i++) {
		//���S���C���𒴂��Ă��Ȃ���Δ�΂�
		if (posXYZ[i] <= deadLine && posXYZ[i] >= -deadLine) { continue; }

		//�����Ă�����Ԃɂ���
		lifePhase = LifePhase::Fadeout;
		//������
		return;
	}
}

void Bird::FadeoutAction()
{
	//�^�C�}�[�X�V
	actionTimer++;
	const float fadeoutTime = 1000;	//�t�F�[�h�A�E�g�ɂ����鎞��
	const float time = actionTimer / fadeoutTime;	//�C�[�W���O�p����

	//���̂Ɨ��̑傫��������������
	const float size = Easing::OutQuad(1, 0, time);
	body->SetScale({size, size, size});

	//�^�C�}�[���w�肵�����Ԉȉ��Ȃ甲����
	if (actionTimer < fadeoutTime) { return; }

	//���S��Ԃɂ���
	isDead = true;
}
