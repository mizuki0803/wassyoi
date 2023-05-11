#include "Block.h"
#include "Easing.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const Vector3& position)
{
    //�C���X�^���X����
    Block* instance = new Block();

	//���f�����Z�b�g
	assert(model);
	instance->model = model;

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	// �C�[�W���O�f�[�^�̐ݒ�
	instance->SetEaseData(90);
	//���W���Z�b�g
	instance->SetBlockStratPos(position);
	Vector3 temppos = position;
	temppos.y = -50.0f;
	instance->SetBlockEndPos(temppos);
	instance->position = position;
	//�傫�����Z�b�g
	instance->scale = { blockSize, blockSize, blockSize };

    return instance;
}

void Block::Update()
{
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
