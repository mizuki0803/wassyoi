#pragma once
#include "PostEffect.h"
#include <memory>

/// <summary>
/// ゲームで使用するオブジェクト内で変更可能なポストエフェクトクラス
/// </summary>
class GamePostEffect
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 描画
	/// </summary>
	static void Draw();

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	static void DrawScenePrev();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	static void DrawSceneRear();

	//getter
	static PostEffect* GetPostEffect() { return GamePostEffect::postEffect.get(); }

private: //静的メンバ変数
	//ポストエフェクト
	static std::unique_ptr<PostEffect> postEffect;
};