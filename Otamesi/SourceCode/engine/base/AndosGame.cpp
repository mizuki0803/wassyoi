#include "AndosGame.h"
#include "SceneFactory.h"


void AndosGame::Initialize()
{
	//���N���X�̏���������
	FrameWork::Initialize();

	//�V�[���H��𐶐��A�}�l�[�W���[�ɃZ�b�g
	BaseScene::SetDirectXbase(dxbase.get());
	sceneFactory.reset(new SceneFactory());
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory.get());

	//�V�[���}�l�[�W���[�ɍŏ��̃V�[�����Z�b�g
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void AndosGame::Finalize()
{
	//���N���X�̏I������
	FrameWork::Finalize();
}

void AndosGame::Update()
{
	//���N���X�̍X�V����
	FrameWork::Update();
}

void AndosGame::Draw()
{
	//���N���X�̕`�揈��
	FrameWork::Draw();
}
