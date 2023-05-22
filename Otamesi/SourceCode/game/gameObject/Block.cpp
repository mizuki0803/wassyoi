#include "Block.h"
#include "Easing.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const XMINT3& mapChipNum)
{
    //インスタンス生成
    Block* instance = new Block();

	//初期化処理
	if (!instance->Initialize(model, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//ブロックの種類を「ブロック」に設定
	instance->blockType = BlockType::Block;

    return instance;
}

bool Block::Initialize(ObjModel* model, const XMINT3& mapChipNum)
{
	//モデルをセット
	assert(model);
	this->model = model;

	//object3Dの初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	// イージングデータの設定
	SetEaseData(60);

	//座標をセット
	Vector3 temppos = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
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

void Block::Update()
{
	if (phase_ != static_cast<int>(GamePhase::None))
	{
		func_[phase_]();
	}
	//オブジェクト更新
	ObjObject3d::Update();
}

void Block::PlayStratMove()
{
	Vector3 easePos = {};
	// イージングの計算
	easePos.x = Easing::InOutBack(blockStratPos_.x, blockEndPos_.x, easeData_->GetTimeRate());
	easePos.y = Easing::InOutBack(blockStratPos_.y, blockEndPos_.y, easeData_->GetTimeRate());
	easePos.z = Easing::InOutBack(blockStratPos_.z, blockEndPos_.z, easeData_->GetTimeRate());
	// 位置の設定
	SetPosition(easePos);

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::None);
	}

	// イージングデータ更新
	easeData_->Update();
}

void Block::SetEaseData(const int count)
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

void Block::CreateAct()
{
	func_.push_back([this] { return PlayStratMove(); });
}
