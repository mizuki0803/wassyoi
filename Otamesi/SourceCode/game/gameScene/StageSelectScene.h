#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "ObjObject3d.h"
#include "Camera.h"
#include "MapDataStageSelectManager.h"
#include "UserInterface.h"

/// <summary>
/// �X�e�[�W�I���V�[��
/// </summary>
class StageSelectScene :public BaseGameScene
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

private: //�����o�ϐ�
	//�J����
	std::unique_ptr<Camera> camera;
	//�e�p�����J����
	std::unique_ptr<LightCamera> lightCamera;

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

	//�}�b�v�f�[�^
	std::unique_ptr<MapDataStageSelectManager> mapDataManager;

	//obj���f��
	std::unique_ptr<ObjModel> modelSkydome;

	//�V��
	std::unique_ptr<ObjObject3d> skydome;

	//UI�֌W
	std::unique_ptr<UserInterface> userInterface_;
};