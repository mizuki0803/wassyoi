#include "Input.h"
#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input instance;

	return &instance;
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	// �f�o�C�X�������Ɏ��s�����
	return DIENUM_CONTINUE;
}

void Input::Initialize(WindowApp* win)
{
	//�؂�Ă���WindowApp�̃C���X�^���X���L�^
	this->win = win;

	HRESULT result;

	//DirectInput�̃C���X�^���X����
	result = DirectInput8Create(
		win->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);


	///-----------------�L�[���͏�����-----------------///

	//�L�[�{�[�h�f�o�C�X�̐ݒ�
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//���̓f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);	//�W���`��
	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);


	///----------------�}�E�X���͏�����----------------///

	//�}�E�X�f�o�C�X�̐ݒ�
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	//���̓f�[�^�`���̃Z�b�g
	result = devmouse->SetDataFormat(&c_dfDIMouse);	//�W���`��
	//�r�����䃌�x���̃Z�b�g
	result = devmouse->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	///----------------�p�b�h���͏�����----------------///
	//�f�o�C�X�̗�
	result = dinput->EnumDevices(
		DI8DEVTYPE_JOYSTICK, DeviceFindCallBack, &parameter, DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY);
	//�Q�[���p�b�h�f�o�C�X�̐ݒ�
	result = dinput->CreateDevice(GUID_Joystick, &devgamepad, NULL);

	//�Q�[���p�b�h���ڑ�����Ă��Ȃ���΂����Ŋ֐��𔲂���
	if (devgamepad == nullptr) { return; }

	//���̓f�[�^�`���̃Z�b�g
	result = devgamepad->SetDataFormat(&c_dfDIJoystick);	//�W���`��
	//�r�����䃌�x���̃Z�b�g
	result = devgamepad->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);


	//�����[�h���Βl���[�h�Ƃ��Đݒ�
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	//��Βl���[�h�̎w��(DIPROPAXISMODE_REL�ɂ����瑊�Βl)
	//�����[�h��ύX
	result = devgamepad->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	//X���̒l�͈̔͐ݒ�
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//Y���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//RX���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RX;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//RY���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RY;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//Z���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
}

void Input::Update()
{
	HRESULT result;

	///-----------------�L�[���͍X�V-----------------///

	//�O��̃L�[���͂�ۑ�
	memcpy(keyPre, key, sizeof(key));
	//�L�[�{�[�h���̎擾�J�n
	result = devkeyboard->Acquire();
	//�S�L�[�̓��͏�Ԃ��擾����
	result = devkeyboard->GetDeviceState(sizeof(key), key);


	///----------------�}�E�X���͍X�V----------------///

	//�O��̃}�E�X���͂�ۑ�
	mousePre = mouse;
	//�}�E�X���̎擾�J�n
	result = devmouse->Acquire();
	//�}�E�X�̓��͏�Ԃ��擾����
	result = devmouse->GetDeviceState(sizeof(mouse), &mouse);
	//�}�E�X���W���擾����
	GetCursorPos(&mousePoint);
	//�X�N���[�����W���N���C�A���g���W�ɕϊ�����
	ScreenToClient(win->GetHwnd(), &mousePoint);


	///----------------�p�b�h���͍X�V----------------///

	//�Q�[���p�b�h���ڑ�����Ă��Ȃ���΂����Ŋ֐��𔲂���
	if (devgamepad == nullptr) { return; }

	//�O��̃Q�[���p�b�h���͂�ۑ�
	padPre = pad;
	//�Q�[���p�b�h���̎擾�J�n
	result = devgamepad->Acquire();
	//�Q�[���p�b�h�̓��͏�Ԃ��擾����
	result = devgamepad->GetDeviceState(sizeof(pad), &pad);
}

bool Input::PushKey(const BYTE keyNumber)
{
	//�w��̃L�[�������Ă����true��Ԃ�
	if (key[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::TriggerKey(const BYTE keyNumber)
{
	//�w��̃L�[���������u�ԂȂ�true��Ԃ�
	if (key[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::ReleaseKey(const BYTE keyNumber)
{
	//�w��̃L�[�𗣂����u�ԂȂ�true��Ԃ�
	if (!key[keyNumber] && keyPre[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::PushMouseButton(const int mouseButton)
{
	//�w��̃{�^���������Ă����true��Ԃ�
	if (mouse.rgbButtons[mouseButton]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::TriggerMouseButton(const int mouseButton)
{
	//�w��̃{�^�����������u�ԂȂ�true��Ԃ�
	if (mouse.rgbButtons[mouseButton] &&
		!mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::ReleaseMouseButton(const int mouseButton)
{
	//�w��̃{�^���𗣂����u�ԂȂ�true��Ԃ�
	if (!mouse.rgbButtons[mouseButton] &&
		mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

DirectX::XMFLOAT2 Input::GetMousePoint()
{
	return DirectX::XMFLOAT2((float)mousePoint.x, (float)mousePoint.y);
}

DirectX::XMFLOAT2 Input::GetMouseVelocity()
{
	return DirectX::XMFLOAT2((float)mouse.lX, (float)mouse.lY);
}

float Input::GetMouseWheelVelocity()
{
	float wheel = mouse.lZ / 120.0f;
	return wheel;
}

bool Input::PushGamePadButton(const int gamePadButton)
{
	//�Q�[���p�b�h���ڑ�����Ă��Ȃ����false��Ԃ�
	if (devgamepad == nullptr) { return false; }

	//�\���L�[�̎w��̃{�^���������Ă����true��Ԃ�
	if (gamePadButton >= PAD_LEFT && gamePadButton <= PAD_DOWN) {
		if (gamePadButton == PAD_LEFT) {
			bool inputPadLeft = (pad.rgdwPOV[0] == 22500 || pad.rgdwPOV[0] == 27000 || pad.rgdwPOV[0] == 31500);
			if (inputPadLeft) {
				return true;
			}
		}
		else if (gamePadButton == PAD_RIGHT) {
			bool inputPadRight = (pad.rgdwPOV[0] == 4500 || pad.rgdwPOV[0] == 9000 || pad.rgdwPOV[0] == 13500);
			if (inputPadRight) {
				return true;
			}
		}
		else if (gamePadButton == PAD_UP) {
			bool inputPadUp = (pad.rgdwPOV[0] == 0 || pad.rgdwPOV[0] == 4500 || pad.rgdwPOV[0] == 31500);
			if (inputPadUp) {
				return true;
			}
		}
		else if (gamePadButton == PAD_DOWN) {
			bool inputPadDown = (pad.rgdwPOV[0] == 13500 || pad.rgdwPOV[0] == 18000 || pad.rgdwPOV[0] == 22500);
			if (inputPadDown) {
				return true;
			}
		}
	}
	//�\���L�[�ȊO�̎w��̃{�^���������Ă����true��Ԃ�
	else {
		if (pad.rgbButtons[gamePadButton]) {
			return true;
		}
	}
	//�ǂ���ł��Ȃ����false��Ԃ�
	return false;
}

bool Input::TriggerGamePadButton(const int gamePadButton)
{
	//�Q�[���p�b�h���ڑ�����Ă��Ȃ����false��Ԃ�
	if (devgamepad == nullptr) { return false; }

	//�\���L�[�̎w��̃{�^�����������u�ԂȂ�true��Ԃ�
	if (gamePadButton >= PAD_LEFT && gamePadButton <= PAD_DOWN) {
		if (gamePadButton == PAD_LEFT) {
			bool inputPadLeft = (pad.rgdwPOV[0] == 22500 || pad.rgdwPOV[0] == 27000 || pad.rgdwPOV[0] == 31500);
			bool inputPadPreLeft = (padPre.rgdwPOV[0] == 22500 || padPre.rgdwPOV[0] == 27000 || padPre.rgdwPOV[0] == 31500);
			if (inputPadLeft && !inputPadPreLeft) {
				return true;
			}
		}
		else if (gamePadButton == PAD_RIGHT) {
			bool inputPadRight = (pad.rgdwPOV[0] == 4500 || pad.rgdwPOV[0] == 9000 || pad.rgdwPOV[0] == 13500);
			bool inputPadPreRight = (padPre.rgdwPOV[0] == 4500 || padPre.rgdwPOV[0] == 9000 || padPre.rgdwPOV[0] == 13500);
			if (inputPadRight && !inputPadPreRight) {
				return true;
			}
		}
		else if (gamePadButton == PAD_UP) {
			bool inputPadUp = (pad.rgdwPOV[0] == 0 || pad.rgdwPOV[0] == 4500 || pad.rgdwPOV[0] == 31500);
			bool inputPadPreUp = (padPre.rgdwPOV[0] == 0 || padPre.rgdwPOV[0] == 4500 || padPre.rgdwPOV[0] == 31500);
			if (inputPadUp && !inputPadPreUp) {
				return true;
			}
		}
		else if (gamePadButton == PAD_DOWN) {
			bool inputPadDown = (pad.rgdwPOV[0] == 13500 || pad.rgdwPOV[0] == 18000 || pad.rgdwPOV[0] == 22500);
			bool inputPadPreDown = (padPre.rgdwPOV[0] == 13500 || padPre.rgdwPOV[0] == 18000 || padPre.rgdwPOV[0] == 22500);
			if (inputPadDown && !inputPadPreDown) {
				return true;
			}
		}
	}
	//�\���L�[�ȊO�̎w��̃{�^�����������u�ԂȂ�true��Ԃ�
	else {
		if (pad.rgbButtons[gamePadButton] &&
			!padPre.rgbButtons[gamePadButton]) {
			return true;
		}
	}
	//�ǂ���ł��Ȃ����false��Ԃ�
	return false;
}

bool Input::ReleaseGamePadButton(const int gamePadButton)
{
	//�Q�[���p�b�h���ڑ�����Ă��Ȃ����false��Ԃ�
	if (devgamepad == nullptr) { return false; }

	//�\���L�[�̎w��̃{�^���𗣂����u�ԂȂ�true��Ԃ�
	if (gamePadButton >= PAD_LEFT && gamePadButton <= PAD_DOWN) {
		if (gamePadButton == PAD_LEFT) {
			bool inputPadLeft = (pad.rgdwPOV[0] == 22500 || pad.rgdwPOV[0] == 27000 || pad.rgdwPOV[0] == 31500);
			bool inputPadPreLeft = (padPre.rgdwPOV[0] == 22500 || padPre.rgdwPOV[0] == 27000 || padPre.rgdwPOV[0] == 31500);
			if (!inputPadLeft && inputPadPreLeft) {
				return true;
			}
		}
		else if (gamePadButton == PAD_RIGHT) {
			bool inputPadRight = (pad.rgdwPOV[0] == 4500 || pad.rgdwPOV[0] == 9000 || pad.rgdwPOV[0] == 13500);
			bool inputPadPreRight = (padPre.rgdwPOV[0] == 4500 || padPre.rgdwPOV[0] == 9000 || padPre.rgdwPOV[0] == 13500);
			if (!inputPadRight && inputPadPreRight) {
				return true;
			}
		}
		else if (gamePadButton == PAD_UP) {
			bool inputPadUp = (pad.rgdwPOV[0] == 0 || pad.rgdwPOV[0] == 4500 || pad.rgdwPOV[0] == 31500);
			bool inputPadPreUp = (padPre.rgdwPOV[0] == 0 || padPre.rgdwPOV[0] == 4500 || padPre.rgdwPOV[0] == 31500);
			if (!inputPadUp && inputPadPreUp) {
				return true;
			}
		}
		else if (gamePadButton == PAD_DOWN) {
			bool inputPadDown = (pad.rgdwPOV[0] == 13500 || pad.rgdwPOV[0] == 18000 || pad.rgdwPOV[0] == 22500);
			bool inputPadPreDown = (padPre.rgdwPOV[0] == 13500 || padPre.rgdwPOV[0] == 18000 || padPre.rgdwPOV[0] == 22500);
			if (!inputPadDown && inputPadPreDown) {
				return true;
			}
		}
	}
	//�\���L�[�ȊO�̎w��̃{�^���𗣂����u�ԂȂ�true��Ԃ�
	else {
		if (!pad.rgbButtons[gamePadButton] &&
			padPre.rgbButtons[gamePadButton]) {
			return true;
		}
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::TiltGamePadLStickX(const float incline)
{
	//�E�ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�����true��Ԃ�
		if (pad.lX >= incline) {
			return true;
		}
	}
	//���ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�������true��Ԃ�
		if (pad.lX <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::TiltGamePadLStickY(const float incline)
{
	//���ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�����true��Ԃ�
		if (pad.lY >= incline) {
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�������true��Ԃ�
		if (pad.lY <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::TiltGamePadRStickX(const float incline)
{
	//�E�ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�����true��Ԃ�
		if (pad.lRx >= incline) {
			return true;
		}
	}
	//���ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�������true��Ԃ�
		if (pad.lRx <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::TiltGamePadRStickY(const float incline)
{
	//���ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�����true��Ԃ�
		if (pad.lRy >= incline) {
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�������true��Ԃ�
		if (pad.lRy <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::PushGamePadTrigger(const float incline)
{

	//���ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�����true��Ԃ�
		if (pad.lZ >= incline) {
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�������true��Ԃ�
		if (pad.lZ <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::TriggerGamePadLStickX(const float incline)
{
	//�E�ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫���u�ԂȂ�true��Ԃ�
		if (pad.lX >= incline && !(padPre.lX >= incline)) {
			return true;
		}
	}
	//���ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�����u�ԂȂ�true��Ԃ�
		if (pad.lX <= incline && !(padPre.lX <= incline)) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::TriggerGamePadLStickY(const float incline)
{
	//���ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫���u�ԂȂ�true��Ԃ�
		if (pad.lY >= incline && !(padPre.lY >= incline)) {
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�����u�ԂȂ�true��Ԃ�
		if (pad.lY <= incline && !(padPre.lY <= incline)) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::TriggerGamePadRStickX(const float incline)
{
	//�E�ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫���u�ԂȂ�true��Ԃ�
		if (pad.lRx >= incline && !(padPre.lRx >= incline)) {
			return true;
		}
	}
	//���ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�����u�ԂȂ�true��Ԃ�
		if (pad.lRx <= incline && !(padPre.lRx <= incline)) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::TriggerGamePadRStickY(const float incline)
{
	//���ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫���u�ԂȂ�true��Ԃ�
		if (pad.lRy >= incline && !(padPre.lRy >= incline)) {
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬�����u�ԂȂ�true��Ԃ�
		if (pad.lRy <= incline && !(padPre.lRy <= incline)) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::ReleaseGamePadLStickX(const float incline)
{
	//�E�ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�������Ȃ�true��Ԃ�
		if (!(pad.lX >= incline) && padPre.lX >= incline) {
			return true;
		}
	}
	//���ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬���������Ȃ�true��Ԃ�
		if (!(pad.lX <= incline) && padPre.lX <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::ReleaseGamePadLStickY(const float incline)
{
	//���ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�������Ȃ�true��Ԃ�
		if (!(pad.lY >= incline) && padPre.lY >= incline) {
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬���������Ȃ�true��Ԃ�
		if (!(pad.lY <= incline) && padPre.lY <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::ReleaseGamePadRStickX(const float incline)
{
	//�E�ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�������Ȃ�true��Ԃ�
		if (!(pad.lRx >= incline) && padPre.lRx >= incline) {
			return true;
		}
	}
	//���ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬���������Ȃ�true��Ԃ�
		if (!(pad.lRx <= incline) && padPre.lRx <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

bool Input::ReleaseGamePadRStickY(const float incline)
{
	//���ɓ|�������̔���
	if (incline > 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l���傫�������Ȃ�true��Ԃ�
		if (!(pad.lRy >= incline) && padPre.lRy >= incline) {
			return true;
		}
	}
	//��ɓ|�������̔���
	else if (incline < 0) {
		//�X�e�B�b�N�̌X�΂��w�肵�����l��菬���������Ȃ�true��Ԃ�
		if (!(pad.lRy <= incline) && padPre.lRy <= incline) {
			return true;
		}
	}

	//�ǂ���ł�true���Ȃ����false��Ԃ�
	return false;
}

DirectX::XMFLOAT2 Input::GetPadLStickIncline()
{
	return DirectX::XMFLOAT2((float)pad.lX / 1000, (float)pad.lY / 1000);
}

DirectX::XMFLOAT2 Input::GetPadRStickIncline()
{
	return DirectX::XMFLOAT2((float)pad.lRx / 1000, (float)pad.lRy / 1000);
}

float Input::GetPadLStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadLStickIncline();
	float h = pad.x;
	float v = pad.y;

	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	//�}�C�i�X�������ꍇ�̒l���C��
	if (radian < 0) {
		radian += 360;
	}

	return radian;
}

float Input::GetPadRStickAngle()
{
	DirectX::XMFLOAT2 pad = GetPadRStickIncline();
	float h = pad.x;
	float v = pad.y;

	float radian = atan2f(v, h) * 360 / (3.141592f * 2);

	//�}�C�i�X�������ꍇ�̒l���C��
	if (radian < 0) {
		radian += 360;
	}

	return radian;
}
