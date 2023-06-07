#include "StageBlock.h"
#include "Easing.h"
#include <random>

StageBlock* StageBlock::Create(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos)
{
	//�C���X�^���X����
	StageBlock* instance = new StageBlock();

	//����������
	if (!instance->Initialize(_blockType, mapChipNum, shiftPos)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageBlock::Initialize(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos)
{
	//�u���b�N�̎�ނ��u�u���b�N�v�ɐݒ�
	blockType = BROCK_TYPE(_blockType);

	// �C�[�W���O�f�[�^�̐ݒ�
	SetEaseData(60);
	vecEaseData_ = std::make_unique<EaseData>(60);

	//���W���Z�b�g
	Vector3 temppos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
	temppos -= shiftPos;
	SetBlockEndPos(temppos);
	temppos.y = -100.0f;
	SetBlockStratPos(temppos);
	position = temppos;
	//�傫�����Z�b�g
	scale = { blockSize, blockSize, blockSize };
	// �֐��̐ݒ�
	CreateAct();
	// �֐��̃^�C�v�̐ݒ�
	SetGamePhase(GamePhase::Start);

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
	// �C�[�W���O�̌v�Z
	position.x = Easing::InOutBack(blockStratPos_.x, blockEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::InOutBack(blockStratPos_.y, blockEndPos_.y, easeData_->GetTimeRate());
	position.z = Easing::InOutBack(blockStratPos_.z, blockEndPos_.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::None);
	}

	// �C�[�W���O�f�[�^�X�V
	easeData_->Update();
}

void StageBlock::ReStratMove()
{
	// �C�[�W���O�̌v�Z
	position.x = Easing::InCubic(blockStratPos_.x, blockEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::InCubic(blockStratPos_.y, blockEndPos_.y, easeData_->GetTimeRate()) + Easing::InCubic(vec_.y, 0.0f, vecEaseData_->GetTimeRate());
	position.z = Easing::InCubic(blockStratPos_.z, blockEndPos_.z, easeData_->GetTimeRate()) + Easing::InCubic(vec_.z, 0.0f, vecEaseData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::None);
	}

	// �C�[�W���O�f�[�^�X�V
	vecEaseData_->Update();
	easeData_->Update();
}

void StageBlock::DeleteMove()
{
	// �C�[�W���O�̌v�Z
	position.x = Easing::InCubic(blockStratPos_.x, blockEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::InQuart(blockStratPos_.y, blockEndPos_.y, easeData_->GetTimeRate()) + Easing::InCubic(vec_.y, 0.0f, vecEaseData_->GetTimeRate());
	position.z = Easing::InCubic(blockStratPos_.z, blockEndPos_.z, easeData_->GetTimeRate()) + Easing::InCubic(vec_.z, 0.0f, vecEaseData_->GetTimeRate());

	rotation.x += vec_.x;
	rotation.y += vec_.y;
	rotation.z += vec_.z;

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::None);
		deleteFlag_ = true;
	}

	// �C�[�W���O�f�[�^�X�V
	vecEaseData_->Update();
	easeData_->Update();
}

void StageBlock::ReCreateMove()
{
	// �C�[�W���O�̌v�Z
	position.x = Easing::OutBack(blockStratPos_.x, blockEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::OutBack(blockStratPos_.y, blockEndPos_.y, easeData_->GetTimeRate());
	position.z = Easing::OutBack(blockStratPos_.z, blockEndPos_.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::None);
	}

	// �C�[�W���O�f�[�^�X�V
	easeData_->Update();
}

void StageBlock::ReCreate(const GamePhase phase, const XMINT3& mapChipNum)
{
	if (phase == GamePhase::None || phase == GamePhase::Start)
	{
		return;
	}

	float speed = 1.5f;

	if (phase == GamePhase::Move)
	{
		//���W���Z�b�g
		Vector3 temppos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
		SetBlockStratPos(position);
		temppos.x += 100.0f;
		SetBlockEndPos(temppos);
		SetEaseData(120);
	}
	else if (phase == GamePhase::Delete)
	{
		//���W���Z�b�g
		Vector3 temppos = position;
		SetBlockStratPos(temppos);
		temppos.x +=  100.0f;
		temppos.y += -100.0f;
		SetBlockEndPos(temppos);
		SetEaseData(120);
	}
	else if (phase == GamePhase::ReStart)
	{
		Vector3 temppos = { mapChipNum.x * blockSize, mapChipNum.y * blockSize, mapChipNum.z * blockSize };
		SetBlockEndPos(temppos);
		temppos.x +=  100.0f;
		temppos.y += -100.0f;
		SetBlockStratPos(temppos);
		position = temppos;
		SetEaseData(30);
	}

	vec_ = { RandCalculate(-speed, speed), RandCalculate(-speed, speed), RandCalculate(-speed, speed) };
	vecEaseData_->Reset();
	phase_ = static_cast<int>(phase);
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
	func_.push_back([this] { return ReStratMove(); });
	func_.push_back([this] { return DeleteMove(); });
	func_.push_back([this] { return ReCreateMove(); });
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

const float StageBlock::RandCalculate(float a, float b)
{
	// �����Z���k�E�c�C�X�^�[�@�ɂ��[��������������A
	// �n�[�h�E�F�A�������V�[�h�ɂ��ď�����
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	// ��l�������z
	// [a, b)�̒l�͈̔͂ŁA���m���Ɏ����𐶐�����
	std::uniform_real_distribution<> dist1(a, b);

	return float(dist1(engine));
}
