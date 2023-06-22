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
#include "SpriteTextureLoader.h"
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
	modelPlayerEffect.reset(ObjModel::LoadFromOBJ("effect"));
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));

	//�}�b�v����
	mapData.reset(MapDataStage::Create(StageManager::GetSelectStage()));

	//�w�i�I�u�W�F�N�g����
	backGround.reset(BackGround::Create());

	//�J����������
	camera.reset(new GameCamera());
	const XMFLOAT3 distanceStageCenter = mapData->GetCameraDist(); //�J�������_�̃X�e�[�W��������̋���
	const XMINT3 mapSize = mapData->GetMapSize(); //�}�b�v�̑傫��
	const Vector3 stageCenterPos = {}; //�X�e�[�W�������W
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });

	//�v���C���[����
	player.reset(Player::Create(modelPlayer.get(), mapData->GetPlayerCreateMapChipNum(), mapData->GetShiftPos(), camera.get(), modelPlayerEffect.get()));
	player->SetMoveSurfacePhase(mapData->GetInstallationSurface());
	//�v���C���[�̈ړ��\����p�Ƀ}�b�v�ԍ����Z�b�g
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());
	//�J�����ɐ��������v���C���[���Z�b�g
	camera->SetPlayer(player.get());

	//�V������
	skydome.reset(Skydome::Create(modelSkydome.get()));

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
	KeepBinary(*camera, *player);

	userInterface_ = UserInterface::Create();
	//������@UI����
	howToPlayUI.reset(HowToPlayUI::Create(true));
	//�X�e�[�W�N���AUI����
	stageClearUI.reset(StageClearUI::Create());
	stageClear_ = ClearStaging::Create();

	// �X�J�C�h�[������
	//paranomaSkyDorm.reset(dynamic_cast<ParanomaSkyDorm*>(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::ParanomaSky))));
}

void GameScene::Finalize()
{
	DeleteBinary();
}

void GameScene::Update()
{
	if (!isStageClear) {
		//undo
		if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Z)) {
			Undo(camera.get(), player.get());
		}
		//redo
		else if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Y)) {
			Redo(camera.get(), player.get());
		}

		//R�L�[�Ń��Z�b�g
		if (Input::GetInstance()->TriggerKey(DIK_R)) {
			//�V�[���؂�ւ�
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
			//binary�폜
			DeleteBinary();
		}
		//�G�X�P�[�v�L�[�Ń��j���[���(���͑��X�e�[�W�Z���N�g��)
		else if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			//se�Đ�
			Audio::GetInstance()->PlayWave(Audio::SoundName::button);
			//�V�[���؂�ւ�
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "STAGESELECT");
			//binary�폜
			DeleteBinary();
		}

		//binary�o��
		if (player->GetIsMove() || (!player->GetNowMove() && camera->GetIsTriggerDimensionChange())) {
			orderNum++;
			orderMaxNum = orderNum;
			if (deleteOrderMaxNum < orderMaxNum) {
				deleteOrderMaxNum = orderMaxNum;
			}

			KeepBinary(*camera, *player);
		}

		//�v���C���[���S�[����������X�e�[�W�N���A
		if (player->GetIsGoal()) {
			//�N���A��
			Audio::GetInstance()->PlayWave(Audio::SoundName::clear);

			isStageClear = true;
			StageManager::StageClear();
			camera->SetClearMode();
			stageClear_->SetMovePhase(ClearStaging::MovePhase::Start);
		}
	}
	else {
		if (stageClear_->GetEndFlag())
		{
			//binary�폜
			DeleteBinary();
			//���̃X�e�[�W��
			StageManager::NextStageSelect();
			//�Đ���
			ReCreate();
			stageClear_->Reset();

		}
		//�}�b�v���Đ������I������A���̃X�e�[�W���J�n���邽�߂Ƀt���O�Ȃǂ����Z�b�g
		if (mapData->GetIsReCreateEnd()) {
			RestartGame();
		}
	}

	//�J�����X�V
	camera->Update();
	lightCamera->Update();

	//���C�g�X�V
	lightGroup->Update();

	// �X�J�C�h�[���X�V
	//paranomaSkyDorm->Update();

	//�I�u�W�F�N�g�X�V
	//�v���C���[
	player->Update();
	//�}�b�v�p�u���b�N
	mapData->Update();
	//�V��
	skydome->Update();
	//�w�i�I�u�W�F�N�g
	backGround->Update();

	//�X�v���C�g
	//UI�̍X�V
	userInterface_->Update();
	//������@
	howToPlayUI->Update();
	//�X�e�[�W�N���AUI�X�V
	stageClearUI->Update();
	stageClear_->Update();

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

	//������Ԃ�binary�ۑ�
	if (player->GetIsStartMove()) {
		KeepBinary(*camera, *player);
	}

	//�V�[���ύX���
	SceneChangeMode();
	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
}

void GameScene::DrawBackSprite()
{
	/*SpriteCommon::GetInstance()->DrawPrev("SkyDorm");
	paranomaSkyDorm->Draw();*/
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
	//�w�i�I�u�W�F�N�g
	backGround->Draw();

	///-------Instance�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void GameScene::Draw3DLightView()
{
	///-------Instance�`�悱������-------///

	InstanceObject::DrawLightViewPrev();

	//�w�i�I�u�W�F�N�g
	backGround->DrawLightCameraView();

	///-------Instance�`�悱���܂�-------///
}

void GameScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//������@
	howToPlayUI->Draw();

	userInterface_->Draw();

	//�X�e�[�W�N���AUI
	/*if (isStageClear) {
		stageClearUI->Draw();
	}*/

	stageClear_->Draw();

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void GameScene::ReCreate()
{
	mapData->ReCreateMapBlock(StageManager::GetSelectStage());
	player->ReCreate(mapData->GetPlayerCreateMapChipNum(), mapData->GetShiftPos());
	player->SetMoveSurfacePhase(mapData->GetInstallationSurface());
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());
	camera->SetReCreateMove();
	camera->SetSaveDistanceStageCenter(mapData->GetCameraDist());
	skydome->SetIsRotate(true);
}

void GameScene::RestartGame()
{
	//���̃X�e�[�W���J�n���邽�߂Ƀt���O�Ȃǂ����Z�b�g
	isStageClear = false;
	mapData->SetIsReCreateEnd(false);
	player->Reset();
	camera->Reset();
	skydome->SetIsRotate(false);
}
