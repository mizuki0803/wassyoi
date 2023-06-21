#pragma once
#include "Camera.h"
#include "EaseData.h"
#include <array>
#include <memory>
#include <vector>
#include <functional>

//�v���C���[�N���X�̑O���錾
class Player;

/// <summary>
/// �Q�[���J����
/// </summary>
class GameCamera : public Camera
{
public: //enum
	/// <summary>
	/// �X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y
	/// </summary>
	enum CameraXPosPhase
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
	enum CameraYPosPhase
	{
		Top,		//��
		Side,		//��
		Buttom,		//��
		ReverseSide,//�t�����̉�

		YPosPhaseNum	//�t�F�[�Y�̐�(3)
	};

	/// <summary>
	/// �s���t�F�[�Y
	/// </summary>
	enum class ActionPhase
	{
		None,			//�������Ȃ�
		Rotation,		//�J������]
		ChangeDimension,//�����؂�ւ�
	};

	/// <summary>
	/// �Q�[���̑S��
	/// </summary>
	enum class GamePhase
	{
		Start,	//�J�n
		Play,	//�Q�[����
		ClearReturn3D,	//�N���A��3�����ɖ߂�
		ReStart,//�ēx����
		BackBased,
		None,	//�������Ȃ�
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="distanceStageCenter">�X�e�[�W��������̋���</param>
	/// <param name="stageCenterPos">�X�e�[�W�������W</param>
	/// <returns>�Q�[���J����</returns>
	GameCamera* Create(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary> 
	/// <param name="distanceStageCenter">�X�e�[�W��������̋���</param>
	/// <param name="stageCenterPos">�X�e�[�W�������W</param>
	void Initialize(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Q�[����
	/// </summary>
	void PlayGame();
	/// <summary>
	/// �Q�[���J�n
	/// </summary>
	void GameStart();
	/// <summary>
	/// �N���A���J�n�̏���
	/// </summary>
	void SetClearMode();
	/// <summary>
	/// �N���A����3�����ɖ߂�����
	/// </summary>
	void ClearReturn3D();
	/// <summary>
	/// �Q�[���ĊJ�n
	/// </summary>
	void GameReStart();
	/// <summary>
	/// �������Ȃ�
	/// </summary>
	void StayGame();

	void BackBasedMove();
	/// <summary>
	/// �֐��̐ݒ�
	/// </summary>
	void CreateAct();

	/// <summary>
	/// �����؂�ւ��J�n
	/// </summary>
	void ChanegeDimensionStart();
	/// <summary>
	/// �Đ������ɓ��������߂̏���
	/// </summary>
	void SetReCreateMove();

	/// <summary>
	/// �J�n��Ԃɖ߂�
	/// </summary>
	void Reset();

	//getter
	ActionPhase GetActionPhase() { return actionPhase; };
	const int GetCameraXPosPhase() { return cameraXPosPhase; };
	const int GetCameraYPosPhase() { return cameraYPosPhase; };
	const bool GetIs2D() { return is2D; }
	const bool GetIsTriggerDimensionChange() { return isTriggerDimensionChange; }
	const Vector3 GetRotation() { return rotation; }
	//setter
	void SetPlayer(Player* _player) { player = _player; }
	void SetIs2D(const bool _is2D) { is2D = _is2D; }
	void SetCameraXPosPhase(const int _cameraXPosPhase) { cameraXPosPhase = _cameraXPosPhase; };
	void SetCameraYPosPhase(const int _cameraYPosPhase) { cameraYPosPhase = _cameraYPosPhase; };
	void SetRotation(const Vector3& _rotation) { rotation = _rotation; };
	void SetIsStageClear(const bool _isStageClear) { isStageClear = _isStageClear; }
	void SetGamePhase(GamePhase gamePhase) { phase_ = static_cast<int>(gamePhase); }
	void SetSaveDistanceStageCenter(XMFLOAT3 saveDistanceStageCenter) { saveDistanceStageCenter_ = saveDistanceStageCenter; }

private: //�����o�֐�
	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateMatProjection() override;

	/// <summary>
	/// ���[���h�s����X�V
	/// </summary>
	/// <param name="matTrans">���s�ړ��s��</param>
	void UpdateMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// ���_�ƒ����_���X�V
	/// </summary>
	void UpdateEyeTarget();

	/// <summary>
	/// ���W�X�V
	/// </summary>
	void UpdatePosition();

	/// <summary>
	/// ��]�J�n���̓��͂ɂ���]�����ݒ�
	/// </summary>
	/// <returns></returns>
	Vector3 InputRotateNum();

	/// <summary>
	/// ��]�J�n
	/// </summary>
	/// <param name="is2D">2������Ԃ�</param>
	void RotateStart();

	/// <summary>
	/// ��]����
	/// </summary>
	void Rotate();

	/// <summary>
	/// ��]�p���傫�������菬���������肵�Ȃ��悤�A0�`360���Ɏ��܂�p�悤�ɒ���
	/// </summary>
	/// <param name="rotation">��]�p</param>
	void MaxMinRotate(float& rotation);

	/// <summary>
	///	�J�����̉�]�p�̌X������J�����ʒu�t�F�[�Y��Ԃ�
	/// </summary>
	void CameraPosPhaseCheck();

	/// <summary>
	/// �����؂�ւ�
	/// </summary>
	void ChanegeDimension();

	/// <summary>
	/// �J�n���̈ʒu����
	/// </summary>
	void CameraSetMove();

	/// <summary>
	/// 
	/// </summary>
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

	/// <summary>
	/// 2d�ɂȂ�ۂ̃C�[�W���O
	/// </summary>
	/// <param name="_mat1">�C�[�W���O�O</param>
	/// <param name="_mat2">�C�[�W���O��</param>
	/// <param name="_timer">�C�[�W���O�p�^�C�}�[</param>
	/// <returns>���ݍ��W</returns>
	XMMATRIX Ease4x4_in(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer);

	/// <summary>
	/// 3d�ɂȂ�ۂ̃C�[�W���O
	/// </summary>
	/// <param name="_mat1">�C�[�W���O�O</param>
	/// <param name="_mat2">�C�[�W���O��</param>
	/// <param name="_timer">�C�[�W���O�p�^�C�}�[</param>
	/// <returns>���ݍ��W</returns>
	XMMATRIX Ease4x4_out(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer);


private: //�ÓI�����o�ϐ�
	//��x�̉�]�ɂ����鎞��
	static const int32_t rotateTime = 40;
	//3D�ł̎��_���ɃJ�������X�����
	static const float rotate3DDistance;
	//2d�̃v���W�F�N�V�����s��
	static const XMMATRIX matProj2D;
	//3d�̃v���W�F�N�V�����s��
	static const XMMATRIX matProj3D;

private: //�����o�ϐ�
	//�v���C���[�̏��
	Player* player;
	//��]�p
	Vector3 rotation = { 0, 0, 0 };
	//���W
	Vector3 position = { 0, 0, 0 };
	//���ۂɃJ�����̎��_,�����_,��x�N�g���Ɏg�p���郏�[���h�ϊ��s��
	XMMATRIX cameraMatWorld = {};
	//�X�e�[�W�������W
	Vector3 stageCenterPos;
	//�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y
	int cameraXPosPhase = (int)CameraXPosPhase::Front;
	//�X�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y
	int cameraYPosPhase = (int)CameraYPosPhase::Side;
	//�X�e�[�W��������̋���
	XMFLOAT3 distanceStageCenter;
	//�A�N�V����
	ActionPhase actionPhase = ActionPhase::None;
	//�A�N�V�����p�^�C�}�[
	int32_t actionTimer = 0;
	//��]�O��]�p
	Vector3 rotateBefore;
	//��]���]�p
	Vector3 rotateAfter;
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
	// �C�[�W���O�p�ɕۑ�������W
	// �J�n�ʒu
	Vector3 stratPos_ = {};
	// �I���ʒu
	Vector3 endPos_ = {};
	//
	XMFLOAT3 saveDistanceStageCenter_;
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
	//�X�e�[�W�N���A��Ԃ�
	bool isStageClear = false;
};