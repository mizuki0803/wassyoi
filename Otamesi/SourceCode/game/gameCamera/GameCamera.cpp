#include "GameCamera.h"
#include "Input.h"
#include "Easing.h"

GameCamera* GameCamera::Create(float distanceStageCenter, const Vector3& stageCenterPos)
{
	//�C���X�^���X����
	GameCamera* instance = new GameCamera();

	// ������
	instance->Initialize(distanceStageCenter, stageCenterPos);

	return instance;
}

void GameCamera::Initialize(float distanceStageCenter, const Vector3& stageCenterPos)
{
	//�J����������
	Camera::Initialize();

	//�X�e�[�W��������̋������Z�b�g
	this->distanceStageCenter = distanceStageCenter;
	//�X�e�[�W�������W���Z�b�g
	this->stageCenterPos = stageCenterPos;
	SetTarget(stageCenterPos);

	//�X�e�[�W�����̍��W���J�����̊�ɂ���
	for (int i = 0; i < (int)CameraYPosPhase::YPosPhaseNum; i++) {
		for (int j = 0; j < (int)CameraXPosPhase::XPosPhaseNum; j++) {
			phaseEyePositions3D[i][j] = stageCenterPos;
			phaseEyePositions2D[i][j] = stageCenterPos;
		}
	}

	const float eyeYRatio = 0.1f; //�X�e�[�W��������̋����ɑ΂���Y���W�̊���
	const float topButtomDifference3D = 0.01f;	//���S�Ȑ^���^��{ 0, �Z�Z, 0 }�ɂ��Ă��܂��Ɛ������`�悳��Ȃ��̂ō�����ݒ�
	const float topButtomDifference2D = 0.5f;	//���S�Ȑ^���^��{ 0, �Z�Z, 0 }�ɂ��Ă��܂��Ɛ������`�悳��Ȃ��̂ō�����ݒ�

	//�X�e�[�W��������̋�������Ɏ��_�ʒu��ݒ�
	//�c���J�����ʒu��Top�̏ꍇ�̎��_�ʒu3D
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Front] += { 0, distanceStageCenter, -topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Right] += { topButtomDifference3D, distanceStageCenter, 0 };
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Back] += { 0, distanceStageCenter, topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Left] += { -topButtomDifference3D, distanceStageCenter, 0 };
	//�c���J�����ʒu��Side�̏ꍇ�̎��_�ʒu3D
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Front] += { 0, distanceStageCenter* eyeYRatio, -distanceStageCenter };
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Right] += { distanceStageCenter, distanceStageCenter* eyeYRatio, 0 };
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Back] += { 0, distanceStageCenter* eyeYRatio, distanceStageCenter };
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Left] += { -distanceStageCenter, distanceStageCenter* eyeYRatio, 0 };
	//�c���J�����ʒu��Buttom�̏ꍇ�̎��_�ʒu3D
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Front] += { 0, -distanceStageCenter, -topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Right] += { topButtomDifference3D, -distanceStageCenter, 0 };
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Back] += { 0, -distanceStageCenter, topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Left] += { -topButtomDifference3D, -distanceStageCenter, 0 };

	//�c���J�����ʒu��Top�̏ꍇ�̎��_�ʒu2D
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Front] += { 0, distanceStageCenter, -topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Right] += { topButtomDifference2D, distanceStageCenter, 0 };
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Back] += { 0, distanceStageCenter, topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Left] += { -topButtomDifference2D, distanceStageCenter, 0 };
	//�c���J�����ʒu��Side�̏ꍇ�̎��_�ʒu2D
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Front] += { 0, 0, -distanceStageCenter };
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Right] += { distanceStageCenter, 0, 0 };
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Back] += { 0, 0, distanceStageCenter };
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Left] += { -distanceStageCenter, 0, 0 };
	//�c���J�����ʒu��Buttom�̏ꍇ�̎��_�ʒu2D
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Front] += { 0, -distanceStageCenter, -topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Right] += { topButtomDifference2D, -distanceStageCenter, 0 };
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Back] += { 0, -distanceStageCenter, topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Left] += { -topButtomDifference2D, -distanceStageCenter, 0 };

	//�J�n���̎��_���Z�b�g(X:��O Y:��)
	SetEye(phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Front]);
}

void GameCamera::Update()
{
	//�g���K�[�t���O��true��ԂȂ��false�ɒ����Ă���
	if (isTriggerDimensionChange) { isTriggerDimensionChange = false; }

	//�J�����ʒu�ړ��J�n
	ChanegeCameraPosStart();
	//�J�����ʒu�ړ�
	ChanegeCameraPos();

	//�����؂�ւ�
	ChanegeDimension();

	//�J�����X�V
	Camera::Update();
}

void GameCamera::ChanegeDimensionStart()
{
	//2D��Ԃ��t���O��؂�ւ���
	if (is2D) {
		//is2D = false;
		//�ړ��ڕW���_���Z�b�g
		moveAfterEye = phaseEyePositions3D[cameraYPosPhase][cameraXPosPhase];
	}
	else {
		//is2D = true;
		//�ړ��ڕW���_���Z�b�g
		moveAfterEye = phaseEyePositions2D[cameraYPosPhase][cameraXPosPhase];
	}
	dirtyProjection = true;

	//�ړ��O���_���Z�b�g
	moveBeforeEye = eye;

	//�A�N�V�����p�^�C�}�[�����������Ă���
	actionTimer = 0;

	//�s�����u�����؂�ւ��v�ɂ���
	actionPhase = ActionPhase::ChangeDimension;
}

void GameCamera::UpdateMatProjection()
{
	//2D��Ԃ̏ꍇ�͕��s���e
	if (is2D) {
		matProjection = XMMatrixOrthographicOffCenterLH(
			-128, 128,
			-72, 72,
			0.1f, 2000.0f
		);
	}
	//3D��Ԃ̏ꍇ�͓������e
	else {
		Camera::UpdateMatProjection();
	}
}

void GameCamera::ChanegeCameraPosStart()
{
	//�s�����u�������Ȃ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::None)) { return; }
	//2D��ԂȂ甲����
	if (is2D) { return; }

	//�����ړ����c�����J�n���Ȃ���Δ�����
	if (!(ChanegeCameraXPosStart() || ChanegeCameraYPosStart())) { return; }

	//�ړ��O���_���Z�b�g
	moveBeforeEye = eye;
	//�ړ��ڕW���_���Z�b�g
	moveAfterEye = phaseEyePositions3D[cameraYPosPhase][cameraXPosPhase];

	//�A�N�V�����p�^�C�}�[�����������Ă���
	actionTimer = 0;

	//�s�����u���_�ړ��v�ɂ���
	actionPhase = ActionPhase::MoveEye;
}

bool GameCamera::ChanegeCameraXPosStart()
{
	//�c���J�����ʒu�t�F�[�Y���uSide�v�łȂ���Δ�����
	//if (!(cameraYPosPhase == (int)CameraYPosPhase::Side)) { return false; }
	//�ړ��L�[���͂��Ȃ����false��Ԃ�������
	if (!(Input::GetInstance()->TriggerKey(DIK_LEFT) || Input::GetInstance()->TriggerKey(DIK_RIGHT))) { return false; }

	//�L�[���͂ɉ����ăX�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y��ύX
	if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		//�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y��0(Front)�̏ꍇ�͈�����������ɂ��āA3(Left)�ɂ��Ă���
		if (cameraXPosPhase == (int)CameraXPosPhase::Front) { cameraXPosPhase = (int)CameraXPosPhase::Left; }
		//����ȊO�̏ꍇ�͌��݂���J�����ʒu���獶�Ɉړ�������
		else { cameraXPosPhase--; }
	}
	else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		//�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y��3(Left)�̏ꍇ�͈�����������ɂ��āA0(Front)�ɂ��Ă���
		if (cameraXPosPhase == (int)CameraXPosPhase::Left) { cameraXPosPhase = (int)CameraXPosPhase::Front; }
		//����ȊO�̏ꍇ�͌��݂���J�����ʒu����E�Ɉړ�������
		else { cameraXPosPhase++; }
	}

	//�����J�����ړ����J�n����̂�true��Ԃ�
	return true;
}

bool GameCamera::ChanegeCameraYPosStart()
{
	//�ړ��L�[���͂��Ȃ����false��Ԃ�������
	if (!(Input::GetInstance()->TriggerKey(DIK_UP) || Input::GetInstance()->TriggerKey(DIK_DOWN))) { return false; }

	//�L�[���͂ɉ����ăX�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y��ύX
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		//�X�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y��0(Top)�̏ꍇ�͂���ȏ��������Ȃ��̂ŁAfalse��Ԃ�������
		if (cameraYPosPhase == (int)CameraYPosPhase::Top) {
			if (cameraXPosPhase == (int)CameraXPosPhase::Front) { cameraXPosPhase = (int)CameraXPosPhase::Back; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Right) { cameraXPosPhase = (int)CameraXPosPhase::Left; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Back) { cameraXPosPhase = (int)CameraXPosPhase::Front; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Left) { cameraXPosPhase = (int)CameraXPosPhase::Right; }

			cameraYPosPhase = (int)CameraYPosPhase::Side;
		}
		//����ȊO�̏ꍇ�͌��݂���J�����ʒu�����Ɉړ�������
		else { cameraYPosPhase--; }
	}
	else if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		//�X�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y��2(Buttom)�̏ꍇ�͂���ȏ㉺�������Ȃ��̂ŁAfalse��Ԃ�������
		if (cameraYPosPhase == (int)CameraYPosPhase::Buttom) {
			if (cameraXPosPhase == (int)CameraXPosPhase::Front) { cameraXPosPhase = (int)CameraXPosPhase::Back; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Right) { cameraXPosPhase = (int)CameraXPosPhase::Left; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Back) { cameraXPosPhase = (int)CameraXPosPhase::Front; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Left) { cameraXPosPhase = (int)CameraXPosPhase::Right; }

			cameraYPosPhase = (int)CameraYPosPhase::Side;
		}
		//����ȊO�̏ꍇ�͌��݂���J�����ʒu���牺�Ɉړ�������
		else { cameraYPosPhase++; }
	}

	//�����J�����ړ����J�n����̂�true��Ԃ�
	return true;
}

void GameCamera::ChanegeCameraPos()
{
	//�s�����u���_�ړ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::MoveEye)) { return; }

	//�^�C�}�[�X�V
	actionTimer++;
	const float moveTime = 30; //���_�ړ��ɂ����鎞��

	//���_�ړ��C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = actionTimer / moveTime;
	//���_���ړ�������
	Vector3 easeEye;
	easeEye.x = Easing::OutQuad(moveBeforeEye.x, moveAfterEye.x, time);
	easeEye.y = Easing::OutQuad(moveBeforeEye.y, moveAfterEye.y, time);
	easeEye.z = Easing::OutQuad(moveBeforeEye.z, moveAfterEye.z, time);
	SetEye(easeEye);

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (actionTimer < moveTime) { return; }

	//�s�����u�������Ȃ��v�ɖ߂�
	actionPhase = ActionPhase::None;
}

void GameCamera::ChanegeDimension()
{
	//�s�����u�����؂�ւ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::ChangeDimension)) { return; }

	//�^�C�}�[�X�V
	actionTimer++;
	const float moveTime = 40; //�����؂�ւ��ړ��ɂ����鎞��

	//�C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = actionTimer / moveTime;
	//���_���ړ�������
	Vector3 easeEye;
	easeEye.x = Easing::OutCubic(moveBeforeEye.x, moveAfterEye.x, time);
	easeEye.y = Easing::OutCubic(moveBeforeEye.y, moveAfterEye.y, time);
	easeEye.z = Easing::OutCubic(moveBeforeEye.z, moveAfterEye.z, time);
	SetEye(easeEye);

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (actionTimer < moveTime) { return; }

	//2D��Ԃ��t���O��؂�ւ���
	if (is2D) { is2D = false; }
	else { is2D = true; }

	dirtyProjection = true;

	//�����ɕύX�����������g���K�[�𗧂Ă�
	isTriggerDimensionChange = true;

	//�s�����u�������Ȃ��v�ɖ߂�
	actionPhase = ActionPhase::None;
}
