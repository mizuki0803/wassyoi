#include "MapDataStageSelect.h"
#include "MapBlockData.h"
#include "StageManager.h"

MapDataStageSelect* MapDataStageSelect::Create(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase)
{
	//�C���X�^���X�𐶐�
	MapDataStageSelect* instance = new MapDataStageSelect();

	//�}�b�v����
	if (!instance->Initialize(selectStageNum, positionPhase)) {
		delete instance;
		assert(0);
		return nullptr;
	};


	return instance;
}

bool MapDataStageSelect::Initialize(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase)
{
	//�u���b�N�Ǘ�����
	blockManager.reset(StageSelectBlockManager::Create(positionPhase));

	//�}�b�v����
	CreateMap(selectStageNum);

	//�I�����Ă���X�e�[�W���N���A���Ă��Ȃ���΃u���b�N��^���Âɂ���
	if (!StageManager::GetIsClear(selectStageNum)) {
		for (const std::unique_ptr<StageSelectBlock>& block : blocks) {
			const XMFLOAT4 black = { 0,0,0,1 };
			block->SetColor(black);
		}
	}

	return true;
}

void MapDataStageSelect::Update()
{
	//�u���b�N�Ǘ�
	blockManager->Update();

	//�u���b�N
	for (const std::unique_ptr<StageSelectBlock>& block : blocks) {
		block->Update();
	}
}

void MapDataStageSelect::Draw()
{
	//�u���b�N�`��
	Block::Draw();
}

void MapDataStageSelect::FrameReset()
{
	for (auto& i : blocks) {
		i->FrameReset();
	}
}

void MapDataStageSelect::CreateMapBlock()
{
	//�u���b�N����
	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//�}�b�v�ԍ�
				const XMINT3 chipNum = { i, j, k };
				//�v���C���[�̐����p�}�b�v�ԍ���ۑ�
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Player) {
					std::unique_ptr<StageSelectBlock> newBlock;
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::player), chipNum, shiftPos, blockManager.get()));
					blocks.push_back(std::move(newBlock));
				}
				//�S�[������
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Goal) {
					std::unique_ptr<StageSelectBlock> newBlock;
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::goal), chipNum, shiftPos, blockManager.get()));
					blocks.push_back(std::move(newBlock));
				}
				//�u���b�N����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Block) {
					std::unique_ptr<StageSelectBlock> newBlock;
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::block), chipNum, shiftPos, blockManager.get()));
					blocks.push_back(std::move(newBlock));
				}
				//������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::UpPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = {};	//�X����p�x
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::DownPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 180, 0, 0 };	//�X����p�x
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::LeftPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, 90 };	//�X����p�x
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�E�����n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::RightPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, -90 };	//�X����p�x
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//���ʌ����n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::ForwardPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { -90, 0, 0 };	//�X����p�x
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::AwayPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 90, 0, 0 };	//�X����p�x
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
			}
		}
	}
}
