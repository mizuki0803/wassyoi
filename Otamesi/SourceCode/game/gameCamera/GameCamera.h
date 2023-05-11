#pragma once
#include "Camera.h"
#include "EaseData.h"
#include <array>
#include <memory>
#include <vector>
#include <functional>

/// <summary>
/// �Q�[���J����
/// </summary>
class GameCamera : public Camera 
{
public: //enum
	/// <summary>
	/// �X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y
	/// </summary>
	enum class CameraXPosPhase
	{
		Front,	//��O
		Right,	//�E
		Back,	//��
		Left,	//��

		XPosPhaseNum	//�t�F�[�Y�̐�(4)
	};

	/// <summary>
	/// �X�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y
	/// </summary>
	enum class CameraYPosPhase
	{
		Top,	//��
		Side,	//��
		Buttom,	//��

		YPosPhaseNum	//�t�F�[�Y�̐�(3)
	};

	/// <summary>
	/// �s���t�F�[�Y
	/// </summary>
	enum class ActionPhase
	{
		None,			//�������Ȃ�
		MoveEye,		//���_�ړ�
		ChangeDimension,//�����؂�ւ�
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="distanceStageCenter">�X�e�[�W��������̋���</param>
	/// <param name="stageCenterPos">�X�e�[�W�������W</param>
	/// <returns>�Q�[���J����</returns>
	GameCamera* Create(float distanceStageCenter, const Vector3& stageCenterPos);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary> 
	/// <param name="distanceStageCenter">�X�e�[�W��������̋���</param>
	/// <param name="stageCenterPos">�X�e�[�W�������W</param>
	void Initialize(float distanceStageCenter, const Vector3& stageCenterPos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �����؂�ւ��J�n
	/// </summary>
	void ChanegeDimensionStart();

	//getter
	ActionPhase GetActionPhase() { return actionPhase; };
	const int GetCameraXPosPhase() { return cameraXPosPhase; };
	const int GetCameraYPosPhase() { return cameraYPosPhase; };
	const bool GetIs2D() { return is2D; }
	const bool GetIsTriggerDimensionChange() { return isTriggerDimensionChange; }

private: //�����o�֐�
	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateMatProjection() override;

	/// <summary>
	/// �J�����ʒu�ړ��J�n
	/// </summary>
	void ChanegeCameraPosStart();

	/// <summary>
	/// �����J�����ʒu�ړ��J�n
	/// </summary>
	/// <returns>�����J�����ʒu�ړ��J�n���J�n���邩</returns>
	bool ChanegeCameraXPosStart();

	/// <summary>
	/// �c���J�����ʒu�ړ��J�n
	/// </summary>
	/// <returns>�c���J�����ʒu�ړ��J�n���J�n���邩</returns>
	bool ChanegeCameraYPosStart();

	/// <summary>
	/// �J�����ʒu�ړ�
	/// </summary>
	void ChanegeCameraPos();

	/// <summary>
	/// �����؂�ւ�
	/// </summary>
	void ChanegeDimension();

	/// <summary>
	/// �J�n���̈ʒu����
	/// </summary>
	void CameraSetMove();

	void GamePlayStratCameraSetMove();
	/// <summary>
	/// �J�����̃V�F�C�N
	/// </summary>
	void ShakeMove();
	/// <summary>
	/// �C�[�W���O�f�[�^�̐ݒ�
	/// </summary>
	/// <param name="count">�t���[���̃J�E���g</param>
	void SetEaseData(const int count);

private:
	//�X�e�[�W�������W
	Vector3 stageCenterPos;
	//�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y
	int cameraXPosPhase = (int)CameraXPosPhase::Front;
	//�X�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y
	int cameraYPosPhase = (int)CameraYPosPhase::Side;
	//�X�e�[�W��������̋���
	float distanceStageCenter;
	//�J�����ʒu�t�F�[�Y�S�p�^�[���̎��_�ʒu3D
	std::array<std::array<Vector3, (int)CameraXPosPhase::XPosPhaseNum>, (int)CameraYPosPhase::YPosPhaseNum> phaseEyePositions3D;
	//�J�����ʒu�t�F�[�Y�S�p�^�[���̎��_�ʒu2D
	std::array<std::array<Vector3, (int)CameraXPosPhase::XPosPhaseNum>, (int)CameraYPosPhase::YPosPhaseNum> phaseEyePositions2D;
	//�A�N�V����
	ActionPhase actionPhase = ActionPhase::None;
	//�A�N�V�����p�^�C�}�[
	int32_t actionTimer = 0;
	//�ړ��O���_
	Vector3 moveBeforeEye;
	//�ړ��ڕW���_
	Vector3 moveAfterEye;
	//2������Ԃ�
	bool is2D = false;
	//�����ύX�����������u�Ԃ�
	bool isTriggerDimensionChange = false;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_ = 0;
	// �C�[�W���O�f�[�^
	std::unique_ptr<EaseData> easeData_;
	// �V�F�C�N���Ă��邩
	bool isShake_ = false;
	// �V�F�C�N���I�������
	bool shakeEnd_ = false;
	// �V�F�C�N�^�C�}�[
	int shakeTimer_ = 0;
	// �����l
	int attenuation_ = 0;
	// �J�����̃C�[�W���O�̐؂�ւ���
	bool cameraEaseChangeFlag_ = false;
};