#pragma once
#include "Sprite.h"

/// <summary>
/// 音量変更用ポインタースプライト
/// </summary>
class SoundVolumePointer : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="barLeftPos">音量バー左端座標</param>
	/// <param name="barLength">音量バーの長さ</param>
	/// <param name="startPercentage">生成時の音量パーセンテージ</param>
	/// <returns>音量変更用ポインタースプライト</returns>
	static SoundVolumePointer* Create(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="barLeftPos">音量バー左端座標</param>
	/// <param name="barLength">音量バーの長さ</param>
	/// <param name="startPercentage">生成時の音量パーセンテージ</param>
	/// <returns>成否</returns>
	bool Initialize(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage);

	/// <summary>
	/// 音量割合変更
	/// </summary>
	void SetPercentage(float percentage);

private: //メンバ変数
	//バーの左端座標
	Vector2 barLeftPos;
	//バーの長さ
	float barLength;
};