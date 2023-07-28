#pragma once
#include "ObjObject3d.h"
#include "GameCamera.h"
#include "EaseData.h"
#include "PlayerEffect.h"


/// <summary>
/// プレイヤー
/// </summary>
class Player : public ObjObject3d
{
public:
	friend class PlayerEffect;

	//enum
	/// <summary>
	/// 行動フェーズ
	/// </summary>
	enum class ActionPhase
	{
		None,	//何もしない
		MovePos,	//座標移動
	};

	/// <summary>
	/// ゲームフェーズ
	/// </summary>
	enum class GamePhase
	{
		GamePlay,	//ゲーム
		Start,		//開始
		ReStart,	//再開始
	};

	/// <summary>
	/// 上下左右前後、ブロックのどの面移動するかフェーズ
	/// </summary>
	enum class MoveSurfacePhase
	{
		Upward,			//上向き
		Downward,		//下向き
		FacingLeft,		//左向き
		FacingRight,	//右向き
		FacingForward,	//手前向き
		FacingAway,		//奥向き
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="mapChipNum">プレイヤー位置を表すマップ番号</param>
	/// <param name="shiftPos">マップを中心にずらす値</param>
	/// <param name="gameCamera">ゲームカメラ</param>
	/// <returns>プレイヤー</returns>
	static Player* Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& shiftPos, GameCamera* gameCamera, ObjModel* effectModel);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ゲーム中
	/// </summary>
	void PlayGame();
	/// <summary>
	/// ゲーム開始
	/// </summary>
	void GameStart();
	/// <summary>
	/// ゲーム再開始
	/// </summary>
	void GameReStart();

	/// <summary>
	/// 関数の設定
	/// </summary>
	void CreateAct();

	void ReCreate(const XMINT3& mapChipNum, const Vector3& shiftPos);

	void Draw();
	void EffectDraw();

	/// <summary>
	/// 開始状態に戻す
	/// </summary>
	void Reset();

	/// <summary>
	/// 次元変更を開始できるか判定
	/// </summary>
	/// <returns>次元変更を開始できるか</returns>
	bool ChangeDimensionStartCheck();

	//getter
	const bool GetIsGoal() { return isGoal; }
	const bool GetIsMove() { return isMove; }
	const bool GetNowMove() { return bool(actionPhase); }
	const bool GetIsStartMove() { return isStartMove; }
	const ActionPhase GetActionPhase() { return actionPhase; }
	const int GetMoveSurfacePhase() { return int(moveSurfacePhase); }
	const XMINT3 GetMapChipNumberPos() { return mapChipNumberPos; }
	//setter
	void SetMoveSurfacePhase(const int _moveSurfacePhase) { moveSurfacePhase = MoveSurfacePhase(_moveSurfacePhase); }
	void SetMapChipNumberPos(const XMINT3& _mapChipNumberPos) { mapChipNumberPos = _mapChipNumberPos; }
	void SetNotMove(bool menuFlag, bool isCreateMove, bool hintFlag)
	{ 
		menuFlag_ = menuFlag;
		isCreateMove_ = isCreateMove;
		hintFlag_ = hintFlag;
	}

private: //メンバ関数
	/// <summary>
	/// 座標移動開始
	/// </summary>
	void MovePosStart();

	/// <summary>
	/// 座標移動
	/// </summary>
	void MovePos();

	/// <summary>
	/// 次元切り替え開始
	/// </summary>
	void ChanegeDimensionStart();

	/// <summary>
	/// ステージクリアのチェック
	/// </summary>
	void StageClearCheck();

	/// <summary>
	/// マップ番号の座標を取得
	/// </summary>
	Vector3 GetMapChipPos(const XMINT3& mapChipNumberPos);

	/// <summary>
	/// イージングデータの設定
	/// </summary>
	/// <param name="count">フレームのカウント</param>
	void SetEaseData(const int count);
	/// <summary>
	/// イージングを開始する時の位置の保存
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerStratPos(const Vector3& position) { playerStratPos_ = position; }
	/// <summary>
	/// イージングを終了する時の位置の保存
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerEndPos(const Vector3& position) { playerEndPos_ = position; }
	/// <summary>
	/// 行動の設定
	/// </summary>
	/// <param name="gamePhase"></param>
	void SetGamePhase(GamePhase gamePhase) { phase_ = static_cast<int>(gamePhase); }

private: //静的メンバ変数
	//プレイヤーの大きさ
	static const float playerSize;

private: //メンバ変数

	std::array<std::unique_ptr<PlayerEffect>, 3> effect;


	//プレイヤー位置を表すマップ番号
	XMINT3 mapChipNumberPos;
	//マップの中心にずらす値
	Vector3 shiftPos;
	//ゲームカメラ
	GameCamera* gameCamera = nullptr;
	//アクション
	ActionPhase actionPhase = ActionPhase::None;
	//アクション用タイマー
	int32_t actionTimer = 0;
	//ブロックのどの面移動するかフェーズ
	MoveSurfacePhase moveSurfacePhase = MoveSurfacePhase::Upward;
	//移動前座標
	Vector3 moveBeforePos;
	//移動目標座標
	Vector3 moveAfterPos;
	//ゴールしたか
	bool isGoal = false;
	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_ = static_cast<int>(GamePhase::Start);
#pragma region
	// 開始位置
	Vector3 playerStratPos_;
	// 終了位置
	Vector3 playerEndPos_;
	// イージングデータ
	std::unique_ptr<EaseData> easeData_;
#pragma endregion イージング関係

	//移動処理が終わったタイミング
	bool isMove;

	//最初の移動が終わったかのフラグ
	bool isStartMove;

	// メニューを開いているか
	bool menuFlag_ = false;
	// 行動が終わったか
	bool isCreateMove_ = false;
	// ヒントを拡大表示しているか
	bool hintFlag_ = false;
};