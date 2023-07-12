#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "Block.h"
#include "PlayerActionManager.h"


const float Player::playerSize = 3.5f;

Player* Player::Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& shiftPos, GameCamera* gameCamera, ObjModel* effectModel)
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

	// �֐��̐ݒ�
	instance->CreateAct();
	// �C�[�W���O�̐ݒ�
	instance->easeData_ = std::make_unique<EaseData>(60);
	//�v���C���[�ʒu��\���}�b�v�ԍ����Z�b�g
	instance->mapChipNumberPos = mapChipNum;
	//�}�b�v�̒��S�����炷�l���Z�b�g
	instance->shiftPos = shiftPos;
	//�������W���Z�b�g
	instance->SetPlayerEndPos(instance->GetMapChipPos(mapChipNum));
	Vector3 tempPos = instance->GetMapChipPos(mapChipNum);
	//�ʒu�����炵�ăC�[�W���O
	instance->playerEndPos_ = tempPos;
	tempPos.y -= 100.0f;
	instance->playerStratPos_ = tempPos;
	//�傫�����Z�b�g
	instance->scale = { playerSize, playerSize, playerSize };
	//�Q�[���J�������Z�b�g
	instance->gameCamera = gameCamera;
	//�ŏ��̈ړ����I��������̃t���O
	instance->isStartMove = false;

	// �G�t�F�N�g�ǂݍ���
	for (int i = 0; i < instance->effect.size(); ++i)
	{
		instance->effect[i].reset(PlayerEffect::Create(effectModel, static_cast<float>(i)));
	}

	return instance;
}

void Player::Update()
{
	//�s��
	func_[phase_]();

	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();

	// �G�t�F�N�g�X�V
	for (auto& e : effect)
	{
		e->Update(this);
	}
}

void Player::ReCreate(const XMINT3& mapChipNum, const Vector3& shiftPos)
{
	SetGamePhase(GamePhase::ReStart);

	//�}�b�v�̒��S�����炷�l���Z�b�g
	this->shiftPos = shiftPos;
	//�v���C���[�ʒu��\���}�b�v�ԍ����Z�b�g
	mapChipNumberPos = mapChipNum;
	//�������W���Z�b�g
	SetPlayerEndPos(GetMapChipPos(mapChipNum));
	Vector3 tempPos = GetMapChipPos(mapChipNum);
	//�ʒu�����炵�ăC�[�W���O
	playerStratPos_ = position;
	playerEndPos_ = tempPos;

	easeData_->Reset();
}

void Player::PlayGame()
{
	//�S�[�����Ă��Ȃ��Ƃ��ɓ���������
	if (isGoal || menuFlag_ || isCreateMove_) {
		return;
	}
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
}

void Player::GameStart()
{
	// �C�[�W���O�̌v�Z
	position.x = Easing::OutBack(playerStratPos_.x, playerEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::OutBack(playerStratPos_.y, playerEndPos_.y, easeData_->GetTimeRate());
	position.z = Easing::OutBack(playerStratPos_.z, playerEndPos_.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		isStartMove = true;
		phase_ = static_cast<int>(GamePhase::GamePlay);
	}
	easeData_->Update();
}

void Player::GameReStart()
{
	// �C�[�W���O�̌v�Z
	position.x = Easing::InCubic(playerStratPos_.x, playerEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::InCubic(playerStratPos_.y, playerEndPos_.y, easeData_->GetTimeRate());
	position.z = Easing::InCubic(playerStratPos_.z, playerEndPos_.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		phase_ = static_cast<int>(GamePhase::GamePlay);
	}
	easeData_->Update();
}

void Player::CreateAct()
{
	func_.push_back([this] { return PlayGame(); });
	func_.push_back([this] { return GameStart(); });
	func_.push_back([this] { return GameReStart(); });
}

void Player::Draw()
{
	
	ObjObject3d::Draw();

	isStartMove = false;
}

void Player::EffectDraw()
{
	ObjObject3d::DrawAddPrev();
	
	for (auto &e : effect)
	{
		e->Draw();
	}
}
void Player::Reset()
{
	//�J�n��Ԃɖ߂����߂Ƀt���O�Ȃǂ����Z�b�g
	isGoal = false;
	phase_ = static_cast<int>(GamePhase::GamePlay);
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

			//2D����3D�֖߂�ꍇ�A����ƂȂ�u���b�N�ɐڒn����}�b�v�ԍ��Ƀv���C���[���ړ�������
			PlayerActionManager::PlayerScaffoldReturn3D(mapChipNumberPos, moveSurfacePhase);
			//�X�V�����}�b�v�ԍ��̍��W�Ɉړ�
			position = GetMapChipPos(mapChipNumberPos);
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
	Vector3 mapChipPos = { mapChipNumberPos.x * Block::GetBlockSize(), mapChipNumberPos.y * Block::GetBlockSize(), mapChipNumberPos.z * Block::GetBlockSize() };
	return mapChipPos - shiftPos;
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
