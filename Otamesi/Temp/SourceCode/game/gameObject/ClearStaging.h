#pragma once
#include "Sprite.h"
#include "EaseData.h"
#include <memory>
#include <vector>
#include <functional>

class ClearStaging
{
public: //enum
	/// <summary>
	/// 全体流れ
	/// </summary>
	enum class MovePhase
	{
		Start,			//開始
		intermediate1,	//移動
		intermediate2,	//削除
		End,			//終了
		None,	//何もしない
	};

public: //メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="pos">位置</param>
	/// <returns></returns>
	static std::unique_ptr<ClearStaging> Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// フラグの設定
	/// </summary>
	/// <param name="flag">フラグ</param>
	void SetPushFlag(bool flag) { pushFlag_ = flag; }
	/// <summary>
	/// 行動の設定
	/// </summary>
	/// <param name="movePhase">行動</param>
	void SetMovePhase(MovePhase movePhase) { phase_ = static_cast<int>(movePhase); }

private: //メンバ関数
	/// <summary>
	/// 初動
	/// </summary>
	void StartMove();
	/// <summary>
	/// 中間の動き1
	/// </summary>
	void Intermediate1Move();
	/// <summary>
	/// 中間の動き2
	/// </summary>
	void Intermediate2Move();
	/// <summary>
	/// 終了の動き
	/// </summary>
	void EndMove();

private: //メンバ変数
	// イージング
	std::unique_ptr<EaseData> easeData_;
	// 背景
	std::unique_ptr<Sprite> backScreen_;
	// テキスト
	std::unique_ptr<Sprite> textSprite_;
	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_;
	// ボタンを押したか
	bool pushFlag_ = false;
};

