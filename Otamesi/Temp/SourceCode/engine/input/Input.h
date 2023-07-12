#pragma once
#include <wrl.h>
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include <dinput.h>
#include <DirectXMath.h>

#include "WindowApp.h"

/// <summary>
/// 入力
/// </summary>
class Input final
{
public: //エイリアス
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //列挙
	//マウスボタン
	enum MouseButton
	{
		MOUSE_LEFT,
		MOUSE_RIGHT,
		MOUSE_WHEEL
	};
	//ゲームパッドボタン
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

private: //シングルトン化
	//コンストラクタを隠蔽
	Input() = default;
	//デストラクタを隠蔽
	~Input() = default;
public:
	//コピーコンストラクタを無効化
	Input(const Input& input) = delete;
	//代入演算子を無効化
	void operator = (const Input& input) = delete;

public: //メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>入力</returns>
	static Input* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="win">ウインドウアプリケーション</param>
	void Initialize(WindowApp* win);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name = "keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(const BYTE keyNumber);

	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name = "keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(const BYTE keyNumber);

	/// <summary>
	/// キーを離したかチェック
	/// </summary>
	/// <param name = "keyNumber">キー番号( DIK_0 等)</param>
	/// <returns>離したか</returns>
	bool ReleaseKey(const BYTE keyNumber);

	/// <summary>
	/// マウスボタンの押下をチェック
	/// </summary>
	/// <param name = "mouseButton">マウスボタン</param>
	/// <returns>押されているか</returns>
	bool PushMouseButton(const int mouseButton);

	/// <summary>
	/// マウスボタンのトリガーをチェック
	/// </summary>
	/// <param name = "mouseButton">マウスボタン</param>
	/// <returns>トリガーか</returns>
	bool TriggerMouseButton(const int mouseButton);

	/// <summary>
	/// マウスボタンを離したかチェック
	/// </summary>
	/// <param name = "mouseButton">マウスボタン</param>
	/// <returns>離したか</returns>
	bool ReleaseMouseButton(const int mouseButton);

	/// <summary>
	/// マウスポインターの座標を取得
	/// </summary>
	/// <returns>マウスポインターの座標</returns>
	DirectX::XMFLOAT2 GetMousePoint();

	/// <summary>
	/// 1フレームのマウスの移動量の取得
	/// </summary>
	/// <returns>1フレームのマウスの移動量</returns>
	DirectX::XMFLOAT2 GetMouseVelocity();

	/// <summary>
	/// 1フレームのマウスホイールの移動量の取得
	/// </summary>
	/// <returns>1フレームのマウスホイールの移動量</returns>
	float GetMouseWheelVelocity();

	/// <summary>
	/// ゲームパッドボタンの押下をチェック
	/// </summary>
	/// <param name = "gamePadButton">ゲームパッドボタン</param>
	/// <returns>押されているか</returns>
	bool PushGamePadButton(const int gamePadButton);

	/// <summary>
	/// ゲームパッドボタンのトリガーをチェック
	/// </summary>
	/// <param name = "gamePadButton">ゲームパッドボタン</param>
	/// <returns>トリガーか</returns>
	bool TriggerGamePadButton(const int gamePadButton);

	/// <summary>
	/// ゲームパッドボタンを離したかチェック
	/// </summary>
	/// <param name = "gamePadButton">ゲームパッドボタン</param>
	/// <returns>離したか</returns>
	bool ReleaseGamePadButton(const int gamePadButton);

	/// <summary>
	/// ゲームパッドの左スティックをX方向に傾けたかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをX方向に傾けたか</returns>
	bool TiltGamePadLStickX(const float incline);

	/// <summary>
	/// ゲームパッドの左スティックをY方向に傾けたかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをY方向に傾けたか</returns>
	bool TiltGamePadLStickY(const float incline);

	/// <summary>
	/// ゲームパッドの右スティックをX方向に傾けたかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをX方向に傾けたか</returns>
	bool TiltGamePadRStickX(const float incline);

	/// <summary>
	/// ゲームパッドの右スティックをY方向に傾けたかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをY方向に傾けたか</returns>
	bool TiltGamePadRStickY(const float incline);


	bool PushGamePadTrigger(const float incline);

	/// <summary>
	/// ゲームパッドの左スティックをX方向に傾けた瞬間かチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをX方向に傾けた瞬間か</returns>
	bool TriggerGamePadLStickX(const float incline);

	/// <summary>
	/// ゲームパッドの左スティックをY方向に傾けた瞬間かチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをY方向に傾けた瞬間か</returns>
	bool TriggerGamePadLStickY(const float incline);

	/// <summary>
	/// ゲームパッドの右スティックをX方向に傾けた瞬間かチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをX方向に傾けた瞬間か</returns>
	bool TriggerGamePadRStickX(const float incline);

	/// <summary>
	/// ゲームパッドの右スティックをY方向に傾けた瞬間かチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをY方向に傾けた瞬間か</returns>
	bool TriggerGamePadRStickY(const float incline);

	/// <summary>
	/// ゲームパッドの左スティックをX方向から離したかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをX方向から離したか</returns>
	bool ReleaseGamePadLStickX(const float incline);

	/// <summary>
	/// ゲームパッドの左スティックをY方向から離したかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをY方向から離したか</returns>
	bool ReleaseGamePadLStickY(const float incline);

	/// <summary>
	/// ゲームパッドの右スティックをX方向から離したかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをX方向から離したか</returns>
	bool ReleaseGamePadRStickX(const float incline);

	/// <summary>
	/// ゲームパッドの右スティックをY方向から離したかチェック
	/// </summary>
	/// <param name="incline">どこまで傾けたら判定を取るか(-1000～1000)</param>
	/// <returns>スティックをY方向から離したか</returns>
	bool ReleaseGamePadRStickY(const float incline);

	/// <summary>
	/// ゲームパッドの左スティックの傾きを取得
	/// </summary>
	/// <returns>ゲームパッドの左スティックの傾き(-1～1)</returns>
	DirectX::XMFLOAT2 GetPadLStickIncline();

	/// <summary>
	/// ゲームパッドの右スティックの傾きを取得
	/// </summary>
	/// <returns>ゲームパッドの右スティックの傾き(-1～1)</returns>
	DirectX::XMFLOAT2 GetPadRStickIncline();

	/// <summary>
	/// ゲームパッドの左スティックの角度を取得
	/// </summary>
	/// <returns>ゲームパッドの左スティックの角度</returns>
	float GetPadLStickAngle();

	/// <summary>
	/// ゲームパッドの右スティックの角度を取得
	/// </summary>
	/// <returns>ゲームパッドの右スティックの角度</returns>
	float GetPadRStickAngle();

private: //メンバ変数
	//DirectInputのインスタンス生成
	ComPtr<IDirectInput8> dinput;
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};
	//マウスのデバイス
	ComPtr<IDirectInputDevice8> devmouse;
	//マウスの状態
	DIMOUSESTATE mouse;
	//前回のマウスの状態
	DIMOUSESTATE mousePre;
	//マウスポインター
	POINT mousePoint;
	//ゲームパッドのデバイス
	ComPtr<IDirectInputDevice8> devgamepad;
	//ゲームパッドの状態
	DIJOYSTATE pad;
	//前回のゲームパッドの状態
	DIJOYSTATE padPre;
	//パラメータ
	LPVOID parameter;
	//WindowApp
	WindowApp* win = nullptr;
};