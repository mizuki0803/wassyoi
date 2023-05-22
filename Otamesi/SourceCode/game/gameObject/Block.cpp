#include "Block.h"
#include "Easing.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const XMINT3& mapChipNum)
{
    //�C���X�^���X����
    Block* instance = new Block();

	//����������
	if (!instance->Initialize(model, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//�u���b�N�̎�ނ��u�u���b�N�v�ɐݒ�
	instance->blockType = BlockType::Block;

    return instance;
}

bool Block::Initialize(ObjModel* model, const XMINT3& mapChipNum)
{
	//���f�����Z�b�g
	assert(model);
	this->model = model;

	//object3D�̏�����
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	// �C�[�W���O�f�[�^�̐ݒ�
	SetEaseData(60);

	//���W���Z�b�g
	Vector3 temppos = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
	SetBlockEndPos(temppos);
	temppos.y = -100.0f;
	SetBlockStratPos(temppos);
	position = temppos;
	//�傫�����Z�b�g
	scale = { blockSize, blockSize, blockSize };
	// �֐��̐ݒ�
	CreateAct();

	return true;
}

void Block::Update()
{
	if (phase_ != static_cast<int>(GamePhase::None))
	{
		func_[phase_]();
	}
	//�I�u�W�F�N�g�X�V
	ObjObject3d::Update();
}

void Block::PlayStratMove()
{
	Vector3 easePos = {};
	// �C�[�W���O�̌v�Z
	easePos.x = Easing::InOutBack(blockStratPos_.x, blockEndPos_.x, easeData_->GetTimeRate());
	easePos.y = Easing::InOutBack(blockStratPos_.y, blockEndPos_.y, easeData_->GetTimeRate());
	easePos.z = Easing::InOutBack(blockStratPos_.z, blockEndPos_.z, easeData_->GetTimeRate());
	// �ʒu�̐ݒ�
	SetPosition(easePos);

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::None);
	}

	// �C�[�W���O�f�[�^�X�V
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
