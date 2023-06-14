#include "SceneChange.h"
#include "Easing.h"
#include "SpriteTextureLoader.h"

SceneChange::SceneChange()
{
	type_ = TYPE::FadeIn;

	// アンカーポイント
	XMFLOAT2 anchorPoint = { 0.5f, 0.5f };
	// 色
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (int x = 0; x < static_cast<int>(SIZEMAX::Width); x++)
	{
		std::vector<std::unique_ptr<BlackBox>> tempX;
		for (int y = 0; y < static_cast<int>(SIZEMAX::Height); y++)
		{
			std::unique_ptr<BlackBox> tempY = std::make_unique<BlackBox>();

			tempY->sprite_ = std::unique_ptr<Sprite>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::MenuBackScreen)));
			tempY->sprite_->SetPosition(Vector2(static_cast<float>(20 + x * 40), static_cast<float>(20 + y * 40)));
			tempY->offsetTimer_ = (31 - x + y);

			tempX.push_back(std::move(tempY));
		}
		blackBox_.push_back(std::move(tempX));
	}
}

void SceneChange::Update()
{
	if (type_ == TYPE::Not)
	{
		return;
	}

	for (int x = 0; x < blackBox_.size(); x++)
	{
		for (int y = 0; y < blackBox_[x].size(); y++)
		{
			if (blackBox_[x][y]->GetOffsetTimer() <= 0)
			{
				blackBox_[x][y]->Spin(type_);
			}
			else
			{
				blackBox_[x][y]->AddOffsetTimer(-1);
			}

		}
	}

	//末端がtrueならそれを全体に適応
	if (blackBox_[0][17]->inEnd_)
	{
		inEndFlag_ = true;
	}
	if (blackBox_[0][17]->outEnd_)
	{
		outEndFlag_ = true;
		//
	}
}

void SceneChange::Draw()
{
	if (type_ == TYPE::Not)
	{
		return;
	}

	for (int x = 0; x < blackBox_.size(); x++)
	{
		for (int y = 0; y < blackBox_[x].size(); y++)
		{
			blackBox_[x][y]->sprite_->Draw();
		}
	}
}

void SceneChange::SceneChangeStart(const std::string& sceneName)
{
	if (type_ != TYPE::FadeOut)
	{
		outEndFlag_ = false;

		for (int x = 0; x < blackBox_.size(); x++)
		{
			for (int y = 0; y < blackBox_[x].size(); y++)
			{
				blackBox_[x][y]->sprite_->SetRotation(0.0f);
				blackBox_[x][y]->offsetTimer_ = (31 - x + y);
			}
		}
	}
	type_ = TYPE::FadeOut;

	sceneName_ = sceneName;
}

void BlackBox::Spin(TYPE type)
{
	//終了していれば処理を省略
	if (type == TYPE::FadeIn && inEnd_ || type == TYPE::FadeOut && outEnd_)
	{
		return;
	}

	float timeRate = 0.0f;
	int countNum = 60;
	timeRate = easeTimer_ / countNum;
	easeTimer_++;

	//縮小
	float scale = 0.0f;
	if (type == TYPE::FadeIn)
	{
		scale = Easing::InOutQuart(1.0f, 0.0f, timeRate);
	}
	else if (type == TYPE::FadeOut)
	{
		scale = Easing::InOutQuart(0.0f, 1.0f, timeRate);
	}

	//見えなくなったら終了
	if (type == TYPE::FadeIn && easeTimer_ > countNum)
	{
		scale = 0.0f;
		easeTimer_ = 0.0f;
		inEnd_ = true;
	}
	else if (type == TYPE::FadeOut && easeTimer_ > countNum)
	{
		scale = 1.0f;
		easeTimer_ = 0.0f;
		outEnd_ = true;
	}

	sprite_->SetScale(scale);
}