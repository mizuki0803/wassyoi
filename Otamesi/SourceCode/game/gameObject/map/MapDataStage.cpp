#include "MapDataStage.h"
#include "MapBlockData.h"
#include "ParticleEmitter.h"
#include <sstream>

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

	instance->CreateAct();

	return instance;
}

void MapDataStage::Update()
{
	func_[phase_]();
}

void MapDataStage::Draw()
{
	//�`��
	//�}�b�v�p�u���b�N
	Block::Draw();
}

void MapDataStage::PlayGame()
{
	if (isAllStageClear && selectStageNum >= 100)
	{
		int count = 0;
		int count2 = 0;
		for (const std::unique_ptr<StageBlock>& block : blocks) {
			
			if (count2 <= count)
			{
				ParticleEmitter::GetInstance()->ItemShine(block->GetPosition(), 30.0f, 1);
				count2 += 10;
			}

			count++;
		}
	}

	//�}�b�v�p�u���b�N
	for (const std::unique_ptr<StageBlock>& block : blocks) {
		block->Update();
	}
}

void MapDataStage::GameStart()
{
	isMoveEnd_ = true;
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
	int count = 0;
	for (auto& block : blocks)
	{
		if (!block->IsEaseEndFlag())
		{
			hitFlag = true;
		}
	}

	if (!hitFlag)
	{
		isMoveEnd_ = false;
		phase_ = static_cast<int>(GamePhase::GamePlay);
	}
}

void MapDataStage::GameReStart()
{
	isMoveEnd_ = true;
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

		if (!hitFlag && (block->GetActPhase() == StageBlock::GamePhase::Move || block->GetActPhase() == StageBlock::GamePhase::Delete))
		{
			hitFlag = true;
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
	int count = 0;
	for (auto& block : blocks)
	{
		if (!block->IsEaseEndFlag())
		{
			hitFlag = true;
		}
	}

	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->GetDeleteFlag())
		{
			blocks.erase(blocks.begin() + i);
		}
	}

	if (!hitFlag)
	{
		isMoveEnd_ = false;
		phase_ = static_cast<int>(GamePhase::GamePlay);
		isReCreateEnd = true;
	}
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
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::goal), chipNum, shiftPos, MapBlockData::MapBlockType::Goal));
					blocks.push_back(std::move(newBlock));
				}
				//�u���b�N����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Block) {
					std::unique_ptr<StageBlock> newBlock;
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::block), chipNum, shiftPos, MapBlockData::MapBlockType::Block));
					blocks.push_back(std::move(newBlock));
				}
				//������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::UpPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = {};	//�X����p�x
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, MapBlockData::MapBlockType::UpPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::DownPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 180, 0, 0 };	//�X����p�x
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, MapBlockData::MapBlockType::DownPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::LeftPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, 90 };	//�X����p�x
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, MapBlockData::MapBlockType::LeftPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�E�����n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::RightPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, -90 };	//�X����p�x
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, MapBlockData::MapBlockType::RightPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//���ʌ����n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::ForwardPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { -90, 0, 0 };	//�X����p�x
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, MapBlockData::MapBlockType::ForwardPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//�������n���{�e����
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::AwayPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 90, 0, 0 };	//�X����p�x
					newBlock.reset(StageBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, MapBlockData::MapBlockType::AwayPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
			}
		}
	}
}

void MapDataStage::CreateAct()
{
	func_.push_back([this] { return PlayGame(); });
	func_.push_back([this] { return GameStart(); });
	func_.push_back([this] { return GameReStart(); });
}

void MapDataStage::ReCreateMapBlock(const int selectStageNum)
{
	mapChipNum.clear();
	//json�}�b�v�f�[�^�ǂݍ���
	std::ostringstream  fileName;
	fileName << selectStageNum;
	LoadMapData(fileName.str());

	int count = 0;
	int oldSize = static_cast<int>(blocks.size());

	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//�����Ȃ����̓��^�[��
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::None)
				{
					continue;
				}

				//�}�b�v�ԍ�
				const XMINT3 chipNum = { i, j, k };
				
				//�v���C���[�̐����p�}�b�v�ԍ���ۑ�
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Player) {
					playerCreateMapChipNum = chipNum;
					continue;
				}

				if (blocks.size() > count)
				{
					BlockCreate(static_cast<MapBlockData::MapBlockType>(mapChipNum[i][j][k]), chipNum, count);
				}
				else
				{
					BlockAdd(static_cast<MapBlockData::MapBlockType>(mapChipNum[i][j][k]), chipNum);
				}

				count++;
			}
		}
	}

	for (int i = count; i < blocks.size(); i++)
	{
		blocks[i]->ReCreate(StageBlock::GamePhase::Delete, {}, shiftPos);
	}
	rndcount.clear();
	phase_ = static_cast<int>(GamePhase::ReStart);
}

void MapDataStage::FrameReset()
{
	for (auto& i : blocks) {
		i->FrameReset();
	}
}

void MapDataStage::BlockCreate(const MapBlockData::MapBlockType type, const XMINT3 chipNum, const int count)
{
	std::unique_ptr<StageBlock> newBlock;

	if (type == static_cast<int>(blocks[count]->GetMapBlockType()))
	{
		blocks[count]->ReCreate(StageBlock::GamePhase::Move, chipNum, shiftPos);
	}
	//�S�[������
	else if (type == MapBlockData::MapBlockType::Goal) {
		newBlock.reset(StageBlock::Create(2, chipNum, shiftPos, MapBlockData::MapBlockType::Goal));

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
	//�u���b�N����
	else if (type == MapBlockData::MapBlockType::Block) {
		newBlock.reset(StageBlock::Create(0, chipNum, shiftPos, MapBlockData::MapBlockType::Block));

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
	//������n���{�e����
	else if (type == MapBlockData::MapBlockType::UpPlane) {
		const XMFLOAT3 rot = {};	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::UpPlane));
		newBlock->SetRotation(rot);

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
	//�������n���{�e����
	else if (type == MapBlockData::MapBlockType::DownPlane) {
		const XMFLOAT3 rot = { 180, 0, 0 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::DownPlane));
		newBlock->SetRotation(rot);

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
	//�������n���{�e����
	else if (type == MapBlockData::MapBlockType::LeftPlane) {
		const XMFLOAT3 rot = { 0, 0, 90 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::LeftPlane));
		newBlock->SetRotation(rot);

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
	//�E�����n���{�e����
	else if (type == MapBlockData::MapBlockType::RightPlane) {
		const XMFLOAT3 rot = { 0, 0, -90 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::RightPlane));
		newBlock->SetRotation(rot);

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
	//���ʌ����n���{�e����
	else if (type == MapBlockData::MapBlockType::ForwardPlane) {
		const XMFLOAT3 rot = { -90, 0, 0 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::ForwardPlane));
		newBlock->SetRotation(rot);

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
	//�������n���{�e����
	else if (type == MapBlockData::MapBlockType::AwayPlane) {
		const XMFLOAT3 rot = { 90, 0, 0 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::AwayPlane));
		newBlock->SetRotation(rot);

		blocks.insert(blocks.begin() + count, std::move(newBlock));
		blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
	}
}

void MapDataStage::BlockAdd(const MapBlockData::MapBlockType type, const XMINT3 chipNum)
{
	std::unique_ptr<StageBlock> newBlock;

	//�S�[������
	if (type == MapBlockData::MapBlockType::Goal) {
		newBlock.reset(StageBlock::Create(2, chipNum, shiftPos, MapBlockData::MapBlockType::Goal));
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
	//�u���b�N����
	else if (type == MapBlockData::MapBlockType::Block) {
		newBlock.reset(StageBlock::Create(0, chipNum, shiftPos, MapBlockData::MapBlockType::Block));
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
	//������n���{�e����
	else if (type == MapBlockData::MapBlockType::UpPlane) {
		const XMFLOAT3 rot = {};	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::UpPlane));
		newBlock->SetRotation(rot);
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
	//�������n���{�e����
	else if (type == MapBlockData::MapBlockType::DownPlane) {
		const XMFLOAT3 rot = { 180, 0, 0 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::DownPlane));
		newBlock->SetRotation(rot);
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
	//�������n���{�e����
	else if (type == MapBlockData::MapBlockType::LeftPlane) {
		const XMFLOAT3 rot = { 0, 0, 90 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::LeftPlane));
		newBlock->SetRotation(rot);
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
	//�E�����n���{�e����
	else if (type == MapBlockData::MapBlockType::RightPlane) {
		const XMFLOAT3 rot = { 0, 0, -90 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::RightPlane));
		newBlock->SetRotation(rot);
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
	//���ʌ����n���{�e����
	else if (type == MapBlockData::MapBlockType::ForwardPlane) {
		const XMFLOAT3 rot = { -90, 0, 0 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::ForwardPlane));
		newBlock->SetRotation(rot);
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
	//�������n���{�e����
	else if (type == MapBlockData::MapBlockType::AwayPlane) {
		const XMFLOAT3 rot = { 90, 0, 0 };	//�X����p�x
		newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::AwayPlane));
		newBlock->SetRotation(rot);
		newBlock->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
		blocks.push_back(std::move(newBlock));
	}
}

void MapDataStage::BlockCountCreate()
{
	// size�������ɂȂ����瑁�����^�[��
	if (blocks.size() <= rndcount.size())
	{
		return;
	}
	
	while (true)
	{
		if (BlockCount())
		{
			break;
		}
	}
}

bool MapDataStage::BlockCount()
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
		return true;
	}

	return false;
}
