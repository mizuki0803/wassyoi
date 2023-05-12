#include "Block.h"
#include "Easing.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const Vector3& position)
{
    //インスタンス生成
    Block* instance = new Block();

	//モデルをセット
	assert(model);
	instance->model = model;

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	// イージングデータの設定
	instance->SetEaseData(60);
	//座標をセット
	
	Vector3 temppos = position;
	temppos.y = -100.0f;
	instance->SetBlockStratPos(temppos);
	instance->SetBlockEndPos(position);
	instance->position = temppos;
	//大きさをセット
	instance->scale = { blockSize, blockSize, blockSize };
	// 関数の設定
	instance->CreateAct();

    return instance;
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
