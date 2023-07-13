#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "GameCamera.h"
#include "GameLightCamera.h"
#include "Player.h"
#include "MapDataStage.h"
#include "BackGround.h"
#include "Skydome.h"
#include "UserInterface.h"

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene :public BaseGameScene
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite() override;

	/// <summary>
	/// 3D�V�[���`��
	/// </summary>
	void Draw3D() override;
	void AfterBloomDraw() override;

	/// <summary>
	/// 3D�V�[���`��(���C�g���猩�����_)
	/// </summary>
	void Draw3DLightView() override;

	/// <summary>
	/// �O�i�X�v���C�g�`��
	/// </summary>
	void DrawFrontSprite() override;

	/// <summary>
	/// ���j���[�̓���
	/// </summary>
	void MenuAction() override;

	/// <summary>
	/// frame���Ƃ̏�����
	/// </summary>
	void FrameReset()override;


private: //�����o�ϐ�
	//�J����
	std::unique_ptr<GameCamera> camera;
	//�e�p�����J����
	std::unique_ptr<GameLightCamera> lightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//obj���f��
	std::unique_ptr<ObjModel> modelPlayer;
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelPlayerEffect;

	//�v���C���[
	std::unique_ptr<Player> player;

	//�X�e�[�W�p�}�b�v�f�[�^
	std::unique_ptr<MapDataStage> mapData;

	//�V��
	std::unique_ptr<Skydome> skydome;

	//�w�i�I�u�W�F�N�g
	std::unique_ptr<BackGround> backGround;

	//UI�֌W
	std::unique_ptr<UserInterface> userInterface_;

	//�^�C�g�����S
	std::unique_ptr<Sprite> titleLogo;

	//�X�e�[�W�N���A�t���O
	bool isStageClear = false;
};