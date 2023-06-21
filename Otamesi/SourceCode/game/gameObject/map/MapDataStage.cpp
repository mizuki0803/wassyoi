#include "MapDataStage.h"
#include "MapBlockData.h"
#include <sstream>

MapDataStage* MapDataStage::Create(const int selectStageNum)
{
	//インスタンスを生成
	MapDataStage* instance = new MapDataStage();

	//マップ生成
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
	//描画
	//マップ用ブロック
	Block::Draw();
}

void MapDataStage::PlayGame()
{
	//マップ用ブロック
	for (const std::unique_ptr<StageBlock>& block : blocks) {
		block->Update();
	}
}

void MapDataStage::GameStart()
{
	// 何個ずつ上げるかの計算(全体の30%)
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

	// 今検索しているブロックの数
	int blockCount = 0;
	//マップ用ブロック
	for (const std::unique_ptr<StageBlock>& block : blocks) {
		// 検索が当たったか
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

	// 検索が当たったか
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
		phase_ = static_cast<int>(GamePhase::GamePlay);
	}
}

void MapDataStage::GameReStart()
{
	// 何個ずつ上げるかの計算(全体の30%)
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

	// 今検索しているブロックの数
	int blockCount = 0;
	//マップ用ブロック
	for (const std::unique_ptr<StageBlock>& block : blocks) {
		// 検索が当たったか
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

	// 検索が当たったか
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
		phase_ = static_cast<int>(GamePhase::GamePlay);
		isReCreateEnd = true;
	}
}

void MapDataStage::CreateMapBlock()
{
	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//マップ番号
				const XMINT3 chipNum = { i, j, k };
				//プレイヤーの生成用マップ番号を保存
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Player) {
					playerCreateMapChipNum = chipNum;
				}
				//ゴール生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Goal) {
					std::unique_ptr<StageBlock> newBlock;
					newBlock.reset(StageBlock::Create(2, chipNum, shiftPos, MapBlockData::MapBlockType::Goal));
					blocks.push_back(std::move(newBlock));
				}
				//ブロック生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Block) {
					std::unique_ptr<StageBlock> newBlock;
					newBlock.reset(StageBlock::Create(0, chipNum, shiftPos, MapBlockData::MapBlockType::Block));
					blocks.push_back(std::move(newBlock));
				}
				//上向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::UpPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = {};	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::UpPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//下向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::DownPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 180, 0, 0 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::DownPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//左向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::LeftPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, 90 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::LeftPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//右向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::RightPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, -90 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::RightPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//正面向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::ForwardPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { -90, 0, 0 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::ForwardPlane));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//奥向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::AwayPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 90, 0, 0 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::AwayPlane));
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
	//jsonマップデータ読み込み
	std::ostringstream  fileName;
	fileName << selectStageNum;
	LoadMapData(fileName.str());

	int count = 0;
	int oldSize = static_cast<int>(blocks.size());

	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//何もない時はリターン
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::None)
				{
					continue;
				}

				//マップ番号
				const XMINT3 chipNum = { i, j, k };
				
				//プレイヤーの生成用マップ番号を保存
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Player) {
					playerCreateMapChipNum = chipNum;
					continue;
				}

				if (mapChipNum[i][j][k] == static_cast<int>(blocks[count]->GetMapBlockType()))
				{
					blocks[count]->ReCreate(StageBlock::GamePhase::Move, chipNum, shiftPos);
				}
				//ゴール生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Goal) {
					std::unique_ptr<StageBlock> newBlock;
					newBlock.reset(StageBlock::Create(2, chipNum, shiftPos, MapBlockData::MapBlockType::Goal));

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
				}
				//ブロック生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Block) {
					std::unique_ptr<StageBlock> newBlock;
					newBlock.reset(StageBlock::Create(0, chipNum, shiftPos, MapBlockData::MapBlockType::Block));

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
				}
				//上向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::UpPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = {};	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::UpPlane));
					newBlock->SetRotation(rot);

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
				}
				//下向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::DownPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 180, 0, 0 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::DownPlane));
					newBlock->SetRotation(rot);

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
				}
				//左向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::LeftPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, 90 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::LeftPlane));
					newBlock->SetRotation(rot);

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
				}
				//右向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::RightPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, -90 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::RightPlane));
					newBlock->SetRotation(rot);

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
				}
				//正面向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::ForwardPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { -90, 0, 0 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::ForwardPlane));
					newBlock->SetRotation(rot);

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
				}
				//奥向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::AwayPlane) {
					std::unique_ptr<StageBlock> newBlock;
					const XMFLOAT3 rot = { 90, 0, 0 };	//傾ける角度
					newBlock.reset(StageBlock::Create(1, chipNum, shiftPos, MapBlockData::MapBlockType::AwayPlane));
					newBlock->SetRotation(rot);

					blocks.insert(blocks.begin() + count, std::move(newBlock));
					blocks[count]->ReCreate(StageBlock::GamePhase::ReStart, chipNum, shiftPos);
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

void MapDataStage::BlockCountCreate()
{
	// sizeが同じになったら早期リターン
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
	// 検索に当たったか
	bool hitFlag = false;
	// ランダムに数値を決める
	int temp = rand() % blocks.size();
	for (auto& count : rndcount)
	{
		// 中に同じ数があるか
		if (count == temp)
		{
			hitFlag = true;
			break;
		}
	}

	// 無かったら
	if (!hitFlag)
	{
		rndcount.push_back(temp);
		return true;
	}

	return false;
}
