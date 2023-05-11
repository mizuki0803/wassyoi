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
	instance->SetEaseData(90);
	//座標をセット
	instance->SetBlockStratPos(position);
	Vector3 temppos = position;
	temppos.y = -50.0f;
	instance->SetBlockEndPos(temppos);
	instance->position = position;
	//大きさをセット
	instance->scale = { blockSize, blockSize, blockSize };

    return instance;
}

void Block::Update()
{
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
