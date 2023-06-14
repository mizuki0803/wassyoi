#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Sprite.h"

namespace
{
	//フェードタイプ
	enum class TYPE
	{
		FadeOut = -1,
		Not = 0,
		FadeIn = 1,
	};

	//フェードタイプ
	enum class SIZEMAX
	{
		Width = 32,
		Height = 18,
	};

	class BlackBox
	{
	public: // メンバ関数
		/// <summary>
		/// 動作
		/// </summary>
		/// <param name="type">フェードタイプ</param>
		void Spin(TYPE type);
		/// <summary>
		/// オフセットタイマーの設定
		/// </summary>
		/// <param name="num"></param>
		void AddOffsetTimer(int num) { offsetTimer_ += num; }

	public: // メンバ関数
		/// <summary>
		/// オフセットタイマーの取得
		/// </summary>
		/// <returns>オフセットタイマー</returns>
		int GetOffsetTimer() { return offsetTimer_; }

	public: // メンバ変数
		std::unique_ptr<Sprite> sprite_;
		int offsetTimer_ = 0;
		// イージングの進行度用
		float easeTimer_ = 0.0f;
		bool inEnd_ = false;
		bool outEnd_ = false;
	};
}

class SceneChange
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // サブクラス


public: // メンバ関数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	SceneChange();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// シーンチェンジ開始
	/// </summary>
	/// <param name="sceneName">シーン名</param>
	void SceneChangeStart(const std::string& sceneName);

public: // メンバ関数
	/// <summary>
	/// シーンイン終了フラグの取得
	/// </summary>
	/// <returns>シーンイン終了フラグ</returns>
	bool GetinEndFlag() { return inEndFlag_; }

private: // メンバ変数
	bool inEndFlag_ = false;
	bool outEndFlag_ = false;

	std::vector<std::vector<std::unique_ptr<BlackBox>>> blackBox_;
	TYPE type_;
	std::string sceneName_;
};