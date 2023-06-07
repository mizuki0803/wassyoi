#include "TitleScene.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "PlayerActionManager.h"
#include "ParticleEmitter.h"
#include "MapBlockData.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "StageManager.h"
#include "JsonLoader.h"

void TitleScene::Initialize()
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

	//�I�𒆂̃X�e�[�W�ԍ���0�ɃZ�b�g
	StageManager::SetSelectStage(0);
	//�}�b�v����
	mapData.reset(MapDataStage::Create(StageManager::GetSelectStage()));

	//�J����������
	camera.reset(new GameCamera());
	const XMFLOAT3 distanceStageCenter = mapData->GetCameraDist(); //�J�������_�̃X�e�[�W��������̋���
	const XMINT3 mapSize = mapData->GetMapSize(); //�}�b�v�̑傫��
	const Vector3 stageCenterPos = { Block::GetBlockSize() / 2 * (mapSize.x - 1), Block::GetBlockSize() / 2 * (mapSize.y - 1), Block::GetBlockSize() / 2 * (mapSize.z - 1) }; //�X�e�[�W�������W
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });

	//�v���C���[����
	player.reset(Player::Create(modelPlayer.get(), mapData->GetPlayerCreateMapChipNum(), camera.get(), modelPlayerEffect.get()));
	player->SetMoveSurfacePhase(mapData->GetInstallationSurface());

	//�v���C���[�̈ړ��\����p�Ƀ}�b�v�ԍ����Z�b�g
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());

	//�V������
	skydome.reset(ObjObject3d::Create(modelSkydome.get()));
	skydome->SetPosition({});
	skydome->SetScale({ 2, 2, 2 });

	//�w�i�I�u�W�F�N�g����
	backGround.reset(BackGround::Create());

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

	//������@UI����
	howToPlayUI.reset(HowToPlayUI::Create(false));

	////������Ԃ�binary�ۑ�
	KeepBinary(*camera, *player);
}

void TitleScene::Finalize()
{
	DeleteBinary();
}

void TitleScene::Update()
{
	//�f�o�b�O�p�e�L�X�g
	//DebugText::GetInstance()->Print("TITLE SCENE", 350, 200, 5);
	DebugText::GetInstance()->Print("PRESS ENTER", 600, 600);

	//�v���C���[���S�[����������X�e�[�W�N���A
	if (!isStageClear) {
		if (player->GetIsGoal()) {
			isStageClear = true;
			StageManager::StageClear();
		}
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
	mapData->Update();
	//�V��
	skydome->Update();
	//�w�i�I�u�W�F�N�g
	backGround->Update();

	//�X�v���C�g�X�V
	//������@
	howToPlayUI->Update();

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

	//binary�o��
	if (player->GetIsMove() || camera->GetIsTriggerDimensionChange()) {
		orderNum++;
		orderMaxNum = orderNum;
		if (deleteOrderMaxNum < orderMaxNum) {
			deleteOrderMaxNum = orderMaxNum;
		}

		KeepBinary(*camera, *player);
	}

	//undo
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Z)) {
		Undo(camera.get(), player.get());

	}
	//redo
	else if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Y)) {
		Redo(camera.get(), player.get());
	}

	if (Input::GetInstance()->TriggerKey(DIK_RETURN) && !isSceneChange) {
		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
		//binary�폜
		DeleteBinary();
		//���̃X�e�[�W��
		StageManager::NextStageSelect();
	}
	//�V�[���ύX���
	SceneChangeMode();
	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
}

void TitleScene::DrawBackSprite()
{
}

void TitleScene::Draw3D()
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

void TitleScene::Draw3DLightView()
{
}

void TitleScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//������@
	howToPlayUI->Draw();

	//�V�[���ύX���o
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}
