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

	//���}�b�v�ԍ�
	/*const int mapSize = 7;
	int mapData[mapSize][mapSize][mapSize] = {

		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,1,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,3,3,3,0,0},
			{0,0,3,0,3,0,0},
			{0,0,3,3,3,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,3,3,2,3,3,0},
			{0,0,3,3,3,2,0},
			{0,0,0,3,0,0,0},
			{0,0,3,3,3,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,3,3,3,0,0},
			{0,0,3,0,3,0,0},
			{0,0,3,3,3,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,3,3,3,3,3,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
	};*/

	const int mapSizeYZ = 7;
	const int mapSizeX = 8;
	int mapData[mapSizeX][mapSizeYZ][mapSizeYZ] = {
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,1,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,3,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,3,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,2,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,3,0,0,0,0,0},
			{0,3,0,0,3,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,3,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,3,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
		{
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0},
		},
	};

	/*const int mapSize = 3;
	int mapData[mapSize][mapSize][mapSize] = {
		{
			{1,2,3},
			{3,3,3},
			{3,3,3},
		},
		{
			{3,3,3},
			{3,3,3},
			{3,3,3},
		},
		{
			{3,3,3},
			{3,3,3},
			{3,3,3},
		}
	};*/

	//�}�b�v����
	mapChipNum.resize(mapSizeX);
	for (int i = 0; i < mapSizeX; i++) {
		mapChipNum[i].resize(mapSizeYZ);
		for (int j = 0; j < mapSizeYZ; j++) {
			for (int k = 0; k < mapSizeYZ; k++) {
				mapChipNum[i][j].push_back(mapData[i][j][k]);
			}
		}
	}

	//�J����������
	camera.reset(new GameCamera());
	const float distanceStageCenter = 60.0f; //�J�������_�̃X�e�[�W��������̋���
	const Vector3 stageCenterPos = { Block::GetBlockSize() / 2 * (mapSizeX - 1), -Block::GetBlockSize() / 2 * (mapSizeYZ - 1), Block::GetBlockSize() / 2 * (mapSizeYZ - 1) }; //�X�e�[�W�������W
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
					const Vector3 pos = { k * Block::GetBlockSize(), -j * Block::GetBlockSize(), i * Block::GetBlockSize() };
					newBlock.reset(Block::Create(modelGoal.get(), pos));
					blocks.push_back(std::move(newBlock));
				}
				//�u���b�N����
				else if (mapChipNum[i][j][k] == MapBlockData::BlockType::Block) {
					std::unique_ptr<Block> newBlock;
					const Vector3 pos = { k * Block::GetBlockSize(), -j * Block::GetBlockSize(), i * Block::GetBlockSize() };
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
	//�}�b�v�p�u���b�N
	for (const std::unique_ptr<Block>& block : blocks) {
		block->Update();
	}
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