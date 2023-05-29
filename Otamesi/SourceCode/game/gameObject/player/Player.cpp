#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "Block.h"
#include "PlayerActionManager.h"

const float Player::playerSize = 5.0f;

Player* Player::Create(ObjModel* model, const XMINT3& mapChipNum, GameCamera* gameCamera)
{
	//�C���X�^���X�𐶐�
	Player* instance = new Player();

	//���f�����Z�b�g
	assert(model);
	instance->model = model;

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//�v���C���[�ʒu��\���}�b�v�ԍ����Z�b�g
	instance->mapChipNumberPos = mapChipNum;
	//�������W���Z�b�g
	instance->SetPlayerEndPos(instance->GetMapChipPos(mapChipNum));
	Vector3 tempPos = instance->GetMapChipPos(mapChipNum);
	//�ʒu�����炵�ăC�[�W���O

	instance->position = tempPos;
	//�傫�����Z�b�g
	instance->scale = { playerSize, playerSize, playerSize };
	//�Q�[���J�������Z�b�g
	instance->gameCamera = gameCamera;

	return instance;
}

void Player::Update()
{
	//frame�ŏ��̏�����
	isMove = false;

	//�Q�[���J�����̎����ɕύX�������g���K�[�t���O��true�Ȃ�
	if (gameCamera->GetIsTriggerDimensionChange()) {
		//2������ԂȂ�A�v���C���[�̈ʒu����ʎ�O�Ɉړ�������
		if (gameCamera->GetIs2D()) {
			PlayerActionManager::PlayerFrontmost2D(mapChipNumberPos, moveSurfacePhase);
			position = GetMapChipPos(mapChipNumberPos);
		}
		//�S�[�������̂��𔻒�
		StageClearCheck();
	}

	//���W�ړ��J�n
	MovePosStart();
	//���W�ړ�
	MovePos();

	//�����؂�ւ��J�n
	ChanegeDimensionStart();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void Player::MovePosStart()
{
	//�s�����u�������Ȃ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::None)) { return; }
	//�Q�[���J�����̍s�����u�������Ȃ��v�ȊO�Ȃ甲����
	if (!(gameCamera->GetActionPhase() == GameCamera::ActionPhase::None)) { return; }
	//�ړ��L�[���͂��Ȃ���Δ�����
	if (!(Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S)
		|| Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D))) {
		return;
	}

	//�ړ��\������
	if (gameCamera->GetIs2D()) {
		if (!PlayerActionManager::PlayerMoveCheck2D(mapChipNumberPos, moveSurfacePhase, gameCamera->GetCameraXPosPhase(), gameCamera->GetCameraYPosPhase())) { return; };
	}
	else {
		if (!PlayerActionManager::PlayerMoveCheck3D(mapChipNumberPos, moveSurfacePhase, gameCamera->GetCameraXPosPhase(), gameCamera->GetCameraYPosPhase())) { return; };
	}

	//�ړ��O���W���Z�b�g
	moveBeforePos = position;
	//�ړ��ڕW���W���Z�b�g
	moveAfterPos = GetMapChipPos(mapChipNumberPos);

	//�A�N�V�����p�^�C�}�[�����������Ă���
	actionTimer = 0;

	//�s�����u���W�ړ��v�ɂ���
	actionPhase = ActionPhase::MovePos;
}

void Player::MovePos()
{
	//�s�����u���W�ړ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::MovePos)) { return; }

	//�^�C�}�[�X�V
	actionTimer++;
	const float moveTime = 30; //���W�ړ��ɂ����鎞��

	//���W�ړ��C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = actionTimer / moveTime;
	//�v���C���[���ړ�������
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (actionTimer < moveTime) { return; }

	//�ړ������̂�true
	isMove = true;

	//�S�[�������̂��𔻒�
	StageClearCheck();

	//�s�����u�������Ȃ��v�ɖ߂�
	actionPhase = ActionPhase::None;
}

void Player::ChanegeDimensionStart()
{
	//�s�����u�������Ȃ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::None)) { return; }
	//�Q�[���J�����̍s�����u�������Ȃ��v�ȊO�Ȃ甲����
	if (!(gameCamera->GetActionPhase() == GameCamera::ActionPhase::None)) { return; }
	//�L�[���͂��Ȃ����false��Ԃ�������
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//���݃J�����̌����Ă�������̖ʂɐڒn����
	MoveSurfacePhase judgeMoveSurfacePhase; //����p�ϐ�
	if (gameCamera->GetCameraYPosPhase() == (int)GameCamera::CameraYPosPhase::Top) { judgeMoveSurfacePhase = MoveSurfacePhase::Upward; }
	else if (gameCamera->GetCameraYPosPhase() == (int)GameCamera::CameraYPosPhase::Buttom) { judgeMoveSurfacePhase = MoveSurfacePhase::Downward; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Front) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingForward; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Right) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingRight; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Back) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingAway; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Left) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingLeft; }

	//���݂̃J���������Ō����ꍇ�ɁA�v���C���[�̎�O�Ƀu���b�N���Ȃ� ���� �v���C���[�̉��Ƀu���b�N�����݂��邩����
	if (!(!PlayerActionManager::DirectionForwardBlockCheck(mapChipNumberPos, judgeMoveSurfacePhase) &&
		PlayerActionManager::DirectionAwayBlockCheck(mapChipNumberPos, judgeMoveSurfacePhase))) {
		return;
	}

	//����p�Ɏg�p�����ϐ���ڒn�ʂɂ���
	moveSurfacePhase = judgeMoveSurfacePhase;

	//2D����3D�֖߂�ꍇ�A����ƂȂ�u���b�N�ɐڒn����}�b�v�ԍ��Ƀv���C���[���ړ�������
	if (gameCamera->GetIs2D()) {
		PlayerActionManager::PlayerScaffoldReturn3D(mapChipNumberPos, moveSurfacePhase);

		//�X�V�����}�b�v�ԍ��̍��W�Ɉړ�
		position = GetMapChipPos(mapChipNumberPos);
	}
	//�Q�[���J�����������؂�ւ���Ԃɂ���
	gameCamera->ChanegeDimensionStart();
}

void Player::StageClearCheck()
{
	//2������ԃS�[�������̂��𔻒�
	if (gameCamera->GetIs2D()) {
		if (PlayerActionManager::PlayerGoalCheck2D(mapChipNumberPos, moveSurfacePhase)) {
			isGoal = true;
		}
	}
	//3������ԃS�[�������̂��𔻒�
	else {
		if (PlayerActionManager::PlayerGoalCheck3D(mapChipNumberPos, moveSurfacePhase)) {
			isGoal = true;
		}
	}
}

Vector3 Player::GetMapChipPos(const XMINT3& mapChipNumberPos)
{
	return { mapChipNumberPos.x * Block::GetBlockSize(), mapChipNumberPos.y * Block::GetBlockSize(), mapChipNumberPos.z * Block::GetBlockSize() };
}

void Player::SetEaseData(const int count)
{
	if (easeData_ == nullptr)
	{
		easeData_ = std::make_unique<EaseData>(count);
	}
	else
	{
		easeData_->SetCount(count);
	}
}
