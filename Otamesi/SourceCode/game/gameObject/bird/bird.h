#pragma once
#include "BirdBody.h"
#include "birdWing.h"
#include <memory>
#include <array>

/// <summary>
/// 鳥
/// </summary>
class Bird
{
public: //enum
	/// <summary>
	/// 生涯フェーズ
	/// </summary>
	enum LifePhase
	{
		Born,	//生まれる
		Alive,	//生きている
		Fadeout	//消えていく
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="velocity">移動速度</param>
	/// <returns>鳥</returns>
	static Bird* Create(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="velocity">移動速度</param>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	bool GetIsDead() { return isDead; }

private: //メンバ関数
	/// <summary>
	/// 産まれる
	/// </summary>
	void BornAction();

	/// <summary>
	/// 死亡確認
	/// </summary>
	void DeadCheck();

	/// <summary>
	/// 消えていく
	/// </summary>
	void FadeoutAction();

private: //静的メンバ変数
	//死亡する座標限界
	static float deadLine;
	//生涯フェーズの行動関数
	static void (Bird::* lifePhasefuncTable[])();

private: //メンバ変数
	//胴体
	std::unique_ptr<BirdBody> body;
	//翼
	std::array<std::unique_ptr<BirdWing>, 2> wings;
	//生涯フェーズ
	LifePhase lifePhase = LifePhase::Born;
	//行動タイマー
	int16_t actionTimer = 0;
	//死亡フラグ
	bool isDead = false;
};