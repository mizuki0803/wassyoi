#include "StageBlock.h"
#include "Easing.h"

StageBlock* StageBlock::Create(const int _blockType, const XMINT3& mapChipNum)
{
	//インスタンス生成
	StageBlock* instance = new StageBlock();

	//初期化処理
	if (!instance->Initialize(_blockType, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageBlock::Initialize(const int _blockType, const XMINT3& mapChipNum)
{
	//ブロックの種類を「ブロック」に設定
	blockType = BROCK_TYPE(_blockType);

	// イージングデータの設定
	SetEaseData(60);

	//座標をセット
	Vector3 temppos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
	SetBlockEndPos(temppos);
	temppos.y = -100.0f;
	SetBlockStratPos(temppos);
	position = temppos;
	//大きさをセット
	scale = { blockSize, blockSize, blockSize };
	// 関数の設定
	CreateAct();

	return true;
}

void StageBlock::Update()
{
	if (phase_ != static_cast<int>(GamePhase::None))
	{
		func_[phase_]();
	}

	Block::Update();
}

void StageBlock::PlayStratMove()
{
	// イージングの計算
	position.x = Easing::InOutBack(blockStratPos_.x, blockEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::InOutBack(blockStratPos_.y, blockEndPos_.y, easeData_->GetTimeRate());
	position.z = Easing::InOutBack(blockStratPos_.z, blockEndPos_.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::None);
	}

	// イージングデータ更新
	easeData_->Update();
}

void StageBlock::SetEaseData(const int count)
{
	if (easeData_ == nullptr)
	{
		easeData_ = std::make_unique<EaseData>(count);
	}
	else
	{
		easeData_->SetCount(count);
	}
}

void StageBlock::CreateAct()
{
    func_.push_back([this] { return PlayStratMove(); });
}

void StageBlock::ReAction()
{
	switch (blockType)
	{
	case Block::BROCK_TYPE::block:
		break;
	case Block::BROCK_TYPE::plane:
		break;
	case Block::BROCK_TYPE::goal:
		break;
	case Block::BROCK_TYPE::size:
		break;
	default:
		break;
	}
}
