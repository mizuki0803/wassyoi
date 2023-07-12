#include "SceneManager.h"
#include "DescHeapSRV.h"
#include <cassert>

SceneManager::~SceneManager()
{
	//�Ō�̃V�[���̏I���Ɖ��
	delete scene;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;

	return &instance;
}

void SceneManager::Finalize()
{
	scene->Finalize();
}

void SceneManager::Update()
{
	//�V�[���؂�ւ�������Ȃ�
	if (nextScene) {
		//���V�[��������Ȃ�
		if (scene) {
			//���V�[���̏I��
			delete scene;
		}
		//�V�[����؂�ւ�
		scene = nextScene;
		//���̃V�[���͋�ɂ��Ă���
		nextScene = nullptr;

		//�S�V�[�����ʂŎg�p����e�N�X�`���̖����܂ŃC���f�b�N�X��߂�
		DescHeapSRV::TextureDestruction();

		//�V�����V�[��������
		scene->Initialize();
	}

	//���t���[���X�V
	scene->Update();
}

void SceneManager::DrawBackSprite()
{
	//�w�i�X�v���C�g�`��
	scene->DrawBackSprite();
}

void SceneManager::Draw3D()
{
	//3D�V�[���`��
	scene->Draw3D();
}

void SceneManager::AfterBloomDraw()
{
	scene->AfterBloomDraw();
}

void SceneManager::OutLineDraw()
{

	scene->OutLineDraw();
}

void SceneManager::Draw3DLightView()
{
	//3D�V�[���`��(���C�g����̎��_)
	scene->Draw3DLightView();
}

void SceneManager::DrawFrontSprite()
{
	//�O�i�X�v���C�g�`��
	scene->DrawFrontSprite();
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//���̃V�[�����Z�b�g
	nextScene = sceneFactory->CreateScene(sceneName);
}
