#include "TitleScene.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ParticleEmitter.h"
#include "MapBlockData.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"

void TitleScene::Initialize()
{
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
	//�f�o�b�O�p�e�L�X�g
	DebugText::GetInstance()->Print("TITLE SCENE", 350, 200, 5);
	DebugText::GetInstance()->Print("PRESS SPACE", 600, 600);

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//�V�[���؂�ւ�
		SceneChangeStart({ 0,0,0,0 }, 60, 60, 60, "GAME");
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
}

void TitleScene::Draw3DLightView()
{
}

void TitleScene::DrawFrontSprite()
{
	//�X�v���C�g���ʃR�}���h
	SpriteCommon::GetInstance()->DrawPrev();
	///-------�X�v���C�g�`�悱������-------///

	//�V�[���ύX���o�`��
	SceneChangeEffect::Draw();


	///-------�X�v���C�g�`�悱���܂�-------///
}
