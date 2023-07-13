#include "GameCamera.h"
#include "Input.h"
#include "Easing.h"
#include "Player.h"
#include "Audio.h"

const float GameCamera::rotate3DDistance = 2.0f;

const DirectX::XMMATRIX GameCamera::matProj2D = XMMatrixOrthographicOffCenterLH(
	-(float)WindowApp::window_width / 18.0f, (float)WindowApp::window_width / 18.0f,
	-WindowApp::window_height / 18.0f, WindowApp::window_height / 18.0f,
	0.0f, 2000.0f
);

const DirectX::XMMATRIX GameCamera::matProj3D = XMMatrixPerspectiveFovLH(
	XMConvertToRadians(60.0f),
	(float)WindowApp::window_width / WindowApp::window_height,
	0.1f, 2000.0f
);

GameCamera* GameCamera::Create(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos)
{
	//�C���X�^���X����
	GameCamera* instance = new GameCamera();

	// ������
	instance->Initialize(distanceStageCenter, stageCenterPos);

	return instance;
}

void GameCamera::Initialize(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos)
{
	//�J����������
	Camera::Initialize();

	//�X�e�[�W��������̋������Z�b�g
	this->distanceStageCenter = distanceStageCenter;
	//�X�e�[�W�̒������W���Z�b�g
	this->stageCenterPos = stageCenterPos;

	// �ŏ��ɓ������֐��̐ݒ�
	phase_ = static_cast<int>(GamePhase::Play);
	// �C�[�W���O�̏�����
	easeData_ = std::make_unique<EaseData>(29);
	reStartEaseData_ = std::make_unique<EaseData>(29);
	//�֐��̐ݒ�
	CreateAct();

	//�����̉�]�p���Z�b�g
	rotation.x = rotate3DDistance;
	//�J�����ʒu�t�F�[�Y���X�V����
	CameraPosPhaseCheck();
}

void GameCamera::Update()
{
	func_[phase_]();
}

void GameCamera::PlayGame()
{
	//�X�e�[�W�N���A��ԂȂ甲����
	if (isStageClear || menuFlag_ || isCreateMove_ || hintFlag_) { return; }

	//�g���K�[�t���O��true��ԂȂ��false�ɒ����Ă���
	if (isTriggerDimensionChange) { isTriggerDimensionChange = false; }

	//�J������]�J�n
	RotateStart();
	//�J������]
	Rotate();

	//�����؂�ւ�
	ChanegeDimension();

	//���W�X�V
	position = UpdatePosition();

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

void GameCamera::GameStart()
{
}

void GameCamera::SetClearMode()
{
	//�N���A��Ԃɂ���
	isStageClear = true;

	//����3D�̏ꍇ�͉������Ȃ���Ԃɂ��Ĕ�����
	if (!is2D) { 
		phase_ = static_cast<int>(GamePhase::None);
		return; 
	}

	//��]�O��]�p���Z�b�g
	rotateBefore = rotation;

	//��]���]�p���Z�b�g
	if (is2D) {
		rotateAfter = { rotation.x + rotate3DDistance, rotation.y, rotation.z };
	}
	else {
		rotateAfter = { rotation.x - rotate3DDistance, rotation.y, rotation.z };
	}
	dirtyProjection = true;

	//�A�N�V�����p�^�C�}�[�����������Ă���
	actionTimer = 0;

	//�N���A����3�����ɖ߂���Ԃɂ���
	phase_ = static_cast<int>(GamePhase::ClearReturn3D);
}

void GameCamera::ClearReturn3D()
{
	//�^�C�}�[�X�V
	actionTimer++;
	const float rotTime = 40; //�����؂�ւ���]�ɂ����鎞��

	//�C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = actionTimer / rotTime;

	//��]������
	rotation.x = Easing::OutCubic(rotateBefore.x, rotateAfter.x, time);
	rotation.y = Easing::OutCubic(rotateBefore.y, rotateAfter.y, time);
	rotation.z = Easing::OutCubic(rotateBefore.z, rotateAfter.z, time);
	//���W�X�V
	position = UpdatePosition();

	//���s�ړ��s��̌v�Z
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s����X�V
	UpdateMatWorld(matTrans);
	//���_�A�����_���X�V
	UpdateEyeTarget();
	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }

	//�v���W�F�N�V�����s��̃C�[�W���O
	if (is2D) {
		matProjection = Ease4x4_out(matProj2D, matProj3D, time);
	}

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (actionTimer < rotTime) { return; }

	//2D��Ԃ��t���O��؂�ւ���
	if (is2D) { is2D = false; }
	else { is2D = true; }

	dirtyProjection = true;

	//�����ɕύX�����������g���K�[�𗧂Ă�
	isTriggerDimensionChange = true;

	//�s�����u�������Ȃ��v�ɖ߂�
	phase_ = static_cast<int>(GamePhase::None);
}

void GameCamera::ClearReturnRotate()
{
	easeData_->SetCount(29);
	//���̃X�e�[�W�J�n�̂��߁A���ʂ������悤�ɃC�[�W���O�ŉ�]������
	rotation.x = Easing::InCubic(rotateBefore.x, rotateAfter.x, easeData_->GetTimeRate());
	rotation.y = Easing::InCubic(rotateBefore.y, rotateAfter.y, easeData_->GetTimeRate());
	rotation.z = Easing::InCubic(rotateBefore.z, rotateAfter.z, easeData_->GetTimeRate());
	//�����ȍ��W���Z�o
	position = UpdatePosition();

	//���s�ړ��s��̌v�Z
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s����X�V
	UpdateMatWorld(matTrans);
	//���_�A�����_���X�V
	UpdateEyeTarget();
	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }

	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		easeData_->SetCount(39);
		phase_ = static_cast<int>(GamePhase::None);
	}

	easeData_->Update();
}

void GameCamera::GameReStart()
{	
	Vector3 moveNum;
	if (!reStartEaseChangeFlag_)
	{
		moveNum.x = Easing::OutQuint(0.0f, -28.0f, reStartEaseData_->GetTimeRate());

		if (reStartEaseData_->GetEndFlag())
		{
			reStartEaseData_->Reset();
			reStartEaseChangeFlag_ = true;
		}
	}
	else
	{
		moveNum.x = Easing::InQuint(-28.0f, 0.0f, reStartEaseData_->GetTimeRate());
	}

	//��]�p����v�Z�������W�Ɉړ��ʂ������Đ����ȍ��W���Z�o
	position = moveNum + UpdatePosition();

	//���s�ړ��s��̌v�Z
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s����X�V
	UpdateMatWorld(matTrans);
	//���_�A�����_���X�V
	UpdateEyeTarget();
	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }

	//�X�e�[�W��ύX����̂ŁA�X�e�[�W�ƃJ�����̋������C�[�W���O�ŕύX���Ă���
	distanceStageCenter.x = Easing::InCubic(beforeDistanceStageCenter.x, afterDistanceStageCenter.x, easeData_->GetTimeRate());
	distanceStageCenter.y = Easing::InCubic(beforeDistanceStageCenter.y, afterDistanceStageCenter.y, easeData_->GetTimeRate());
	distanceStageCenter.z = Easing::InCubic(beforeDistanceStageCenter.z, afterDistanceStageCenter.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag() && reStartEaseData_->GetEndFlag() && reStartEaseChangeFlag_)
	{
		easeData_->Reset();
		reStartEaseData_->Reset();
		phase_ = static_cast<int>(GamePhase::None);
	}

	reStartEaseData_->Update();
	easeData_->Update();
}

void GameCamera::StayGame()
{
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

void GameCamera::CreateAct()
{
	func_.push_back([this] { return GameStart(); });
	func_.push_back([this] { return PlayGame(); });
	func_.push_back([this] { return ClearReturn3D(); });
	func_.push_back([this] { return ClearReturnRotate(); });
	func_.push_back([this] { return GameReStart(); });
	func_.push_back([this] { return StayGame(); });
}

void GameCamera::ChanegeDimensionStart()
{
	//��]�O��]�p���Z�b�g
	rotateBefore = rotation;

	//��]���]�p���Z�b�g
	if (is2D) {
		rotateAfter = { rotation.x + rotate3DDistance, rotation.y, rotation.z };
	}
	else {
		rotateAfter = { rotation.x - rotate3DDistance, rotation.y, rotation.z };
	}
	dirtyProjection = true;

	//�A�N�V�����p�^�C�}�[�����������Ă���
	actionTimer = 0;

	//�s�����u�����؂�ւ��v�ɂ���
	actionPhase = ActionPhase::ChangeDimension;

	if (is2D) {
		Audio::GetInstance()->PlayWave(Audio::SoundName::d2_d3);
	} else {
		Audio::GetInstance()->PlayWave(Audio::SoundName::d3_d2);
	}
}

void GameCamera::SetClearResetAround()
{
	phase_ = static_cast<int>(GamePhase::ClearReturnRotate);

	//��]�O��]�p���Z�b�g
	rotateBefore = rotation;
	//��]���]�p���Z�b�g(0�܂���360�ɋ߂��ق���)
	const float aroundMax = 360;
	if (rotation.x - rotate3DDistance < aroundMax / 2) { rotateAfter.x = rotate3DDistance; }
	else { rotateAfter.x = aroundMax + rotate3DDistance; }
	if (rotation.y < aroundMax / 2) { rotateAfter.y = 0; }
	else { rotateAfter.y = aroundMax; }
	if (rotation.z < aroundMax / 2) { rotateAfter.z = 0; }
	else { rotateAfter.z = aroundMax; }
}

void GameCamera::SetReCreateMove(const XMFLOAT3& distanceStageCenter)
{
	// �ۑ�������W�̍X�V
	beforeDistanceStageCenter = this->distanceStageCenter;
	afterDistanceStageCenter = distanceStageCenter;
	phase_ = static_cast<int>(GamePhase::Play);
	is2D = false;
	isTriggerDimensionChange = false;
	cameraEaseChangeFlag_ = false;
	reStartEaseChangeFlag_ = false;
	isStageClear = false;
	easeData_->Reset();
	reStartEaseData_->Reset();
	phase_ = static_cast<int>(GamePhase::ReStart);
}

void GameCamera::Reset()
{
	cameraXPosPhase = (int)CameraXPosPhase::Front;
	cameraYPosPhase = (int)CameraYPosPhase::Side;
	phase_ = static_cast<int>(GamePhase::Play);
	is2D = false;
	isTriggerDimensionChange = false;
	cameraEaseChangeFlag_ = false;
	reStartEaseChangeFlag_ = false;
	isStageClear = false;
	easeData_->Reset();
	reStartEaseData_->Reset();
}

void GameCamera::UpdateMatProjection()
{
	if (actionPhase == ActionPhase::ChangeDimension) { return; }

	//2D��Ԃ̏ꍇ�͕��s���e
	if (is2D) {
		matProjection = matProj2D;
	}
	//3D��Ԃ̏ꍇ�͓������e
	else {
		matProjection = matProj3D;
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

Vector3 GameCamera::UpdatePosition()
{
	//X,Y��]�p�����W�A���ɒ���
	const float angleX = XMConvertToRadians(rotation.x);
	const float angleY = XMConvertToRadians(rotation.y);
	//�A���_�[�t���[����\��������̂ŁA�����_��؂�̂Ă�
	const float divNum = 1000;
	const float roundAngleX = floorf(angleX * divNum) / divNum;
	const float roundAngleY = floorf(angleY * divNum) / divNum;

	//X,Y���W�A�����g�p���Asin,cos���Z�o
	const float sinfAngleY = sinf(roundAngleY);
	const float cosfAngleY = cosf(roundAngleY);
	const float sinfAngleX = sinf(roundAngleX);
	const float cosfAngleX = cosf(roundAngleX);

	//�v�Z���ʂ����蓖�Ăč��W���Z�b�g
	//Y���W��X��]�p��sin���g�p
	//X,Z���W��Y��]�p��sin,cos�Ōv�Z���AX��]�p(Y���W)��cos����Z���ĎZ�o
	Vector3 pos;
	pos.x = (-sinfAngleY * cosfAngleX) * distanceStageCenter.x + stageCenterPos.x;
	pos.y = sinfAngleX * distanceStageCenter.y + stageCenterPos.y;
	pos.z = (-cosfAngleY * cosfAngleX) * distanceStageCenter.z + stageCenterPos.z;

	return pos;
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
	//�v���C���[�̍s�����u�������Ȃ��v�ȊO�Ȃ甲����
	if (!(player->GetActionPhase() == Player::ActionPhase::None)) { return; }
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

	//�v���W�F�N�V�����s��̃C�[�W���O
	if (is2D) {
		matProjection = Ease4x4_out(matProj2D, matProj3D, time);
	}
	else {
		matProjection = Ease4x4_in(matProj3D, matProj2D, time);
	}

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

XMMATRIX GameCamera::Ease4x4_in(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer)
{
	//4x4�ɕϊ�
	XMFLOAT4X4 a, b;
	XMStoreFloat4x4(&a, _mat1);
	XMStoreFloat4x4(&b, _mat2);

	//�C�[�W���O
	XMFLOAT4X4 out4x4 = {};
	out4x4._11 = Easing::OutQuart(a._11, b._11, _timer);
	out4x4._12 = Easing::OutQuart(a._12, b._12, _timer);
	out4x4._13 = Easing::OutQuart(a._13, b._13, _timer);
	out4x4._14 = Easing::OutQuart(a._14, b._14, _timer);
	out4x4._21 = Easing::OutQuart(a._21, b._21, _timer);
	out4x4._22 = Easing::OutQuart(a._22, b._22, _timer);
	out4x4._23 = Easing::OutQuart(a._23, b._23, _timer);
	out4x4._24 = Easing::OutQuart(a._24, b._24, _timer);
	out4x4._31 = Easing::OutQuart(a._31, b._31, _timer);
	out4x4._32 = Easing::OutQuart(a._32, b._32, _timer);
	out4x4._33 = Easing::OutQuart(a._33, b._33, _timer);
	out4x4._34 = Easing::OutQuart(a._34, b._34, _timer);
	out4x4._41 = Easing::OutQuart(a._41, b._41, _timer);
	out4x4._42 = Easing::OutQuart(a._42, b._42, _timer);
	out4x4._43 = Easing::OutQuart(a._43, b._43, _timer);
	out4x4._44 = Easing::OutQuart(a._44, b._44, _timer);

	return XMLoadFloat4x4(&out4x4);
}

XMMATRIX GameCamera::Ease4x4_out(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer)
{
	//4x4�ɕϊ�
	XMFLOAT4X4 a, b;
	XMStoreFloat4x4(&a, _mat1);
	XMStoreFloat4x4(&b, _mat2);

	XMFLOAT4X4 out4x4 = {};
	out4x4._11 = Easing::InQuart(a._11, b._11, _timer);
	out4x4._12 = Easing::InQuart(a._12, b._12, _timer);
	out4x4._13 = Easing::InQuart(a._13, b._13, _timer);
	out4x4._14 = Easing::InQuart(a._14, b._14, _timer);
	out4x4._21 = Easing::InQuart(a._21, b._21, _timer);
	out4x4._22 = Easing::InQuart(a._22, b._22, _timer);
	out4x4._23 = Easing::InQuart(a._23, b._23, _timer);
	out4x4._24 = Easing::InQuart(a._24, b._24, _timer);
	out4x4._31 = Easing::InQuart(a._31, b._31, _timer);
	out4x4._32 = Easing::InQuart(a._32, b._32, _timer);
	out4x4._33 = Easing::InQuart(a._33, b._33, _timer);
	out4x4._34 = Easing::InQuart(a._34, b._34, _timer);
	out4x4._41 = Easing::InQuart(a._41, b._41, _timer);
	out4x4._42 = Easing::InQuart(a._42, b._42, _timer);
	out4x4._43 = Easing::InQuart(a._43, b._43, _timer);
	out4x4._44 = Easing::InQuart(a._44, b._44, _timer);

	return XMLoadFloat4x4(&out4x4);
}
