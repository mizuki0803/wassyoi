#pragma once
#include "BaseScene.h"
#include <DirectXMath.h>

class GameCamera;
class Player;

/// <summary>
/// ゲーム固有の基底シーン
/// </summary>
class BaseGameScene : public BaseScene
{
protected: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected: //メンバ関数
	/// <summary>
	/// シーン変更情報をセットし、シーン変更を開始する
	/// </summary>
	/// <param name="color">画面を覆う色</param>
	/// <param name="deepenTime">色を濃くする時間</param>
	/// <param name="waitTime">色を濃くしたまま待機する時間</param>
	/// <param name="returnTime">色を戻す時間</param>
	/// <param name="nextSceneName">シーン変更後のシーン名</param>
	void SceneChangeStart(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime, const std::string& nextSceneName);

	/// <summary>
	/// シーン変更状態
	/// </summary>
	void SceneChangeMode();

	/// <summary>
	/// binary削除
	/// </summary>
	void DeleteBinary();

	/// <summary>
	/// 状態の保存
	/// </summary>
	/// <param name="_camera">カメラクラス</param>
	/// <param name="_player">プレイヤークラス</param>
	void KeepBinary(GameCamera& _camera, Player& _player);

	/// <summary>
	/// undo
	/// </summary>
	/// <param name="_camera">カメラクラス</param>
	/// <param name="_player">プレイヤークラス</param>
	void Undo(GameCamera* _camera, Player* _player);

	/// <summary>
	/// redo
	/// </summary>
	/// <param name="_camera">カメラクラス</param>
	/// <param name="_player">プレイヤークラス</param>
	void Redo(GameCamera* _camera, Player* _player);

protected: //メンバ変数
	//シーン変更にかかる時間
	int32_t sceneChangeTime = 0;
	//シーン変更開始からのタイマー
	int32_t sceneChangeTimer = 0;
	//シーン変更状態か
	bool isSceneChange = false;
	//次のシーン
	std::string nextSceneName;
#pragma region
	//現在の手順
	int orderNum;
	//現在の最大手順
	int orderMaxNum;
	//消去時の最大番号
	int deleteOrderMaxNum;
#pragma endregion undo redo用

};
