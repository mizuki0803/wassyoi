#include "TitleScene.h"
#include "WindowApp.h"
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
#include "SpriteTextureLoader.h"
#include "OutLine.h"

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
	const Vector3 stageCenterPos = {}; //�X�e�[�W�������W
	camera->Initialize(distanceStageCenter, stageCenterPos);
	//�e�p�����J����������
	const float lightCameraCenterDistance = 80;
	lightCamera.reset(GameLightCamera::Create(lightCameraCenterDistance));
	lightCamera->SetProjectionNum({ 250, 250 }, { -250, -250 });

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

	//UI�֌W����
	userInterface_ = UserInterface::Create(UserInterface::GamePhase::Title);
	//�^�C�g�����S����
	titleLogo.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::TitleLogo)));
	titleLogo->SetPosition({ WindowApp::window_width / 2, 140 });
	titleLogo->SetTexSize({1658, 518});
	titleLogo->SetSize(titleLogo->GetTexSize() * 0.4f);
}

void TitleScene::Finalize()
{
	DeleteBinary();
}

void TitleScene::Update()
{
	//�f�o�b�O�p�e�L�X�g
	//DebugText::GetInstance()->Print("TITLE SCENE", 350, 200, 5);

	if (!isStageClear) {
		//�����ύX���\��UI�ɓ`����
		userInterface_->IsChangeDimensionCheck(player->ChangeDimensionStartCheck());

		//undo
		if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Z)) {
			Undo(camera.get(), player.get());

		}
		//redo
		else if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_Y)) {
			Redo(camera.get(), player.get());
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
			camera->SetIsStageClear(true);

			//�V�[���؂�ւ�
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
			//binary�폜
			DeleteBinary();
			//���̃X�e�[�W��
			StageManager::NextStageSelect();
		}

		//�G�X�P�[�v�L�[�Ń��j���[
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE))
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

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

	//�^�C�g�����S
	titleLogo->Update();

	//������Ԃ�binary�ۑ�
	if (player->GetIsStartMove()) {
		KeepBinary(*camera, *player);
	}

	//�p�[�e�B�N���X�V
	ParticleEmitter::GetInstance()->Update();

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
	ObjObject3d::DrawOutLinePrev();
	///-------Object3d�`�悱������-------///
	// �A�E�g���C���p�萔�o�b�t�@�Z�b�g
	GamePostEffect::SetIdColorBuffer(5, PostEffect::kNone);
	//�V��
	skydome->Draw();
	////�v���C���[
	GamePostEffect::SetIdColorBuffer(5, PostEffect::kPlayer);
	player->Draw();

	///-------Object3d�`�悱���܂�-------///

	///-------Instance�`�悱������-------///

	//�}�b�v�p�u���b�N
	InstanceObject::DrawPrev(InstanceObject::PipelineType::OutLine);
	mapData->Draw();


	//�w�i�I�u�W�F�N�g
	InstanceObject::DrawPrev();
	backGround->Draw();

	///-------Instance�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void TitleScene::AfterBloomDraw()
{
	player->EffectDraw();
}

void TitleScene::Draw3DLightView()
{
	///-------Instance�`�悱������-------///

	InstanceObject::DrawLightViewPrev();

	//�w�i�I�u�W�F�N�g
	backGround->DrawLightCameraView();

	///-------Instance�`�悱���܂�-------///
}

void TitleScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//UI�֌W
	userInterface_->Draw();

	//�^�C�g�����S
	titleLogo->Draw();

	//�V�[���ύX���o
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void TitleScene::MenuAction()
{
	//���j���[���J���Ă��Ȃ���Δ�����
	if (!userInterface_->GetMenuFlag()) { return; }
	//����̃X�y�[�X�L�[�������Ă��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//�X�y�[�X�L�[���������u�ԂɑI������Ă��鍀�ڂɂ���ċ�����ݒ�
	//.exe�̏I��
	if (userInterface_->GetSelectionNumber() == (int)UserInterface::TitleSceneItem::ExeEnd) {
		isEndRequest = true;
		//se�Đ�
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}

	//binary�폜
	DeleteBinary();
}

void TitleScene::FrameReset()
{
	mapData->FrameReset();
	backGround->FrameReset();
}
