#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ParticleEmitter.h"
#include "PlayerActionManager.h"
#include "MapBlockData.h"
#include "JsonLoader.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "StageManager.h"
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
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));

	//�}�b�v����
	mapData.reset(MapDataStage::Create(StageManager::GetSelectStage()));

	//�J����������
	camera.reset(new GameCamera());
	const float distanceStageCenter = 60.0f; //�J�������_�̃X�e�[�W��������̋���
	const XMINT3 mapSize = mapData->GetMapSize(); //�}�b�v�̑傫��
	const Vector3 stageCenterPos = { Block::GetBlockSize() / 2 * (mapSize.x - 1), Block::GetBlockSize() / 2 * (mapSize.y - 1), Block::GetBlockSize() / 2 * (mapSize.z - 1) }; //�X�e�[�W�������W
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });

	//�v���C���[����
	player.reset(Player::Create(modelPlayer.get(), mapData->GetPlayerCreateMapChipNum(), camera.get()));

	//�v���C���[�̈ړ��\����p�Ƀ}�b�v�ԍ����Z�b�g
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());

	//�V������
	skydome.reset(ObjObject3d::Create(modelSkydome.get()));
	skydome->SetPosition({});
	skydome->SetScale({ 2, 2, 2 });

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	ObjObject3d::SetCamera(camera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());

	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	ObjObject3d::SetLightGroup(lightGroup.get());

	//obj�I�u�W�F�N�g�ɃJ�������Z�b�g
	InstanceObject::SetCamera(camera.get());
	InstanceObject::SetLightCamera(lightCamera.get());

	//obj�I�u�W�F�N�g�Ƀ��C�g���Z�b�g
	InstanceObject::SetLightGroup(lightGroup.get());

	//�p�[�e�B�N���ɃJ�������Z�b�g
	ParticleManager::SetCamera(camera.get());
	//��ʂɃp�[�e�B�N�����c�邱�Ƃ�����̂őS�č폜���Ă���
	ParticleEmitter::GetInstance()->AllDelete();

	//������Ԃ�binary�ۑ�
	Vector3 cameraPos = camera->GetEye();
	Vector3 playerPos = player->GetPosition();
	XMINT3 mapChip = player->GetMapChipNumberPos();

	JsonLoader::SerializeBinary(std::to_string(orderNum), camera->GetIs2D(), player->GetMoveSurfacePhase(),
		{ mapChip.x,mapChip.y,mapChip.z }, { cameraPos.x,cameraPos.y,cameraPos.z },
		{ camera->GetCameraXPosPhase(),camera->GetCameraYPosPhase() }, { playerPos.x, playerPos.y, playerPos.z });

	userInterface_ = UserInterface::Create();
}

void GameScene::Finalize()
{
	DeleteBinary();
}

void GameScene::Update()
{
	//�f�o�b�O�p�e�L�X�g
	DebugText::GetInstance()->Print("CameraMove : arrow", 10, 10);
	DebugText::GetInstance()->Print("PlayerMove : WASD", 10, 30);
	DebugText::GetInstance()->Print("StageReset : R", 10, 50);
	DebugText::GetInstance()->Print("StageSelect: P", 10, 70);

	//�v���C���[���S�[����������X�e�[�W�N���A
	if (!isStageClear) {
		if (player->GetIsGoal()) {
			isStageClear = true;
			StageManager::StageClear();
		}
	}
	else if (!isSceneChange) {
		DebugText::GetInstance()->Print("STAGECLEAR", 100, 300, 5);
		DebugText::GetInstance()->Print("NEXTSTAGE : PRESS ENTER", 500, 600);

		if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
			//�V�[���؂�ւ�
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
			//binary�폜
			DeleteBinary();
			//���̃X�e�[�W��
			StageManager::NextStageSelect();
		}
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
	mapData->Update();
	//�V��
	skydome->Update();
	//UI�̍X�V
	userInterface_->Update();

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

	if (Input::GetInstance()->TriggerKey(DIK_M))
	{
		if (!userInterface_->GetMenuFlag())
		{
			userInterface_->SetMenuFlag(true);
		}
		else
		{
			userInterface_->SetMenuFlag(false);
		}
	}
	

	//binary�o��
	if (player->GetIsMove() || camera->GetIsTriggerDimensionChange()) {
		orderNum++;
		orderMaxNum = orderNum;
		if (deleteOrderMaxNum < orderMaxNum) {
			deleteOrderMaxNum = orderMaxNum;
		}
		Vector3 cameraPos = camera->GetEye();
		Vector3 playerPos = player->GetPosition();
		XMINT3 mapChip = player->GetMapChipNumberPos();
		JsonLoader::SerializeBinary(std::to_string(orderNum), camera->GetIs2D(), player->GetMoveSurfacePhase(),
			{ mapChip.x,mapChip.y,mapChip.z }, { cameraPos.x,cameraPos.y,cameraPos.z },
			{ camera->GetCameraXPosPhase(),camera->GetCameraYPosPhase() }, { playerPos.x,playerPos.y,playerPos.z });
	}

	//undo
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Z)) {
		if (orderNum != 0) {
			orderNum--;
			bool is2D = false;
			int moveSurface = 0;
			std::array<int, 3> mapChip{};
			std::array<float, 3> cameraPos{}, playerPos{};
			std::array<int, 2> cameraPosPhase{};
			JsonLoader::DeserializeBinary(std::to_string(orderNum), &is2D, &moveSurface, &mapChip, &cameraPos, &cameraPosPhase, &playerPos);

			if (camera->GetIs2D() != is2D) {
				camera->SetIs2D(is2D);
				camera->SetDirtyProjection(true);
			}
			player->SetMoveSurfacePhase(moveSurface);
			player->SetMapChipNumberPos({ mapChip[0],mapChip[1],mapChip[2] });
			camera->SetEye({ cameraPos[0],cameraPos[1],cameraPos[2] });
			camera->SetCameraXPosPhase(cameraPosPhase[0]);
			camera->SetCameraYPosPhase(cameraPosPhase[1]);
			player->SetPosition({ playerPos[0],playerPos[1],playerPos[2] });
		}
	}
	//redo
	else if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Y)) {
		if (orderNum != orderMaxNum) {
			orderNum++;
			bool is2D = false;
			int moveSurface = 0;
			std::array<int, 3> mapChip{};
			std::array<float, 3> cameraPos{}, playerPos{};
			std::array<int, 2> cameraPosPhase{};
			JsonLoader::DeserializeBinary(std::to_string(orderNum), &is2D, &moveSurface, &mapChip, &cameraPos, &cameraPosPhase, &playerPos);

			if (camera->GetIs2D() != is2D) {
				camera->SetIs2D(is2D);
				camera->SetDirtyProjection(true);
			}
			player->SetMoveSurfacePhase(moveSurface);
			player->SetMapChipNumberPos({ mapChip[0],mapChip[1],mapChip[2] });
			camera->SetEye({ cameraPos[0],cameraPos[1],cameraPos[2] });
			camera->SetCameraXPosPhase(cameraPosPhase[0]);
			camera->SetCameraYPosPhase(cameraPosPhase[1]);
			player->SetPosition({ playerPos[0],playerPos[1],playerPos[2] });
		}
	}

	DebugText::GetInstance()->Print("move surface" + std::to_string(player->GetMoveSurfacePhase()), 10, 90);


	if (Input::GetInstance()->TriggerKey(DIK_R)) {
		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
		//binary�폜
		DeleteBinary();
	}
	if (Input::GetInstance()->TriggerKey(DIK_P)) {
		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "STAGESELECT");
		//binary�폜
		DeleteBinary();
	}
	//�V�[���ύX���
	SceneChangeMode();
	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
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

	//�V��
	skydome->Draw();

	///-------Object3d�`�悱���܂�-------///

	///-------Instance�`�悱������-------///

	InstanceObject::DrawPrev();
	//�}�b�v�p�u���b�N
	mapData->Draw();

	///-------Instance�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void GameScene::Draw3DLightView()
{
}

void GameScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	userInterface_->Draw();

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}
