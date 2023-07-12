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
	// デバイス発見時に実行される
	return DIENUM_CONTINUE;
}

void Input::Initialize(WindowApp* win)
{
	//借りてきたWindowAppのインスタンスを記録
	this->win = win;

	HRESULT result;

	//DirectInputのインスタンス生成
	result = DirectInput8Create(
		win->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);


	///-----------------キー入力初期化-----------------///

	//キーボードデバイスの設定
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);
	//入力データ形式のセット
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);	//標準形式
	//排他制御レベルのセット
	result = devkeyboard->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);


	///----------------マウス入力初期化----------------///

	//マウスデバイスの設定
	result = dinput->CreateDevice(GUID_SysMouse, &devmouse, NULL);
	//入力データ形式のセット
	result = devmouse->SetDataFormat(&c_dfDIMouse);	//標準形式
	//排他制御レベルのセット
	result = devmouse->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	///----------------パッド入力初期化----------------///
	//デバイスの列挙
	result = dinput->EnumDevices(
		DI8DEVTYPE_JOYSTICK, DeviceFindCallBack, &parameter, DIEDFL_FORCEFEEDBACK | DIEDFL_ATTACHEDONLY);
	//ゲームパッドデバイスの設定
	result = dinput->CreateDevice(GUID_Joystick, &devgamepad, NULL);

	//ゲームパッドが接続されていなければここで関数を抜ける
	if (devgamepad == nullptr) { return; }

	//入力データ形式のセット
	result = devgamepad->SetDataFormat(&c_dfDIJoystick);	//標準形式
	//排他制御レベルのセット
	result = devgamepad->SetCooperativeLevel(
		win->GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);


	//軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;	//絶対値モードの指定(DIPROPAXISMODE_RELにしたら相対値)
	//軸モードを変更
	result = devgamepad->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	//X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//RX軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RX;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//RY軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_RY;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
	//Z軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Z;
	result = devgamepad->SetProperty(DIPROP_RANGE, &diprg.diph);
}

void Input::Update()
{
	HRESULT result;

	///-----------------キー入力更新-----------------///

	//前回のキー入力を保存
	memcpy(keyPre, key, sizeof(key));
	//キーボード情報の取得開始
	result = devkeyboard->Acquire();
	//全キーの入力状態を取得する
	result = devkeyboard->GetDeviceState(sizeof(key), key);


	///----------------マウス入力更新----------------///

	//前回のマウス入力を保存
	mousePre = mouse;
	//マウス情報の取得開始
	result = devmouse->Acquire();
	//マウスの入力状態を取得する
	result = devmouse->GetDeviceState(sizeof(mouse), &mouse);
	//マウス座標を取得する
	GetCursorPos(&mousePoint);
	//スクリーン座標をクライアント座標に変換する
	ScreenToClient(win->GetHwnd(), &mousePoint);


	///----------------パッド入力更新----------------///

	//ゲームパッドが接続されていなければここで関数を抜ける
	if (devgamepad == nullptr) { return; }

	//前回のゲームパッド入力を保存
	padPre = pad;
	//ゲームパッド情報の取得開始
	result = devgamepad->Acquire();
	//ゲームパッドの入力状態を取得する
	result = devgamepad->GetDeviceState(sizeof(pad), &pad);
}

bool Input::PushKey(const BYTE keyNumber)
{
	//指定のキーを押していればtrueを返す
	if (key[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerKey(const BYTE keyNumber)
{
	//指定のキーを押した瞬間ならtrueを返す
	if (key[keyNumber] && !keyPre[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::ReleaseKey(const BYTE keyNumber)
{
	//指定のキーを離した瞬間ならtrueを返す
	if (!key[keyNumber] && keyPre[keyNumber]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::PushMouseButton(const int mouseButton)
{
	//指定のボタンを押していればtrueを返す
	if (mouse.rgbButtons[mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TriggerMouseButton(const int mouseButton)
{
	//指定のボタンを押した瞬間ならtrueを返す
	if (mouse.rgbButtons[mouseButton] &&
		!mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::ReleaseMouseButton(const int mouseButton)
{
	//指定のボタンを離した瞬間ならtrueを返す
	if (!mouse.rgbButtons[mouseButton] &&
		mousePre.rgbButtons[mouseButton]) {
		return true;
	}
	//そうでなければfalseを返す
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
	//ゲームパッドが接続されていなければfalseを返す
	if (devgamepad == nullptr) { return false; }

	//十字キーの指定のボタンを押していればtrueを返す
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
	//十字キー以外の指定のボタンを押していればtrueを返す
	else {
		if (pad.rgbButtons[gamePadButton]) {
			return true;
		}
	}
	//どちらでもなければfalseを返す
	return false;
}

bool Input::TriggerGamePadButton(const int gamePadButton)
{
	//ゲームパッドが接続されていなければfalseを返す
	if (devgamepad == nullptr) { return false; }

	//十字キーの指定のボタンを押した瞬間ならtrueを返す
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
	//十字キー以外の指定のボタンを押した瞬間ならtrueを返す
	else {
		if (pad.rgbButtons[gamePadButton] &&
			!padPre.rgbButtons[gamePadButton]) {
			return true;
		}
	}
	//どちらでもなければfalseを返す
	return false;
}

bool Input::ReleaseGamePadButton(const int gamePadButton)
{
	//ゲームパッドが接続されていなければfalseを返す
	if (devgamepad == nullptr) { return false; }

	//十字キーの指定のボタンを離した瞬間ならtrueを返す
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
	//十字キー以外の指定のボタンを離した瞬間ならtrueを返す
	else {
		if (!pad.rgbButtons[gamePadButton] &&
			padPre.rgbButtons[gamePadButton]) {
			return true;
		}
	}
	//そうでなければfalseを返す
	return false;
}

bool Input::TiltGamePadLStickX(const float incline)
{
	//右に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きければtrueを返す
		if (pad.lX >= incline) {
			return true;
		}
	}
	//左に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さければtrueを返す
		if (pad.lX <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::TiltGamePadLStickY(const float incline)
{
	//下に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きければtrueを返す
		if (pad.lY >= incline) {
			return true;
		}
	}
	//上に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さければtrueを返す
		if (pad.lY <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::TiltGamePadRStickX(const float incline)
{
	//右に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きければtrueを返す
		if (pad.lRx >= incline) {
			return true;
		}
	}
	//左に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さければtrueを返す
		if (pad.lRx <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::TiltGamePadRStickY(const float incline)
{
	//下に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きければtrueを返す
		if (pad.lRy >= incline) {
			return true;
		}
	}
	//上に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さければtrueを返す
		if (pad.lRy <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::PushGamePadTrigger(const float incline)
{

	//下に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きければtrueを返す
		if (pad.lZ >= incline) {
			return true;
		}
	}
	//上に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さければtrueを返す
		if (pad.lZ <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::TriggerGamePadLStickX(const float incline)
{
	//右に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きい瞬間ならtrueを返す
		if (pad.lX >= incline && !(padPre.lX >= incline)) {
			return true;
		}
	}
	//左に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さい瞬間ならtrueを返す
		if (pad.lX <= incline && !(padPre.lX <= incline)) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::TriggerGamePadLStickY(const float incline)
{
	//下に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きい瞬間ならtrueを返す
		if (pad.lY >= incline && !(padPre.lY >= incline)) {
			return true;
		}
	}
	//上に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さい瞬間ならtrueを返す
		if (pad.lY <= incline && !(padPre.lY <= incline)) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::TriggerGamePadRStickX(const float incline)
{
	//右に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きい瞬間ならtrueを返す
		if (pad.lRx >= incline && !(padPre.lRx >= incline)) {
			return true;
		}
	}
	//左に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さい瞬間ならtrueを返す
		if (pad.lRx <= incline && !(padPre.lRx <= incline)) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::TriggerGamePadRStickY(const float incline)
{
	//下に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きい瞬間ならtrueを返す
		if (pad.lRy >= incline && !(padPre.lRy >= incline)) {
			return true;
		}
	}
	//上に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さい瞬間ならtrueを返す
		if (pad.lRy <= incline && !(padPre.lRy <= incline)) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::ReleaseGamePadLStickX(const float incline)
{
	//右に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きかったならtrueを返す
		if (!(pad.lX >= incline) && padPre.lX >= incline) {
			return true;
		}
	}
	//左に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さかったならtrueを返す
		if (!(pad.lX <= incline) && padPre.lX <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::ReleaseGamePadLStickY(const float incline)
{
	//下に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きかったならtrueを返す
		if (!(pad.lY >= incline) && padPre.lY >= incline) {
			return true;
		}
	}
	//上に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さかったならtrueを返す
		if (!(pad.lY <= incline) && padPre.lY <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::ReleaseGamePadRStickX(const float incline)
{
	//右に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きかったならtrueを返す
		if (!(pad.lRx >= incline) && padPre.lRx >= incline) {
			return true;
		}
	}
	//左に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さかったならtrueを返す
		if (!(pad.lRx <= incline) && padPre.lRx <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
	return false;
}

bool Input::ReleaseGamePadRStickY(const float incline)
{
	//下に倒した時の判定
	if (incline > 0) {
		//スティックの傾斜が指定した数値より大きかったならtrueを返す
		if (!(pad.lRy >= incline) && padPre.lRy >= incline) {
			return true;
		}
	}
	//上に倒した時の判定
	else if (incline < 0) {
		//スティックの傾斜が指定した数値より小さかったならtrueを返す
		if (!(pad.lRy <= incline) && padPre.lRy <= incline) {
			return true;
		}
	}

	//どちらでもtrueがなければfalseを返す
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

	//マイナスだった場合の値を修正
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

	//マイナスだった場合の値を修正
	if (radian < 0) {
		radian += 360;
	}

	return radian;
}
