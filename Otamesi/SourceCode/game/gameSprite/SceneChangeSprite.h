#pragma once
#include "Sprite.h"

/// <summary>
/// シーン変更演出用スプライト
/// </summary>
class SceneChangeSprite : public Sprite
{
public:
	//色の濃さ変更フェーズ
	enum class IntensityChangePhase {
		Deepen, //濃くする
		Wait,	//色を濃くしたまま待機する
		Return, //薄くする(戻す)
	};

public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="color">画面を覆う色</param>
	/// <param name="deepenTime">色を濃くしていく時間</param>
	/// <param name="waitTime">色を濃くしたまま待機する時間</param>
	/// <param name="returnTime">色を薄くしていく時間</param>
	/// <returns>シーン変更演出用スプライト</returns>
	static SceneChangeSprite* Create(const XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDead() { return isDead; }


private: //メンバ関数
	/// <summary>
	/// 色を濃くする
	/// </summary>
	void ColorDeepen();

	/// 色を濃くしたまま待機する
	/// </summary>
	void ColorWait();

	/// <summary>
	/// 濃くした色を戻す
	/// </summary>
	void ColorReturn();


private: //静的メンバ変数
	//色の濃さ変更挙動遷移
	static void (SceneChangeSprite::* intensityChangeActionFuncTable[])();

private: //メンバ変数
	//色の濃さ変更フェーズ
	IntensityChangePhase phase = IntensityChangePhase::Deepen;
	//色を濃くする時間
	int32_t deepenTime = 0;
	//色を濃くしたまま待機する時間
	int32_t waitTime = 0;
	//濃くした色を戻す時間
	int32_t returnTime = 0;
	//色の濃さを変更するタイマー
	int32_t intensityChangeTimer = 0;
	//死亡フラグ
	bool isDead = false;
};