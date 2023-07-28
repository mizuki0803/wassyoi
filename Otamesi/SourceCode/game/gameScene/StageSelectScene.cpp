#include "StageSelectScene.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ParticleEmitter.h"
#include "MapBlockData.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "MapDataStageSelect.h"

void StageSelectScene::Initialize()
{
	//���C�g����
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, false);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);

	//obj���烂�f���f�[�^��ǂݍ���
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01", true));

	//�}�b�v�Ǘ�����
	mapDataManager.reset(MapDataStageSelectManager::Create());

	//�J����������
	camera.reset(new Camera());
	camera->Initialize();
	camera->SetEye({ 0, 5, -70 });
	//�e�p�����J����������
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -100, 100, -300 });
	lightCamera->SetProjectionNum({ 400, 400 }, { -400, -400 });


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

	//��ʂɃp�[�e�B�N�����c�邱�Ƃ�����̂őS�č폜���Ă���
	ParticleEmitter::GetInstance()->AllDelete();

	//�|�X�g�G�t�F�N�g�̃u���[���������Ă���
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//UI�֌W����
	userInterface_ = UserInterface::Create(UserInterface::GamePhase::Selection);
}

void StageSelectScene::Finalize()
{
}

void StageSelectScene::Update()
{
	//�G�X�P�[�v�L�[�Ń��j���[���
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
	mapDataManager->SetNotMove(userInterface_->GetMenuFlag());
	MenuAction();

	//�J�����X�V
	camera->Update();
	lightCamera->Update();

	//���C�g�X�V
	lightGroup->Update();

	//�I�u�W�F�N�g�X�V
	//�}�b�v�p�u���b�N
	mapDataManager->Update();
	//�X�e�[�W��ύX�����Ƃ��̂݁A���X�v���C�g��`�悷�邩���肷��
	if (mapDataManager->GetIsChangeStage()) {
		userInterface_->GetStageSelectArrow()->SelectStageCheckDraw();
	}
	//�V��
	skydome->Update();

	//UI�̍X�V
	userInterface_->Update();

	//�X�y�[�X�L�[�ŃX�e�[�W���m�肵�A�Q�[���V�[����
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && !userInterface_->GetMenuFlag()) {
		//se�Đ�
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);

		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
	}

	//�V�[���ύX���
	SceneChangeMode();
	//�V�[���ύX���o�X�V
	SceneChangeEffect::Update();
}

void StageSelectScene::DrawBackSprite()
{
}

void StageSelectScene::Draw3D()
{
	//Object3d���ʃR�}���h
	ObjObject3d::DrawPrev();
	///-------Object3d�`�悱������-------///

	//�V��
	skydome->Draw();

	///-------Object3d�`�悱���܂�-------///

	///-------Instance�`�悱������-------///

	InstanceObject::DrawPrev();
	//�}�b�v�p�u���b�N
	mapDataManager->Draw();

	///-------Instance�`�悱���܂�-------///

	///-------�p�[�e�B�N���`�悱������-------///

	//�p�[�e�B�N���`��
	ParticleEmitter::GetInstance()->DrawAll();

	///-------�p�[�e�B�N���`�悱���܂�-------///
}

void StageSelectScene::Draw3DLightView()
{
}

void StageSelectScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//�X�e�[�W�ԍ�
	mapDataManager->DrawUI();

	//UI�֌W
	userInterface_->Draw();

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}

void StageSelectScene::MenuAction()
{
	//���j���[���J���Ă��Ȃ���Δ�����
	if (!userInterface_->GetMenuFlag()) { return; }
	//����̃X�y�[�X�L�[�������Ă��Ȃ���Δ�����
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//�X�y�[�X�L�[���������u�ԂɑI������Ă��鍀�ڂɂ���ċ�����ݒ�
	//�^�C�g���V�[���Ɉړ�
	if (userInterface_->GetSelectionNumber() == (int)UserInterface::StageSelectSceneItem::SceneChangeTitle) {
		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "TITLE");
		//se�Đ�
		Audio::GetInstance()->PlayWave(Audio::SoundName::button);
	}

	//binary�폜
	DeleteBinary();
}

void StageSelectScene::FrameReset()
{
	mapDataManager->FrameReset();
}
