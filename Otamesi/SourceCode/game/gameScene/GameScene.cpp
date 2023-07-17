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
#include "OutLine.h"


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

	OutLine::SetCmaera(camera.get());

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

	//UI�֌W����
	userInterface_ = UserInterface::Create(UserInterface::GamePhase::Game);
	//�X�e�[�W�N���AUI����
	stageClear_ = ClearStaging::Create();


	// UIレンダー初期化
	imageUiRenderer->Common(dxbase->GetDevice(), dxbase->GetCmdList());
	imageUiRenderer.reset(ImageUIRenderer::Create());
}

void GameScene::Finalize()
{
	DeleteBinary();
}

void GameScene::Update()
{
	if (!isStageClear) {
		//�S�ẴX�e�[�W���N���A��̓��ʂȃX�e�[�W�̂݃X�y�[�X�L�[���͂Ń^�C�g���V�[����
		if (StageManager::GetSelectStage() == 100) {
			if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
				//�V�[���؂�ւ�
				SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "TITLE");
			}
		}
		//���̑��X�e�[�W�͒ʏ�̓���
		else {
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
			//�G�X�P�[�v�L�[�Ń��j���[���
			else if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
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
	}
	else {
		if (stageClear_->GetIntermediateTrigger())
		{
			//binary�폜
			DeleteBinary();
			//���߂đS�ẴX�e�[�W���N���A�����ꍇ�͓��ʂȃX�e�[�W��
			if (StageManager::AllStageClearTriggerCheck()) {
				//�J�����̉�]�����ɖ߂���Ԃɂ���
				camera->SetClearResetAround();
			}
			//���̃X�e�[�W������ꍇ�͎��̃X�e�[�W��
			else if (StageManager::NextStageSelect()) {
				//�J�����̉�]�����ɖ߂���Ԃɂ���
				camera->SetClearResetAround();
			}
			//���̃X�e�[�W���Ȃ��ꍇ�̓X�e�[�W�Z���N�g�V�[��
			else {
				//�V�[���؂�ւ�
				SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "STAGESELECT");
			}
		}
		else if (stageClear_->GetEndFlag())
		{
			//�Đ���
			ReCreate();
			stageClear_->Reset();
		}

		//�}�b�v���Đ������I������A���̃X�e�[�W���J�n���邽�߂Ƀt���O�Ȃǂ����Z�b�g
		if (mapData->GetIsReCreateEnd()) {
			RestartGame();
		}
	}

	camera->SetNotMove(userInterface_->GetMenuFlag(), mapData->GetIsMoveEnd());
	player->SetNotMove(userInterface_->GetMenuFlag(), mapData->GetIsMoveEnd());
	userInterface_->SetNotMove(isStageClear);
	MenuAction();

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

	//�X�v���C�g
	//UI�̍X�V
	userInterface_->Update();
	//�X�e�[�W�N���AUI�X�V
	stageClear_->Update();

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

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
}

void GameScene::Draw3D()
{

	//Object3d���ʃR�}���h
	ObjObject3d::DrawOutLinePrev();
	//ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///
	// �A�E�g���C���p�萔�o�b�t�@�Z�b�g
	GamePostEffect::SetIdColorBuffer(5,PostEffect::kNone);
	//�V��
	skydome->Draw();
	////�v���C���[
	GamePostEffect::SetIdColorBuffer(5,PostEffect::kPlayer);
	player->Draw();

	///-------Object3d�`�悱���܂�-------///

	///-------Instance�`�悱������-------///

	//�}�b�v�p�u���b�N
	InstanceObject::DrawPrev(InstanceObject::PipelineType::OutLine);
	mapData->Draw();



	InstanceObject::DrawPrev();
	//�w�i�I�u�W�F�N�g
	backGround->Draw();


	///-------Instance�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///
	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void GameScene::AfterBloomDraw()
{
	player->EffectDraw();
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

	//UI�֌W
	userInterface_->Draw();

	//�X�e�[�W�N���A
	stageClear_->Draw();

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void GameScene::DrawImageForUI()
{
	imageUiRenderer->DrawCameraDescription();
	imageUiRenderer->DrawMoveDescription();


}

void GameScene::MenuAction()
{
	//���j���[���J���Ă��Ȃ���Δ�����
	if (!userInterface_->GetMenuFlag()) { return; }
	//����̃X�y�[�X�L�[�������Ă��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//�X�y�[�X�L�[���������u�ԂɑI������Ă��鍀�ڂɂ���ċ�����ݒ�
	//�X�e�[�W�I���V�[���ւ̈ڍs
	if (userInterface_->GetSelectionNumber() == (int)UserInterface::GameSceneItem::SceneChangeStageSelect) {
		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "STAGESELECT");
		//se�Đ�
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}
	else if (userInterface_->GetSelectionNumber() == (int)UserInterface::GameSceneItem::SceneChangeTitle) {
		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "TITLE");
		//se�Đ�
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}

	//binary�폜
	DeleteBinary();
}

void GameScene::ReCreate()
{
	mapData->ReCreateMapBlock(StageManager::GetSelectStage());
	player->ReCreate(mapData->GetPlayerCreateMapChipNum(), mapData->GetShiftPos());
	player->SetMoveSurfacePhase(mapData->GetInstallationSurface());
	PlayerActionManager::SetMapChipNum(mapData->GetMapChipNum());
	camera->SetReCreateMove(mapData->GetCameraDist());
	skydome->SetIsRotate(true);
}

void GameScene::RestartGame()
{
	//���̃X�e�[�W���J�n���邽�߂Ƀt���O�Ȃǂ����Z�b�g
	isStageClear = false;
	mapData->SetIsReCreateEnd(false);
	skydome->SetIsRotate(false);
	userInterface_->SetMenuFlag(false);
	userInterface_->DrawerSpriteReset();

	//�S�ẴX�e�[�W���N���A��̓��ʂȃX�e�[�W�݂̂��̑��̐ݒ�����Z�b�g�������ɔ�����
	if (StageManager::GetSelectStage() == 100) { return; }

	//���̃X�e�[�W���J�n���邽�߂Ƀt���O�Ȃǂ����Z�b�g
	player->Reset();
	camera->Reset();
	orderMaxNum = orderNum = deleteOrderMaxNum = 0;
	KeepBinary(*camera, *player);
}
