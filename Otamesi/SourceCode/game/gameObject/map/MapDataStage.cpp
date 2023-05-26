#include "MapDataStage.h"
#include "MapBlockData.h"

MapDataStage* MapDataStage::Create(const int selectStageNum)
{
	//�C���X�^���X�𐶐�
	MapDataStage* instance = new MapDataStage();

	//�}�b�v����
	if (!instance->CreateMap(selectStageNum)) {
		delete instance;
		assert(0);
		return nullptr;
	};

	return instance;
}

void MapDataStage::Update()
{
	if (!isBolckUp_)
	{
		// �����グ�邩�̌v�Z(�S�̂�30%)
		int UpBlockSize = static_cast<int>(blocks.size() * 30.0f / 100.0f);

		if (blockActTimer_ >= 10)
		{
			for (int i = 0; i < UpBlockSize; i++)
			{
				BlockCountCreate();
			}
			blockActTimer_ = 0;
		}
		blockActTimer_++;

		// ���������Ă���u���b�N�̐�
		int blockCount = 0;
		//�}�b�v�p�u���b�N
		for (const std::unique_ptr<StageBlock>& block : blocks) {
			// ����������������
			bool hitFlag = false;
			for (auto& cnt : rndcount)
			{
				if (cnt == blockCount)
				{
					hitFlag = true;
				}
			}
			blockCount++;
			if (!hitFlag)
			{
				continue;
			}
			block->Update();
		}

		// ����������������
		bool hitFlag = false;
		for (auto& block : blocks)
		{
			if (!block->IsEaseEndFlag())
			{
				hitFlag = true;
			}
		}

		if (!hitFlag)
		{
			isBolckUp_ = true;
		}
	}
	else
	{
		//�}�b�v�p�u���b�N
		for (const std::unique_ptr<StageBlock>& block : blocks) {
			block->Update();
		}
	}
}

void MapDataStage::Draw()
{
	//�`��
	//�}�b�v�p�u���b�N
	Block::Draw();
}

void MapDataStage::CreateMapBlock()
{
	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//�}�b�v�ԍ�
				const XMINT3 chipNum = { i, j, k };
				//�v���C���[�̐����p�}�b�v�ԍ���ۑ�
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Player) {
					playerCreateMapChipNum = chipNum;
				}
				//�S�[������
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Goal) {
					std::unique_ptr<StageBlock> newBlock;
					newBlock.reset(StageBlock::Create(2, chipNum));
					blocks.push_back(std::move(newBlock));
				}
				//�u���b�N����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Block) {
					std::unique_ptr<StageBlock> newBlock;
					newBlock.reset(StageBlock::Create(0, chipNum));
					blocks.push_back(std::move(newBlock));
				}
				//������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::UpPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = {};	//�X����p�x
					newBlock.reset(StageBlock::Create(1, chipNum));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::DownPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 180, 0, 0 };	//�X����p�x
					newBlock.reset(StageBlock::Create(1, chipNum));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::LeftPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, 90 };	//�X����p�x
					newBlock.reset(StageBlock::Create(1, chipNum));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�E�����n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::RightPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, -90 };	//�X����p�x
					newBlock.reset(StageBlock::Create(1, chipNum));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//���ʌ����n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::ForwardPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { -90, 0, 0 };	//�X����p�x
					newBlock.reset(StageBlock::Create(1, chipNum));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::AwayPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 90, 0, 0 };	//�X����p�x
					newBlock.reset(StageBlock::Create(1, chipNum));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
			}
		}
	}
}

void MapDataStage::BlockCountCreate()
{
	// size�������ɂȂ����瑁�����^�[��
	if (blocks.size() == rndcount.size())
	{
		return;
	}
	// �I�������
	bool endFlag = false;
	while (!endFlag)
	{
		// �����ɓ���������
		bool hitFlag = false;
		// �����_���ɐ��l�����߂�
		int temp = rand() % blocks.size();
		for (auto& count : rndcount)
		{
			// ���ɓ����������邩
			if (count == temp)
			{
				hitFlag = true;
				break;
			}
		}

		// ����������
		if (!hitFlag)
		{
			rndcount.push_back(temp);
			endFlag = true;
		}
	}
}