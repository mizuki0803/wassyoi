#pragma once
#include "Camera.h"
#include "EaseData.h"
#include <array>
#include <memory>
#include <vector>
#include <functional>

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera : public Camera 
{
public: //enum
	/// <summary>
	/// ステージから見ての横軸カメラ位置フェーズ
	/// </summary>
	enum class CameraXPosPhase
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
	enum class CameraYPosPhase
	{
		Top,	//上
		Side,	//横
		Buttom,	//下

		YPosPhaseNum	//フェーズの数(3)
	};

	/// <summary>
	/// 行動フェーズ
	/// </summary>
	enum class ActionPhase
	{
		None,			//何もしない
		MoveEye,		//視点移動
		ChangeDimension,//次元切り替え
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="distanceStageCenter">ステージ中央からの距離</param>
	/// <param name="stageCenterPos">ステージ中央座標</param>
	/// <returns>ゲームカメラ</returns>
	GameCamera* Create(float distanceStageCenter, const Vector3& stageCenterPos);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary> 
	/// <param name="distanceStageCenter">ステージ中央からの距離</param>
	/// <param name="stageCenterPos">ステージ中央座標</param>
	void Initialize(float distanceStageCenter, const Vector3& stageCenterPos);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 次元切り替え開始
	/// </summary>
	void ChanegeDimensionStart();

	//getter
	ActionPhase GetActionPhase() { return actionPhase; };
	const int GetCameraXPosPhase() { return cameraXPosPhase; };
	const int GetCameraYPosPhase() { return cameraYPosPhase; };
	const bool GetIs2D() { return is2D; }
	const bool GetIsTriggerDimensionChange() { return isTriggerDimensionChange; }

private: //メンバ関数
	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateMatProjection() override;

	/// <summary>
	/// カメラ位置移動開始
	/// </summary>
	void ChanegeCameraPosStart();

	/// <summary>
	/// 横軸カメラ位置移動開始
	/// </summary>
	/// <returns>横軸カメラ位置移動開始を開始するか</returns>
	bool ChanegeCameraXPosStart();

	/// <summary>
	/// 縦軸カメラ位置移動開始
	/// </summary>
	/// <returns>縦軸カメラ位置移動開始を開始するか</returns>
	bool ChanegeCameraYPosStart();

	/// <summary>
	/// カメラ位置移動
	/// </summary>
	void ChanegeCameraPos();

	/// <summary>
	/// 次元切り替え
	/// </summary>
	void ChanegeDimension();

	/// <summary>
	/// 開始時の位置調整
	/// </summary>
	void CameraSetMove();

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

private:
	//ステージ中央座標
	Vector3 stageCenterPos;
	//ステージから見ての横軸カメラ位置フェーズ
	int cameraXPosPhase = (int)CameraXPosPhase::Front;
	//ステージから見ての縦軸カメラ位置フェーズ
	int cameraYPosPhase = (int)CameraYPosPhase::Side;
	//ステージ中央からの距離
	float distanceStageCenter;
	//カメラ位置フェーズ全パターンの視点位置3D
	std::array<std::array<Vector3, (int)CameraXPosPhase::XPosPhaseNum>, (int)CameraYPosPhase::YPosPhaseNum> phaseEyePositions3D;
	//カメラ位置フェーズ全パターンの視点位置2D
	std::array<std::array<Vector3, (int)CameraXPosPhase::XPosPhaseNum>, (int)CameraYPosPhase::YPosPhaseNum> phaseEyePositions2D;
	//アクション
	ActionPhase actionPhase = ActionPhase::None;
	//アクション用タイマー
	int32_t actionTimer = 0;
	//移動前視点
	Vector3 moveBeforeEye;
	//移動目標視点
	Vector3 moveAfterEye;
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
};