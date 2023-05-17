#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Collision.h"
#include "Easing.h"
#include "PlayerActionManager.h"
#include "MapBlockData.h"
#include "JsonLoader.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void GameScene::Initialize()
{
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	//obj���烂�f���f�[�^��ǂݍ���
	modelPlayer.reset(ObjModel::LoadFromOBJ("player"));
	modelBlock.reset(ObjModel::LoadFromOBJ("block"));
	modelGoal.reset(ObjModel::LoadFromOBJ("goal"));
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));


	//json�}�b�v�f�[�^�ǂݍ���
	LoadMapData("star");

	//�J����������
	camera.reset(new GameCamera());
	const float distanceStageCenter = 60.0f; //�J�������_�̃X�e�[�W��������̋���
	const Vector3 stageCenterPos = { Block::GetBlockSize() / 2 * (mapSize.x - 1), Block::GetBlockSize() / 2 * (mapSize.y - 1), Block::GetBlockSize() / 2 * (mapSize.z - 1) }; //�X�e�[�W�������W
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });

	//�}�b�v�p�u���b�N����
	for (int i = 0; i < mapChipNum.size(); i++) {
		for (int j = 0; j < mapChipNum[i].size(); j++) {
			for (int k = 0; k < mapChipNum[i][j].size(); k++) {
				//�v���C���[����
				if (mapChipNum[i][j][k] == MapBlockData::BlockType::Player) {
					player.reset(Player::Create(modelPlayer.get(), { i, j, k }, camera.get()));
				}
				//�S�[������
				else if (mapChipNum[i][j][k] == MapBlockData::BlockType::Goal) {
					std::unique_ptr<Block> newBlock;
					const Vector3 pos = { i * Block::GetBlockSize(), j * Block::GetBlockSize(), k * Block::GetBlockSize() };
					newBlock.reset(Block::Create(modelGoal.get(), pos));
					blocks.push_back(std::move(newBlock));
				}
				//�u���b�N����
				else if (mapChipNum[i][j][k] == MapBlockData::BlockType::Block) {
					std::unique_ptr<Block> newBlock;
					const Vector3 pos = { i * Block::GetBlockSize(), j * Block::GetBlockSize(), k * Block::GetBlockSize() };
					newBlock.reset(Block::Create(modelBlock.get(), pos));
					blocks.push_back(std::move(newBlock));
				}
			}
		}
	}
	//�v���C���[�̈ړ��\����p�Ƀ}�b�v�ԍ����Z�b�g
	PlayerActionManager::SetMapChipNum(mapChipNum);

	//�V������
	skydome.reset(ObjObject3d::Create(modelSkydome.get()));
	skydome->SetPosition({});
	skydome->SetScale({ 2, 2, 2 });

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(camera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());

	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());
}

void GameScene::Update()
{
	//�f�o�b�O�p�e�L�X�g
	DebugText::GetInstance()->Print("CameraMove : arrow", 10, 10);
	DebugText::GetInstance()->Print("PlayerMove : WASD", 10, 30);

	//�v���C���[���S�[����������X�e�[�W�N���A
	if (!isStageClear) {
		if (player->GetIsGoal()) { isStageClear = true; }
	}
	else {
		DebugText::GetInstance()->Print("STAGECLEAR", 100, 300, 5);
	}

	//�J�����X�V
	camera->Update();
	lightCamera->Update();

	//���C�g�X�V
	lightGroup->Update();

	//�I�u�W�F�N�g�X�V
	//�v���C���[
	player->Update();

	BlockUpdate();

	//�V��
	skydome->Update();

	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		//�V�[���؂�ւ�
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void GameScene::DrawBackSprite()
{
}

void GameScene::Draw3D()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///

	//�v���C���[
	player->Draw();
	//�}�b�v�p�u���b�N
	for (const std::unique_ptr<Block>& block : blocks) {
		block->Draw();
	}
	//�V��
	skydome->Draw();

	///-------Object3d�`�悱���܂�-------///
}

void GameScene::Draw3DLightView()
{
}

void GameScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///


	///-------�X�v���C�g�`�悱���܂�-------///
}

bool GameScene::LoadMapData(const std::string& fileName)
{
	//�}�b�v�f�[�^���ǂݎ��
	std::vector<std::vector<std::vector<int>>> inputmap;
	float _cameraDist;	//����͍��֌W�Ȃ�

	if (!JsonLoader::DeserializeTest("Resources/mapdata/" + fileName + ".json", &_cameraDist, &inputmap)) {
		return false;
	}

	//�}�b�v�̃T�C�Y���擾
	const int marginNum = 2; //�Q�[���̎d�l��A�㉺���E�O���1�}�X�̗]�������
	mapSize = { int(inputmap[0][0].size()) + marginNum, int(inputmap.size()) + marginNum, int(inputmap[0].size()) + marginNum };

	//�]������̑傫���Ń}�b�v�f�[�^�𐶐��B�܂���0�Ŗ��߂�
	mapChipNum.resize(mapSize.x);
	for (int x = 0; x < mapSize.x; x++) {
		mapChipNum[x].resize(mapSize.y);
		for (int y = 0; y < mapSize.y; y++) {
			mapChipNum[x][y].resize(mapSize.z);
		}
	}

	//�]���ł͂Ȃ������̗v�f�ԍ��ɓǂݍ��񂾃}�b�v�f�[�^���̔ԍ������蓖�ĂĂ���
	for (int x = 0; x < mapSize.x - marginNum; x++) {
		for (int y = 0; y < mapSize.y - marginNum; y++) {
			for (int z = 0; z < mapSize.z - marginNum; z++) {
				mapChipNum[x + 1][y + 1][z + 1] = inputmap[y][z][x];
			}
		}
	}

	return true;
}

void GameScene::BlockUpdate()
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
		for (const std::unique_ptr<Block>& block : blocks) {
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
		for (const std::unique_ptr<Block>& block : blocks) {
			block->Update();
		}
	}
}

void GameScene::BlockCountCreate()
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
