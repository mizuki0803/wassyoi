#include "DebugText.h"

using namespace DirectX;
using namespace Microsoft::WRL;

DebugText* DebugText::GetInstance()
{
	static DebugText instance;

	return &instance;
}

DebugText::~DebugText()
{
	//全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites); i++) {
		delete sprites[i];
	}
}

void DebugText::Initialize(const Texture& texture)
{
	//全てのスプライトデータについて
	for (int i = 0; i < _countof(sprites); i++) {
		//スプライトを生成する(アンカーポイントを{ 0, 0 }に設定)
		sprites[i] = Sprite::Create(texture, { 0, 0 });
	}
}

void DebugText::Print(const std::string& text, float x, float y, float scale)
{
	//全ての文字について
	for (int i = 0; i < text.size(); i++) {
		//最大文字数を超過していたら抜ける
		if (spriteIndex >= maxCharCount) { break; }

		//1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		//ASCIIコードの1段分飛ばした番号を計算
		const int ASCIISkipNum = 32;
		int fontIndex = character - ASCIISkipNum;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		//座標計算
		sprites[spriteIndex]->SetPosition({ x + fontWidth * scale * i, y });
		sprites[spriteIndex]->SetTexLeftTop({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight });
		sprites[spriteIndex]->SetTexSize({ (float)fontWidth, (float)fontHeight });
		sprites[spriteIndex]->SetSize({ fontWidth * scale, fontHeight * scale });
		//更新
		sprites[spriteIndex]->Update();
		//文字を1つ進める
		spriteIndex++;
	}
}

//まとめて描画
void DebugText::DrawAll()
{
	//全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++) {
		//スプライト描画
		sprites[i]->Draw();
	}

	//文字数をリセットしておく
	spriteIndex = 0;
}
