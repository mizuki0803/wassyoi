#include "StageSelectArrow.h"
#include "SpriteTextureLoader.h"
#include "StageManager.h"
#include "WindowApp.h"

StageSelectArrow* StageSelectArrow::Create()
{
	//�C���X�^���X����
	StageSelectArrow* instance = new StageSelectArrow();

	// ������
	instance->Initialize();

	return instance;
}

void StageSelectArrow::Initialize()
{
	//�X�v���C�g����
	const Vector2 centerPos = { WindowApp::window_width / 2, WindowApp::window_height / 2 }; //	���S���W
	const Vector2 positionCenterDistance = { 500, 0 }; //���S����̋���

	//�����
	arrows[0].reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::StageSelectArrow), centerPos - positionCenterDistance, { 0.5f, 0.5f }, true));
	//�E���
	arrows[1].reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::StageSelectArrow), centerPos + positionCenterDistance));

	//�`��`�F�b�N�ƐF�ݒ�
	for (int i = 0; i < 2; i++) {
		arrows[i]->SetColor({ 0.8f, 0, 0, 1 });
		isDraws[i] = true;
	}
	SelectStageCheckDraw();
}

void StageSelectArrow::Update()
{
	//�X�v���C�g�`��
	for (int i = 0; i < 2; i++) {
		//�`�悵�Ȃ���ԂȂ��΂�
		if (!isDraws[i]) { continue; }
		arrows[i]->Update();
	}
}

void StageSelectArrow::Draw()
{
	//�X�v���C�g�`��
	for (int i = 0; i < 2; i++) {
		//�`�悵�Ȃ���ԂȂ��΂�
		if (!isDraws[i]) { continue; }
		arrows[i]->Draw();
	}
}

void StageSelectArrow::SelectStageCheckDraw()
{
	//�X�e�[�W1��I�𒆂̏ꍇ�͍����̕`����~
	if (StageManager::GetSelectStage() == 1) {
		isDraws[0] = false;
		return;
	}
	//�Ō�̃X�e�[�W��I�𒆂̏ꍇ�͉E���̕`����~
	else if (StageManager::GetSelectStage() == StageManager::GetStageNum()) {
		isDraws[1] = false;
		return;
	}

	//����ȊO�Ȃ�`����s��
	for (int i = 0; i < 2; i++) {
		isDraws[i] = true;
	}
}
