#pragma once
#include "Camera.h"
#include "EaseData.h"
#include <array>
#include <memory>
#include <vector>
#include <functional>

//プレイヤークラスの前方宣言
class Player;

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera : public Camera
{
public: //enum
	/// <summary>
	/// ステージから見ての横軸カメラ位置フェーズ
	/// </summary>
	enum CameraXPosPhase
	{
		Front,	//手前
		Right,	//右
		Back,	//奥
		Left,	//左

		XPosPhaseNum	//フェーズの数(4)
	};

	/// <summary>
	/// ステージから見ての縦軸カメラ位置フェーズ
	/// </summary>
	enum CameraYPosPhase
	{
		Top,		//上
		Side,		//横
		Buttom,		//下
		ReverseSide,//逆向きの横

		YPosPhaseNum	//フェーズの数(3)
	};

	/// <summary>
	/// 行動フェーズ
	/// </summary>
	enum class ActionPhase
	{
		None,			//何もしない
		Rotation,		//カメラ回転
		ChangeDimension,//次元切り替え
	};

	/// <summary>
	/// ゲームの全体
	/// </summary>
	enum class GamePhase
	{
		Start,	//開始
		Play,	//ゲーム中
		ClearReturn3D,	//クリアに3次元に戻す
		ReStart,//再度生成
		BackBased,
		None,	//何もしない
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="distanceStageCenter">ステージ中央からの距離</param>
	/// <param name="stageCenterPos">ステージ中央座標</param>
	/// <returns>ゲームカメラ</returns>
	GameCamera* Create(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary> 
	/// <param name="distanceStageCenter">ステージ中央からの距離</param>
	/// <param name="stageCenterPos">ステージ中央座標</param>
	void Initialize(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ゲーム中
	/// </summary>
	void PlayGame();
	/// <summary>
	/// ゲーム開始
	/// </summary>
	void GameStart();
	/// <summary>
	/// クリア時開始の処理
	/// </summary>
	void SetClearMode();
	/// <summary>
	/// クリア時に3次元に戻す処理
	/// </summary>
	void ClearReturn3D();
	/// <summary>
	/// ゲーム再開始
	/// </summary>
	void GameReStart();
	/// <summary>
	/// 何もしない
	/// </summary>
	void StayGame();

	void BackBasedMove();
	/// <summary>
	/// 関数の設定
	/// </summary>
	void CreateAct();

	/// <summary>
	/// 次元切り替え開始
	/// </summary>
	void ChanegeDimensionStart();
	/// <summary>
	/// 再生成時に動かすための処理
	/// </summary>
	void SetReCreateMove();

	/// <summary>
	/// 開始状態に戻す
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

private: //メンバ関数
	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateMatProjection() override;

	/// <summary>
	/// ワールド行列を更新
	/// </summary>
	/// <param name="matTrans">平行移動行列</param>
	void UpdateMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// 視点と注視点を更新
	/// </summary>
	void UpdateEyeTarget();

	/// <summary>
	/// 座標更新
	/// </summary>
	void UpdatePosition();

	/// <summary>
	/// 回転開始時の入力による回転方向設定
	/// </summary>
	/// <returns></returns>
	Vector3 InputRotateNum();

	/// <summary>
	/// 回転開始
	/// </summary>
	/// <param name="is2D">2次元状態か</param>
	void RotateStart();

	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotate();

	/// <summary>
	/// 回転角が大きすぎたり小さすぎたりしないよう、0～360内に収まる用ように調整
	/// </summary>
	/// <param name="rotation">回転角</param>
	void MaxMinRotate(float& rotation);

	/// <summary>
	///	カメラの回転角の傾きからカメラ位置フェーズを返す
	/// </summary>
	void CameraPosPhaseCheck();

	/// <summary>
	/// 次元切り替え
	/// </summary>
	void ChanegeDimension();

	/// <summary>
	/// 開始時の位置調整
	/// </summary>
	void CameraSetMove();

	/// <summary>
	/// 
	/// </summary>
	void GamePlayStratCameraSetMove();

	/// <summary>
	/// カメラのシェイク
	/// </summary>
	void ShakeMove();
	/// <summary>
	/// イージングデータの設定
	/// </summary>
	/// <param name="count">フレームのカウント</param>
	void SetEaseData(const int count);

	/// <summary>
	/// 2dになる際のイージング
	/// </summary>
	/// <param name="_mat1">イージング前</param>
	/// <param name="_mat2">イージング後</param>
	/// <param name="_timer">イージング用タイマー</param>
	/// <returns>現在座標</returns>
	XMMATRIX Ease4x4_in(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer);

	/// <summary>
	/// 3dになる際のイージング
	/// </summary>
	/// <param name="_mat1">イージング前</param>
	/// <param name="_mat2">イージング後</param>
	/// <param name="_timer">イージング用タイマー</param>
	/// <returns>現在座標</returns>
	XMMATRIX Ease4x4_out(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer);


private: //静的メンバ変数
	//一度の回転にかかる時間
	static const int32_t rotateTime = 40;
	//3Dでの視点時にカメラを傾ける量
	static const float rotate3DDistance;
	//2dのプロジェクション行列
	static const XMMATRIX matProj2D;
	//3dのプロジェクション行列
	static const XMMATRIX matProj3D;

private: //メンバ変数
	//プレイヤーの情報
	Player* player;
	//回転角
	Vector3 rotation = { 0, 0, 0 };
	//座標
	Vector3 position = { 0, 0, 0 };
	//実際にカメラの視点,注視点,上ベクトルに使用するワールド変換行列
	XMMATRIX cameraMatWorld = {};
	//ステージ中央座標
	Vector3 stageCenterPos;
	//ステージから見ての横軸カメラ位置フェーズ
	int cameraXPosPhase = (int)CameraXPosPhase::Front;
	//ステージから見ての縦軸カメラ位置フェーズ
	int cameraYPosPhase = (int)CameraYPosPhase::Side;
	//ステージ中央からの距離
	XMFLOAT3 distanceStageCenter;
	//アクション
	ActionPhase actionPhase = ActionPhase::None;
	//アクション用タイマー
	int32_t actionTimer = 0;
	//回転前回転角
	Vector3 rotateBefore;
	//回転後回転角
	Vector3 rotateAfter;
	//2次元状態か
	bool is2D = false;
	//次元変更が完了した瞬間か
	bool isTriggerDimensionChange = false;
	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_ = 0;
	// イージングデータ
	std::unique_ptr<EaseData> easeData_;
	// イージング用に保存する座標
	// 開始位置
	Vector3 stratPos_ = {};
	// 終了位置
	Vector3 endPos_ = {};
	//
	XMFLOAT3 saveDistanceStageCenter_;
	// シェイクしているか
	bool isShake_ = false;
	// シェイクが終わったか
	bool shakeEnd_ = false;
	// シェイクタイマー
	int shakeTimer_ = 0;
	// 減衰値
	int attenuation_ = 0;
	// カメラのイージングの切り替えし
	bool cameraEaseChangeFlag_ = false;
	//ステージクリア状態か
	bool isStageClear = false;
};