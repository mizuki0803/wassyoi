#include "Camera.h"
#include "WindowApp.h"


void Camera::Initialize()
{
	//�J�����A���O���̍X�V
	UpdateAngle();
	//�r���[�s��̐���
	UpdateMatView();
	//�������e�ɂ��ˉe�s��̐���
	UpdateMatProjection();
}

void Camera::Update()
{
	if (dirtyAngle || dirtyView) {
		if (dirtyAngle) {
			//�J�����A���O���̍X�V
			UpdateAngle();
			//�t���O��������
			dirtyAngle = false;
		}

		//�r���[�s��̐���
		UpdateMatView();
		//�t���O��������
		dirtyView = false;
	}

	if (dirtyProjection) {
		//�������e�ɂ��ˉe�s��̐���
		UpdateMatProjection();
		//�t���O��������
		dirtyProjection = false;
	}
}

void Camera::UpdateAngle()
{
	//���_��]
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(angle.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(angle.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(angle.y));

	XMVECTOR targetEyeV = { 0.0f, 0.0f, -distance, 1.0f };
	targetEyeV = XMVector3TransformNormal(targetEyeV, matRot);

	//�J������target��Ǐ]����
	XMVECTOR targetV = { target.x, target.y, target.z, 0 };
	XMVECTOR eyeV = targetV + targetEyeV;

	//�v�Z����eyeV��eye�ɃZ�b�g����
	SetEye({ eyeV.m128_f32[0], eyeV.m128_f32[1], eyeV.m128_f32[2] });
}

void Camera::UpdateMatView()
{
	//���_���W
	XMVECTOR eyePosition = XMLoadFloat3(&XMFLOAT3(eye.x, eye.y, eye.z));
	//�����X���W
	XMVECTOR targetPosition = XMLoadFloat3(&XMFLOAT3(target.x, target.y, target.z));
	//(����)�����
	XMVECTOR upVector = XMLoadFloat3(&XMFLOAT3(up.x, up.y, up.z));

	//�J������Z��(���_����)
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//0�x�N�g�����ƌ�������܂�Ȃ��̂ŏ��O
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//�x�N�g���𐳋K��
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//�J������X��(�E����)
	XMVECTOR cameraAxisX;

	//X���͏������Z���̊O�ςŋ��܂�
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);

	//�x�N�g���𐳋K��
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//�J������X��(�����)
	XMVECTOR cameraAxisY;

	//X����Z����X���̊O�ςŋ��܂�
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//�J������]�s��
	XMMATRIX matCameraRot;
	//�J�������W�n�����[���h���W�n�̕ϊ��s��
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//�]�u�ɂ��t�s��(�t��])���v�Z
	matView = XMMatrixTranspose(matCameraRot);

	//���_���W��-1���|�������W
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//�J�����̈ʒu���烏�[���h���_�ւ̃x�N�g��(�J�������W�n)
	XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition); //X����
	XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition); //Y����
	XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition); //Z����
	//��̃x�N�g���ɂ܂Ƃ߂�
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//�r���[�s��ɕ��s�ړ�������ݒ�
	matView.r[3] = translation;

#pragma region �S�����r���{�[�h�s��̌v�Z
	//�r���{�[�h�s��
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y�����r���{�[�h�s��̌v�Z
	//�J����X���AY���AZ��
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X���͋���
	ybillCameraAxisX = cameraAxisX;
	//Y���̓��[���h���W�n��y��
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z����X����Y���̊O�ςŋ��܂�
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y�����r���{�[�h�s��
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
}

void Camera::UpdateMatProjection()
{
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),				//�㉺��p60�x
		(float)WindowApp::window_width / WindowApp::window_height,	//�A�X�y�N�g��i��ʉ��� / ��ʏc���j
		0.1f, 2000.0f							//�O�[�A���[
	);
}

void Camera::MoveVector(const Vector3& move)
{
	//���_�ƒ����_�𓯂��ʓ�����
	MoveEyeVector(move);
	MoveTargetVector(move);
}

void Camera::MoveEyeVector(const Vector3& move)
{
	Vector3 eye_moved = GetEye();

	eye_moved += move;

	SetEye(eye_moved);
}

void Camera::MoveTargetVector(const Vector3& move)
{
	Vector3 target_moved = GetTarget();

	target_moved += move;

	SetTarget(target_moved);
}
