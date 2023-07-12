#include "StageSelectBlockManager.h"
#include "Easing.h"

std::array<Vector3, StageSelectBlockManager::PositionNum> StageSelectBlockManager::managerPos = {
	{{0, 0, 0}, {150, 0, 0}, {-150, 0, 0}}
};

StageSelectBlockManager* StageSelectBlockManager::Create(const BlockManagerPositionPhase& positionPhase)
{
	//�C���X�^���X����
	StageSelectBlockManager* instance = new StageSelectBlockManager();

	//����������
	if (!instance->Initialize(positionPhase)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageSelectBlockManager::Initialize(const BlockManagerPositionPhase& positionPhase)
{
	//�I�u�W�F�N�g����
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//���W���Z�b�g
	position = managerPos[positionPhase];

	//���݂̏ꏊ�t�F�[�Y���Z�b�g
	blockManagerPositionPhase = positionPhase;

	//�ꏊ�t�F�[�Y�����S�ȊO�Ȃ�
	if (!positionPhase == Center) {
		//���S�Ɉړ�������
		MoveStart(Center);
	}

	return true;
}

void StageSelectBlockManager::Update()
{
	//���邭���]������
	const float rotSpeed = 0.5f;
	rotation.y += rotSpeed;

	//�ړ�
	Move();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void StageSelectBlockManager::MoveStart(const BlockManagerPositionPhase& nextPositionPhase)
{
	//���Ɉړ����Ȃ甲����
	if (isMove) { return; }

	//���Ɉړ�����ꏊ�t�F�[�Y���Z�b�g
	this->nextBlockManagerPositionPhase = nextPositionPhase;

	//�ړ��p�^�C�}�[�����������Ă���
	moveTimer = 0;
	//�ړ����ɐݒ肷��
	isMove = true;
}

void StageSelectBlockManager::Move()
{
	//�ړ���ԂłȂ��Ȃ甲����
	if (!isMove) { return; }

	//�ړ��p�^�C�}�[���X�V���Ă���
	moveTimer++;
	//�C�[�W���O�p���l���v�Z����
	const float time = moveTimer / (float)moveTime;

	//���W���X�V
	position.x = Easing::OutQuad(managerPos[blockManagerPositionPhase].x, managerPos[nextBlockManagerPositionPhase].x, time);

	//�^�C�}�[���w�肵�����Ԉȉ��Ȃ甲����
	if (moveTimer < moveTime) { return; }

	//���݂̏ꏊ�t�F�[�Y(�ړ��O)�����S�ł���΍폜�t���O�𗧂Ă�
	if (blockManagerPositionPhase == BlockManagerPositionPhase::Center) {
		isDelete = true;
	}
	//�ꏊ�t�F�[�Y�����S�ȊO�ł����
	else {
		//�ړ���p�ɐݒ肵�Ă����ꏊ�t�F�[�Y�����݂̏ꏊ�t�F�[�Y�ɐݒ�
		blockManagerPositionPhase = nextBlockManagerPositionPhase;
	}

	//�ړ���Ԃ��������Ă���
	isMove = false;
}
