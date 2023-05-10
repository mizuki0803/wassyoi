#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

using namespace DirectX;

/// <summary>
/// �J����
/// </summary>
class Camera
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// �J����������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �J�����A���O�����X�V
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// �r���[�s����X�V
	/// </summary>
	void UpdateMatView();

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	virtual void UpdateMatProjection();

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveVector(const Vector3& move);

	/// <summary>
	/// �x�N�g���ɂ�鎋�_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveEyeVector(const Vector3& move);

	/// <summary>
	/// �x�N�g���ɂ�钍���_�ړ�
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveTargetVector(const Vector3& move);


	//getter
	const Vector3& GetEye() { return eye; }
	const Vector3& GetTarget() { return target; }
	const XMMATRIX& GetMatView() { return matView; }
	const XMMATRIX& GetMatProjection() { return matProjection; }
	const XMMATRIX& GetMatBillboard() { return matBillboard; }
	const XMMATRIX& GetMatBillboardY() { return matBillboardY; }
	const Vector3& GetAngle() { return angle; }
	const float GetDistance() { return distance; }
	const XMMATRIX& GetMatWorld() { return matWorld; }

	//setter
	void SetEye(const Vector3& eye) { this->eye = eye; dirtyView = true; }
	void SetTarget(const Vector3& target) { this->target = target; dirtyView = true; }
	void SetAngle(const Vector3& angle) { this->angle = angle; dirtyAngle = true; }
	void SetDistance(const float distance) { this->distance = distance; dirtyAngle = true; }
	
protected:
	//���[���h�ϊ��s��
	XMMATRIX matWorld = {};
	// �r���[�s��
	XMMATRIX matView = XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = XMMatrixIdentity();
	//�r���{�[�h�s��
	XMMATRIX matBillboard = XMMatrixIdentity();
	//Y�����̃r���{�[�h�s��
	XMMATRIX matBillboardY = XMMatrixIdentity();
	// ���_���W
	Vector3 eye = { 0, 1, -10.0f };
	// �����_���W
	Vector3 target = { 0, 0, 0 };
	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };
	//�J�����̉�]�p
	Vector3 angle = { 0.0f, 0.0f, 0.0f };
	//target����J�����܂ł̋���
	float distance = 10.0f;
	//�_�[�e�B�t���O
	bool dirtyAngle = false;
	bool dirtyView = false;
	bool dirtyProjection = false;
};