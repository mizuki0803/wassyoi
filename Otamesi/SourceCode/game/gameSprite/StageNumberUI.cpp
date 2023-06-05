#include "StageNumberUI.h"
#include "SpriteTextureLoader.h"

StageNumberUI* StageNumberUI::Create(const Vector2& position, const float size, const int stageNum)
{
	//インスタンスを生成
	StageNumberUI* instance = new StageNumberUI();

	// 初期化
	if (!instance->Initialize(position, size, stageNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageNumberUI::Initialize(const Vector2& position, const float size, const int stageNum)
{
	//表示用サイズをセット
	const Vector2 texSize = { 32, 48 };
	const Vector2 showsize = texSize * size;
	
	//桁数の分、数字スプライト生成
	for (int i = 0; i < digitNum; i++) {
		const Vector2 pos = { position.x + texSize.x + ((digitNum - i) * showsize.x), position.y };
		numbers[i].reset(NumberSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Number), pos, showsize, texSize));
	}

	//それぞれの桁数に合った数字を割り当てる
	ChengeStageNum(stageNum);

	return true;
}

void StageNumberUI::Update()
{
	//スプライト更新
	for (int i = 0; i < digitNum; i++) {
		numbers[i]->Update();
	}
}

void StageNumberUI::Draw()
{
	//スプライト描画
	for (int i = 0; i < digitNum; i++) {
		numbers[i]->Draw();
	}
}

void StageNumberUI::ChengeStageNum(const int stageNum)
{
	//それぞれの桁数に合った数字を割り当てる
	for (int i = 0; i < digitNum; i++) {
		//割る数
		int divisionNum = 1;
		//桁数分10を乗算する(1,10,100,...)
		for (int j = 0; j < i; j++) {
			divisionNum *= 10;
		}
		//指定した桁の数字を出す
		int numberDigit = (stageNum / divisionNum) % 10;
		//算出した桁の数字を数字スプライトに割り当てる
		numbers[i]->SetNumber(numberDigit);
	}
}
