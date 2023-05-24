#pragma once
#include "SceneChangeSprite.h"
#include <list>
#include <memory>

/// <summary>
/// シーン変更演出
/// </summary>
class SceneChangeEffect
{
public: //静的メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	static void Update();

	/// <summary>
	/// 一斉描画
	/// </summary>
	static void Draw();

	/// <summary>
	/// シーン変更演出を作成する
	/// </summary>
	/// <param name="color">画面を覆う色</param>
	/// <param name="deepenTime">色を濃くする時間</param>
	/// <param name="waitTime">色を濃くしたまま待機する時間</param>
	/// <param name="returnTime">色を戻す時間</param>
	static void CreateSceneChangeEffect(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime);

private: //静的メンバ変数
	//シーン変更演出用スプライト
	static std::list<std::unique_ptr<SceneChangeSprite>> sceneChangeSprites;
};
