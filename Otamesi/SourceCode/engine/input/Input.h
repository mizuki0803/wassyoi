#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800	//DirectInput�̃o�[�W�����w��
#include <dinput.h>
#include <DirectXMath.h>

#include "WindowApp.h"

/// <summary>
/// ����
/// </summary>
class Input final
{
public: //�G�C���A�X
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //��
	//�}�E�X�{�^��
	enum MouseButton
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_WHEEL
	};
	//�Q�[���p�b�h�{�^��
	enum GamePadButton
	{
		PAD_A,
		PAD_B,
		PAD_X,
		PAD_Y,
		PAD_LB,
		PAD_RB,
		PAD_SELECT,
		PAD_START,
		PAD_LSTICK,
		PAD_RSTICK,
		PAD_LEFT,
		PAD_RIGHT,
		PAD_UP,
		PAD_DOWN,
	};

private: //�V���O���g����
	//�R���X�g���N�^���B��
	Input() = default;
	//�f�X�g���N�^���B��
	~Input() = default;
public:
	//�R�s�[�R���X�g���N�^�𖳌���
	Input(const Input& input) = delete;
	//������Z�q�𖳌���
	void operator = (const Input& input) = delete;

public: //�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>����</returns>
	static Input* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="win">�E�C���h�E�A�v���P�[�V����</param>
	void Initialize(WindowApp* win);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[�̉������`�F�b�N
	/// </summary>
	/// <param name = "keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>������Ă��邩</returns>
	bool PushKey(const BYTE keyNumber);

	/// <summary>
	/// �L�[�̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name = "keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerKey(const BYTE keyNumber);

	/// <summary>
	/// �L�[�𗣂������`�F�b�N
	/// </summary>
	/// <param name = "keyNumber">�L�[�ԍ�( DIK_0 ��)</param>
	/// <returns>��������</returns>
	bool ReleaseKey(const BYTE keyNumber);

	/// <summary>
	/// �}�E�X�{�^���̉������`�F�b�N
	/// </summary>
	/// <param name = "mouseButton">�}�E�X�{�^��</param>
	/// <returns>������Ă��邩</returns>
	bool PushMouseButton(const int mouseButton);

	/// <summary>
	/// �}�E�X�{�^���̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name = "mouseButton">�}�E�X�{�^��</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseButton(const int mouseButton);

	/// <summary>
	/// �}�E�X�{�^���𗣂������`�F�b�N
	/// </summary>
	/// <param name = "mouseButton">�}�E�X�{�^��</param>
	/// <returns>��������</returns>
	bool ReleaseMouseButton(const int mouseButton);

	/// <summary>
	/// �}�E�X�|�C���^�[�̍��W���擾
	/// </summary>
	/// <returns>�}�E�X�|�C���^�[�̍��W</returns>
	DirectX::XMFLOAT2 GetMousePoint();

	/// <summary>
	/// 1�t���[���̃}�E�X�̈ړ��ʂ̎擾
	/// </summary>
	/// <returns>1�t���[���̃}�E�X�̈ړ���</returns>
	DirectX::XMFLOAT2 GetMouseVelocity();

	/// <summary>
	/// 1�t���[���̃}�E�X�z�C�[���̈ړ��ʂ̎擾
	/// </summary>
	/// <returns>1�t���[���̃}�E�X�z�C�[���̈ړ���</returns>
	float GetMouseWheelVelocity();

	/// <summary>
	/// �Q�[���p�b�h�{�^���̉������`�F�b�N
	/// </summary>
	/// <param name = "gamePadButton">�Q�[���p�b�h�{�^��</param>
	/// <returns>������Ă��邩</returns>
	bool PushGamePadButton(const int gamePadButton);

	/// <summary>
	/// �Q�[���p�b�h�{�^���̃g���K�[���`�F�b�N
	/// </summary>
	/// <param name = "gamePadButton">�Q�[���p�b�h�{�^��</param>
	/// <returns>�g���K�[��</returns>
	bool TriggerGamePadButton(const int gamePadButton);

	/// <summary>
	/// �Q�[���p�b�h�{�^���𗣂������`�F�b�N
	/// </summary>
	/// <param name = "gamePadButton">�Q�[���p�b�h�{�^��</param>
	/// <returns>��������</returns>
	bool ReleaseGamePadButton(const int gamePadButton);

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N��X�����ɌX�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��X�����ɌX������</returns>
	bool TiltGamePadLStickX(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N��Y�����ɌX�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��Y�����ɌX������</returns>
	bool TiltGamePadLStickY(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N��X�����ɌX�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��X�����ɌX������</returns>
	bool TiltGamePadRStickX(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N��Y�����ɌX�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��Y�����ɌX������</returns>
	bool TiltGamePadRStickY(const float incline);


	bool PushGamePadTrigger(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N��X�����ɌX�����u�Ԃ��`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��X�����ɌX�����u�Ԃ�</returns>
	bool TriggerGamePadLStickX(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N��Y�����ɌX�����u�Ԃ��`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��Y�����ɌX�����u�Ԃ�</returns>
	bool TriggerGamePadLStickY(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N��X�����ɌX�����u�Ԃ��`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��X�����ɌX�����u�Ԃ�</returns>
	bool TriggerGamePadRStickX(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N��Y�����ɌX�����u�Ԃ��`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��Y�����ɌX�����u�Ԃ�</returns>
	bool TriggerGamePadRStickY(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N��X�������痣�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��X�������痣������</returns>
	bool ReleaseGamePadLStickX(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N��Y�������痣�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��Y�������痣������</returns>
	bool ReleaseGamePadLStickY(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N��X�������痣�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��X�������痣������</returns>
	bool ReleaseGamePadRStickX(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N��Y�������痣�������`�F�b�N
	/// </summary>
	/// <param name="incline">�ǂ��܂ŌX�����画�����邩(-1000�`1000)</param>
	/// <returns>�X�e�B�b�N��Y�������痣������</returns>
	bool ReleaseGamePadRStickY(const float incline);

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N�̌X�����擾
	/// </summary>
	/// <returns>�Q�[���p�b�h�̍��X�e�B�b�N�̌X��(-1�`1)</returns>
	DirectX::XMFLOAT2 GetPadLStickIncline();

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N�̌X�����擾
	/// </summary>
	/// <returns>�Q�[���p�b�h�̉E�X�e�B�b�N�̌X��(-1�`1)</returns>
	DirectX::XMFLOAT2 GetPadRStickIncline();

	/// <summary>
	/// �Q�[���p�b�h�̍��X�e�B�b�N�̊p�x���擾
	/// </summary>
	/// <returns>�Q�[���p�b�h�̍��X�e�B�b�N�̊p�x</returns>
	float GetPadLStickAngle();

	/// <summary>
	/// �Q�[���p�b�h�̉E�X�e�B�b�N�̊p�x���擾
	/// </summary>
	/// <returns>�Q�[���p�b�h�̉E�X�e�B�b�N�̊p�x</returns>
	float GetPadRStickAngle();

private: //�����o�ϐ�
	//DirectInput�̃C���X�^���X����
	ComPtr<IDirectInput8> dinput;
	//�L�[�{�[�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devkeyboard;
	//�S�L�[�̏��
	BYTE key[256] = {};
	//�O��̑S�L�[�̏��
	BYTE keyPre[256] = {};
	//�}�E�X�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devmouse;
	//�}�E�X�̏��
	DIMOUSESTATE mouse;
	//�O��̃}�E�X�̏��
	DIMOUSESTATE mousePre;
	//�}�E�X�|�C���^�[
	POINT mousePoint;
	//�Q�[���p�b�h�̃f�o�C�X
	ComPtr<IDirectInputDevice8> devgamepad;
	//�Q�[���p�b�h�̏��
	DIJOYSTATE pad;
	//�O��̃Q�[���p�b�h�̏��
	DIJOYSTATE padPre;
	//�p�����[�^
	LPVOID parameter;
	//WindowApp
	WindowApp* win = nullptr;
};