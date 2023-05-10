#include "FrameWork.h"
#include "FpsCheck.h"
#include "DescHeapSRV.h"
#include "TextureManager.h"
#include "SpriteTextureLoader.h"
#include "ObjObject3d.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "LightGroup.h"

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
	win->WindowCreate(L"�����߂�");

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

	//�V���h�E�}�b�v���ʏ���������
	ShadowMap::ShadowMapCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//�V���h�E�}�b�v�̏�����
	shadowMap.reset(ShadowMap::Create());

	//obj�I�u�W�F�N�g3d���ʏ���������
	ObjObject3d::Object3dCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	ObjModel::SetShadowMapTexture(shadowMap->GetTexture());

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

	//�S�V�[���Ŏg�p����e�N�X�`���̖������m�肳����
	DescHeapSRV::SetAllSceneTextureNum();
}

void FrameWork::Finalize()
{
	//FBXLoader���
	FbxLoader::GetInstance()->Finalize();

	//audio���
	audio->Finalize();

	//�E�C���h�E���
	win->WindowRelease();
}

void FrameWork::Update()
{
	//���b�Z�[�W
	//�~�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
	if (win->MessageProc()) {
		isEndRequest = true;
		return;
	}
	//�G�X�P�[�v�L�[�ŃQ�[�����[�v�I��
	if (input->PushKey(DIK_ESCAPE)) {
		isEndRequest = true;
		return;
	}

	//���͂̍X�V
	input->Update();

	//�V�[���X�V
	SceneManager::GetInstance()->Update();

	//�t���[�����[�g�m�F
	FpsCheck::Display();
}

void FrameWork::Draw()
{
	//SRV�p���ʃf�X�N���v�^�q�[�vSetDescriptorHeaps
	DescHeapSRV::SetDescriptorHeaps();

	//�V���h�E�}�b�v�̃����_�[�e�N�X�`���ւ̕`��
	shadowMap->DrawScenePrev();
	SceneManager::GetInstance()->Draw3DLightView();
	shadowMap->DrawSceneRear();

	//�O���t�B�b�N�X�R�}���h(�O)
	dxbase->GraphicsCommandPrev();

	//�V�[���̔w�i�X�v���C�g�`��
	SceneManager::GetInstance()->DrawBackSprite();

	//�V�[���̕`��
	SceneManager::GetInstance()->Draw3D();

	//�V�[���̑O�i�X�v���C�g�`��
	SceneManager::GetInstance()->DrawFrontSprite();

	//�X�v���C�g���ʃR�}���h
	spriteCommon->DrawPrev();
	//�f�o�b�N�e�L�X�g�`��
	debugText->DrawAll();

	//�O���t�B�b�N�X�R�}���h(��)
	dxbase->GraphicsCommandRear();
}
