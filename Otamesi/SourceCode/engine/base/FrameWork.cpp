#include "FrameWork.h"
#include "FpsCheck.h"
#include "DescHeapSRV.h"
#include "TextureManager.h"
#include "SpriteTextureLoader.h"
#include "ObjObject3d.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "LightGroup.h"
#include "GamePostEffect.h"
#include "InstanceObject.h"
#include "math/Timer.h"

void FrameWork::Run()
{
	//�Q�[��������
	Initialize();

	//�Q�[�����[�v
	while (true) {
		//���t���[���X�V
		Update();

		//�I�����N�G�X�g�������烋�[�v�𔲂���
		if (GetIsEndRequest()) { break; }

		//�`��
		Draw();
	}

	//�Q�[���I��
	Finalize();
}

void FrameWork::Initialize()
{
	//�E�C���h�E�쐬
	win.reset(new WindowApp());
	win->WindowCreate(L"�`�F���W�Q��");

	//DirectX������
	dxbase.reset(new DirectXBase());
	dxbase->Initialize(win.get());

	//���͂̏�����
	input = Input::GetInstance();
	input->Initialize(win.get());

	//����������
	audio = Audio::GetInstance();
	audio->Initialize();

	//SRV�p�f�X�N���v�^�q�[�v�̏�����
	DescHeapSRV::Initialize(dxbase->GetDevice(), dxbase->GetCmdList());
	//�e�N�X�`���Ǘ��̏�����
	TextureManager::SetDevice(dxbase->GetDevice());

	//�X�v���C�g���ʕ���������
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxbase->GetDevice(), dxbase->GetCmdList(), win->window_width, win->window_height);
	//�X�v���C�g�e�N�X�`���S�ǂݍ���
	SpriteTextureLoader::TextureLoad();

	//�f�o�b�O�e�L�X�g������
	debugText = DebugText::GetInstance();
	debugText->Initialize(SpriteTextureLoader::GetTexture(SpriteTextureLoader::DebugFont));

	//�|�X�g�G�t�F�N�g���ʏ���������
	PostEffect::PostEffectCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//�Q�[�����ʃ|�X�g�G�t�F�N�g�̏�����
	GamePostEffect::Initialize();


	afterBloom->AfterBloomCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	afterBloom.reset(AfterBloom::Create());

	outLine->OutLineCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	outLine.reset(OutLine::Create());
	outLineDraw->OutLineCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	outLineDraw.reset(OutLineDraw::Create());
	//�V���h�E�}�b�v���ʏ���������
	ShadowMap::ShadowMapCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//�V���h�E�}�b�v�̏�����
	shadowMap.reset(ShadowMap::Create());

	//obj�I�u�W�F�N�g3d���ʏ���������
	ObjObject3d::Object3dCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	ObjModel::SetShadowMapTexture(shadowMap->GetTexture());

	//obj�I�u�W�F�N�g3d���ʏ���������
	InstanceObject::InstanceObjectCommon(dxbase->GetDevice(), dxbase->GetCmdList());

	//FBXLoader������
	FbxLoader::GetInstance()->Initialize(dxbase->GetDevice());

	//FBX�I�u�W�F�N�g3d����
	FbxObject3d::SetDevice(dxbase->GetDevice());
	FbxObject3d::SetCommandList(dxbase->GetCmdList());
	//�O���t�B�b�N�X�p�C�v���C������
	FbxObject3d::CreateGraphicsPipeline();

	//���C�g���ʏ���������
	LightGroup::StaticInitialize(dxbase->GetDevice());

	//�p�[�e�B�N�����ʏ���������
	ParticleManager::ParticleManagerCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//�p�[�e�B�N���G�~�b�^�[������
	ParticleEmitter::GetInstance()->Initialize();

	//�S�V�[���Ŏg�p����e�N�X�`���̖������m�肳����
	DescHeapSRV::SetAllSceneTextureNum();

	// �^�C�}�[singleton����
	Timer::CreateSingleton();
;
}

void FrameWork::Finalize()
{
	SceneManager::GetInstance()->Finalize();
	//FBXLoader���
	FbxLoader::GetInstance()->Finalize();

	// singleton���
	SingletonFinalizer::Finalize();

	//audio���
	audio->Finalize();

	//�E�C���h�E���
	win->WindowRelease();
}

void FrameWork::Update()
{
	// �^�C�}�[�v���J�n
	Timer::GetInstance().InstrumentationStart();

	//���͂̍X�V
	input->Update();

	//�V�[���X�V
	SceneManager::GetInstance()->Update();

	//���b�Z�[�W
	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (win->MessageProc()) {
		isEndRequest = true;
		return;
	}
	//�e�V�[������I�����N�G�X�g������΃Q�[�����[�v�I��
	if (SceneManager::GetInstance()->GetIsEndRequest()) {
		isEndRequest = true;
		return;
	}

	//�t���[�����[�g�m�F
	//FpsCheck::Display();
}

void FrameWork::Draw()
{
	//SRV�p���ʃf�X�N���v�^�q�[�vSetDescriptorHeaps
	DescHeapSRV::SetDescriptorHeaps();

	//�V���h�E�}�b�v�̃����_�[�e�N�X�`���ւ̕`��
	shadowMap->DrawScenePrev();
	SceneManager::GetInstance()->Draw3DLightView();
	shadowMap->DrawSceneRear();

	//�Q�[���|�X�g�G�t�F�N�g�ւ̕`��
	GamePostEffect::DrawScenePrev();
	SceneManager::GetInstance()->Draw3D();
	GamePostEffect::DrawSceneRear();

	// �P�̂Ŕ���������̗p
	afterBloom->DrawScenePrev(GamePostEffect::GetPostEffect()->GetDsv());
	SceneManager::GetInstance()->AfterBloomDraw();
	afterBloom->DrawSceneRear();
	
	// �A�E�g���C���`��p
	outLine->DrawScenePrev(GamePostEffect::GetPostEffect()->GetDsv());
	SceneManager::GetInstance()->OutLineDraw();
	outLine->DrawSceneRear();

	outLineDraw->DrawScenePrev(GamePostEffect::GetPostEffect()->GetDsv());
	outLine->Draw();
	outLineDraw->DrawSceneRear();


	//�O���t�B�b�N�X�R�}���h(�O)
	dxbase->GraphicsCommandPrev();

	//�V�[���̔w�i�X�v���C�g�`��
	SceneManager::GetInstance()->DrawBackSprite();

	//�Q�[���|�X�g�G�t�F�N�g�̕`��
	GamePostEffect::Draw();
	afterBloom->Draw();				// �A�t�^�[�u���[����`��
	outLineDraw->Draw();
	//�V�[���̑O�i�X�v���C�g�`��
	SceneManager::GetInstance()->DrawFrontSprite();

	//�X�v���C�g���ʃR�}���h
	spriteCommon->DrawPrev();
	//�f�o�b�N�e�L�X�g�`��
	debugText->DrawAll();

	//�O���t�B�b�N�X�R�}���h(��)
	dxbase->GraphicsCommandRear();

	Timer::GetInstance().InstrumentationEnd();

}
