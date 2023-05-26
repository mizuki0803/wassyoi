#include "Block.h"

const float Block::blockSize = 5.0f;
std::unique_ptr<BaseBlock> Block::instanceBlock;

void Block::StaticInitialize()
{
	instanceBlock = std::make_unique<BaseBlock>();
	instanceBlock->StaticInitialize();
}

Block* Block::Create(const int _blockType, const XMINT3& mapChipNum)
{
	//�C���X�^���X����
	Block* instance = new Block();

	//����������
	if (!instance->Initialize(_blockType, mapChipNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

void Block::Draw()
{
	instanceBlock->Draw();
}

bool Block::Initialize(const int _blockType, const XMINT3& mapChipNum)
{
	//�u���b�N�̎�ނ��u�u���b�N�v�ɐݒ�
	blockType = BROCK_TYPE(_blockType);

	//���W���Z�b�g
	position = { mapChipNum.x * Block::GetBlockSize(), mapChipNum.y * Block::GetBlockSize(), mapChipNum.z * Block::GetBlockSize() };
	//�傫�����Z�b�g
	scale = { blockSize, blockSize, blockSize };

	return true;
}

void Block::Update()
{
	if (parent != nullptr) {
		instanceBlock->Update(int(blockType), { position.x,position.y,position.z }, scale, rotation, color, &parent->GetMatWorld());
	} else {
		instanceBlock->Update(int(blockType), { position.x,position.y,position.z }, scale, rotation, color);
	}
}
