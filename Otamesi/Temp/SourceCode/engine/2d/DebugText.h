#pragma once
#include "Sprite.h"
#include <string>


/// <summary>
/// デバッグテキスト
/// </summary>
class DebugText final
{
public: //静的メンバ変数
	static const int maxCharCount = 256;	//1フレームでの最大表示文字数
	static const int fontWidth = 9;			//フォント画像内1文字分の横幅
	static const int fontHeight = 18;		//フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;	//フォント画像内1行分の文字数

private: //シングルトン化
	//コンストラクタを隠蔽
	DebugText() = default;
	//デストラクタを隠蔽
	~DebugText();
public:
	//コピーコンストラクタを無効化
	DebugText(const DebugText& debugText) = delete;
	//代入演算子を無効化
	void operator = (const DebugText& debugText) = delete;

public: //メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>デバッグテキスト</returns>
	static DebugText* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	void Initialize(const Texture& texture);

	/// <summary>
	/// テキスト文字入力
	/// </summary>
	/// <param name="text">テキスト</param>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="scale">大きさ</param>
	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	/// <summary>
	/// 一斉描画
	/// </summary>
	void DrawAll();

private: //メンバ変数
	//スプライトの配列
	Sprite* sprites[maxCharCount];
	//スプライト配列の添え字番号
	int spriteIndex = 0;
};