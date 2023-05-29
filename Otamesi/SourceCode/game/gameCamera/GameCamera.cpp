#include "GameCamera.h"
#include "Input.h"
#include "Easing.h"

const float GameCamera::rotate3DDistance = 2.0f;

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
	//�X�e�[�W�̒������W���Z�b�g
	this->stageCenterPos = stageCenterPos;

	//�����̉�]�p���Z�b�g
	rotation.x = rotate3DDistance;
	//�J�����ʒu�t�F�[�Y���X�V����
	CameraPosPhaseCheck();
}

void GameCamera::Update()
{
	//�g���K�[�t���O��true��ԂȂ��false�ɒ����Ă���
	if (isTriggerDimensionChange) { isTriggerDimensionChange = false; }

	//�J������]�J�n
	RotateStart();
	//�J������]
	Rotate();

	//�����؂�ւ�
	ChanegeDimension();

	//���W�X�V
	UpdatePosition();

	//���s�ړ��s��̌v�Z
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s����X�V
	UpdateMatWorld(matTrans);
	//���_�A�����_���X�V
	UpdateEyeTarget();
	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }
}

void GameCamera::ChanegeDimensionStart()
{
	//��]�O��]�p���Z�b�g
	rotateBefore = rotation;

	//��]���]�p���Z�b�g
	if (is2D) { rotateAfter = { rotation.x + rotate3DDistance, rotation.y, rotation.z }; }
	else { rotateAfter = { rotation.x - rotate3DDistance, rotation.y, rotation.z }; }
	dirtyProjection = true;

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

void GameCamera::UpdateMatWorld(const XMMATRIX& matTrans)
{
	//��]�@
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//�q�ł��鎩�@�p�̃��[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
}

void GameCamera::UpdateEyeTarget()
{
	//���_�����[���h���W�ɐݒ�
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//�J�����̉�]�𔽉f������
	forward = MatrixTransformDirection(forward, matWorld);
	//���_����O���ɐi�񂾈ʒu�𒍎��_�ɐݒ�
	target = eye + forward;
	//�V�n�����]���Ă������悤�ɏ�����x�N�g������]������
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);
}

void GameCamera::UpdatePosition()
{
	//X,Y��]�p�����W�A���ɒ���
	const double angleX = XMConvertToRadians(rotation.x);
	const double angleY = XMConvertToRadians(rotation.y);
	//�A���_�[�t���[����\��������̂ŁA�����_��؂�̂Ă�
	const double divNum = 1000;
	const double roundAngleX = floor(angleX * divNum) / divNum;
	const double roundAngleY = floor(angleY * divNum) / divNum;

	//X,Y���W�A�����g�p���Asin,cos���Z�o
	const double sinfAngleY = sin(roundAngleY);
	const double cosfAngleY = cos(roundAngleY);
	const double sinfAngleX = sin(roundAngleX);
	const double cosfAngleX = cos(roundAngleX);

	//�v�Z���ʂ����蓖�Ăč��W���Z�b�g
	//Y���W��X��]�p��sin���g�p
	//X,Z���W��Y��]�p��sin,cos�Ōv�Z���AX��]�p(Y���W)��cos����Z���ĎZ�o
	position.x = (float)(-sinfAngleY * cosfAngleX) * distanceStageCenter + stageCenterPos.x;
	position.y = (float)sinfAngleX * distanceStageCenter + stageCenterPos.y;
	position.z = (float)(-cosfAngleY * cosfAngleX) * distanceStageCenter + stageCenterPos.z;
}

Vector3 GameCamera::InputRotateNum()
{
	//��]�p�ɉ��Z����l
	Vector3 addRot;
	//��x�̉�]�ŉ���
	const float rotNum = 90;

	//�L�[���͂ɂ����ʂ�ݒ�
	if (Input::GetInstance()->PushKey(DIK_UP)) { addRot = { rotNum, 0, 0 }; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { addRot = { -rotNum, 0, 0 }; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { addRot = { 0, rotNum, 0 }; }
	else if (Input::GetInstance()->PushKey(DIK_RIGHT)) { addRot = { 0, -rotNum, 0 }; }

	//�c���J�����ʒu�t�F�[�Y���t���̉��܂��͂Ȃ�΍��E���t�ɂȂ�̂Ŕ��]�����Ă���
	if (cameraYPosPhase == CameraYPosPhase::ReverseSide || cameraYPosPhase == CameraYPosPhase::Buttom) { addRot.y = -addRot.y; }

	//���݂̉�]�p�ɉ��Z����ʂ𑫂����l��Ԃ�
	return rotation + addRot;
}

void GameCamera::RotateStart()
{
	//�s�����u�������Ȃ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::None)) { return; }
	//2D��ԂȂ甲����
	if (is2D) { return; }

	//���͂��Ȃ���Δ�����
	if (!(Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_RIGHT))) { return; }

	//��]�O��]�p���Z�b�g
	rotateBefore = rotation;
	//��]���]�p���Z�b�g
	rotateAfter = InputRotateNum();

	//�A�N�V�����p�^�C�}�[�����������Ă���
	actionTimer = 0;

	//�s�����u�J������]�v�ɂ���
	actionPhase = ActionPhase::Rotation;
}

void GameCamera::Rotate()
{
	//�s�����u�J������]�v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::Rotation)) { return; }

	//�^�C�}�[�X�V
	actionTimer++;
	//��]�C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = (float)actionTimer / rotateTime;

	//��]������
	rotation.x = Easing::OutQuart(rotateBefore.x, rotateAfter.x, time);
	rotation.y = Easing::OutQuart(rotateBefore.y, rotateAfter.y, time);
	rotation.z = Easing::OutQuart(rotateBefore.z, rotateAfter.z, time);

	//��]�p��0�`360�ȓ��Ɏ��܂�悤�ɒ���
	MaxMinRotate(rotation.x);
	MaxMinRotate(rotation.y);
	MaxMinRotate(rotation.z);

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (actionTimer < rotateTime) { return; }

	//�J�����ʒu�t�F�[�Y���X�V����
	CameraPosPhaseCheck();

	//�s�����u�������Ȃ��v�ɖ߂�
	actionPhase = ActionPhase::None;
}

void GameCamera::MaxMinRotate(float& rotation)
{
	//0�`360�ȓ��Ɏ��܂�悤�ɒ���
	const float rotMax = 360.0f;
	if (rotation >= rotMax) {
		rotation -= rotMax;
	}
	else if (rotation < 0) {
		rotation += rotMax;
	}
}

void GameCamera::CameraPosPhaseCheck()
{
	//3D�ł̎��_���ɃJ�������X����ʂ��������p�x
	const float rotX = rotation.x - rotate3DDistance;

	//X����]�ɂ��c���J�����ʒu�t�F�[�Y��ݒ�
	if ((int)(rotX) == 0) { cameraYPosPhase = CameraYPosPhase::Side; }
	else if ((int)(rotX) == 90) { cameraYPosPhase = CameraYPosPhase::Top; }
	else if ((int)(rotX) == 180) { cameraYPosPhase = CameraYPosPhase::ReverseSide; }
	else if ((int)(rotX) == 270) { cameraYPosPhase = CameraYPosPhase::Buttom; }
	else { assert(0); }

	//Y����]�ɂ�鉡���J�����ʒu�t�F�[�Y��ݒ�
	if ((int)(rotX) == 0 || (int)(rotX) == 90 || (int)(rotX) == 270) {
		if ((int)rotation.y == 0) { cameraXPosPhase = CameraXPosPhase::Front; }
		else if ((int)rotation.y == 90) { cameraXPosPhase = CameraXPosPhase::Left; }
		else if ((int)rotation.y == 180) { cameraXPosPhase = CameraXPosPhase::Back; }
		else if ((int)rotation.y == 270) { cameraXPosPhase = CameraXPosPhase::Right; }
		else { assert(0); }
	}
	else if ((int)(rotX) == 180) {//X����]��180�x�������Ă���Ƃ��݂̂͏㉺���t�ɂȂ�
		if ((int)rotation.y == 0) { cameraXPosPhase = CameraXPosPhase::Back; }
		else if ((int)rotation.y == 90) { cameraXPosPhase = CameraXPosPhase::Right; }
		else if ((int)rotation.y == 180) { cameraXPosPhase = CameraXPosPhase::Front; }
		else if ((int)rotation.y == 270) { cameraXPosPhase = CameraXPosPhase::Left; }
		else { assert(0); }
	}
	else {
		assert(0);
	}
}

void GameCamera::ChanegeDimension()
{
	//�s�����u�����؂�ւ��v�ȊO�Ȃ甲����
	if (!(actionPhase == ActionPhase::ChangeDimension)) { return; }

	//�^�C�}�[�X�V
	actionTimer++;
	const float rotTime = 40; //�����؂�ւ���]�ɂ����鎞��

	//�C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = actionTimer / rotTime;

	//��]������
	rotation.x = Easing::OutCubic(rotateBefore.x, rotateAfter.x, time);
	rotation.y = Easing::OutCubic(rotateBefore.y, rotateAfter.y, time);
	rotation.z = Easing::OutCubic(rotateBefore.z, rotateAfter.z, time);

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (actionTimer < rotTime) { return; }

	//2D��Ԃ��t���O��؂�ւ���
	if (is2D) { is2D = false; }
	else { is2D = true; }

	dirtyProjection = true;

	//�����ɕύX�����������g���K�[�𗧂Ă�
	isTriggerDimensionChange = true;

	//�s�����u�������Ȃ��v�ɖ߂�
	actionPhase = ActionPhase::None;
}

void GameCamera::CameraSetMove()
{
	// InOut��Quint
	SetEye({});

	easeData_->Update();

	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		easeData_->SetCount(30);
		phase_ = 2;
	}
}

void GameCamera::GamePlayStratCameraSetMove()
{
	SetTarget({}); //�v���C���[�̍��W

	if (!cameraEaseChangeFlag_)
	{
		// InOut��Quad
		SetEye({});

		if (easeData_->GetEndFlag())
		{
			easeData_->Reset();
			easeData_->SetCount(20);
			cameraEaseChangeFlag_ = true;
		}
	}
	else
	{
		// InOut��Quad
		SetEye({});

		if (easeData_->GetEndFlag())
		{
			isShake_ = true;
		}
	}

	ShakeMove();
}

void GameCamera::ShakeMove()
{
	if (isShake_ && !shakeEnd_)
	{
		shakeTimer_++;

		Vector3 shake = {};
		shake.x = static_cast<float>(rand() % (5 - attenuation_) - 2);
		shake.y = static_cast<float>(rand() % (5 - attenuation_) - 2) + 5.0f;
		shake.z = 15.0f;

		if (shakeTimer_ >= attenuation_ * 2)
		{
			// �����l�̉��Z
			attenuation_ += 1;
			SetEye(shake);
		}
		else if (attenuation_ >= 4)
		{
			shakeTimer_ = 0;
			attenuation_ = 0;
			shakeEnd_ = true;
			SetEye({ 0,5,15 });
			// ����
			phase_ = 3;
		}
	}
}

void GameCamera::SetEaseData(const int count)
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
