#include "MapDataStageSelect.h"
#include "MapBlockData.h"
#include "StageManager.h"

MapDataStageSelect* MapDataStageSelect::Create(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase)
{
	//インスタンスを生成
	MapDataStageSelect* instance = new MapDataStageSelect();

	//マップ生成
	if (!instance->Initialize(selectStageNum, positionPhase)) {
		delete instance;
		assert(0);
		return nullptr;
	};


	return instance;
}

bool MapDataStageSelect::Initialize(const int selectStageNum, const StageSelectBlockManager::BlockManagerPositionPhase& positionPhase)
{
	//ブロック管理生成
	blockManager.reset(StageSelectBlockManager::Create(positionPhase));

	//マップ生成
	CreateMap(selectStageNum);

	//選択しているステージをクリアしていなければブロックを真っ暗にする
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
	//ブロック管理
	blockManager->Update();

	//ブロック
	for (const std::unique_ptr<StageSelectBlock>& block : blocks) {
		block->Update();
	}
}

void MapDataStageSelect::Draw()
{
	//ブロック描画
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
	//ブロック生成
	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//マップ番号
				const XMINT3 chipNum = { i, j, k };
				//プレイヤーの生成用マップ番号を保存
				if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Player) {
					std::unique_ptr<StageSelectBlock> newBlock;
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::player), chipNum, shiftPos, blockManager.get()));
					blocks.push_back(std::move(newBlock));
				}
				//ゴール生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Goal) {
					std::unique_ptr<StageSelectBlock> newBlock;
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::goal), chipNum, shiftPos, blockManager.get()));
					blocks.push_back(std::move(newBlock));
				}
				//ブロック生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::Block) {
					std::unique_ptr<StageSelectBlock> newBlock;
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::block), chipNum, shiftPos, blockManager.get()));
					blocks.push_back(std::move(newBlock));
				}
				//上向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::UpPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = {};	//傾ける角度
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//下向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::DownPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 180, 0, 0 };	//傾ける角度
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//左向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::LeftPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, 90 };	//傾ける角度
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//右向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::RightPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 0, 0, -90 };	//傾ける角度
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//正面向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::ForwardPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { -90, 0, 0 };	//傾ける角度
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
				//奥向きハリボテ生成
				else if (mapChipNum[i][j][k] == MapBlockData::MapBlockType::AwayPlane) {
					std::unique_ptr<StageSelectBlock> newBlock;
					const XMFLOAT3 rot = { 90, 0, 0 };	//傾ける角度
					newBlock.reset(StageSelectBlock::Create(int(Block::BROCK_TYPE::plane), chipNum, shiftPos, blockManager.get()));
					newBlock->SetRotation(rot);
					blocks.push_back(std::move(newBlock));
				}
			}
		}
	}
}
