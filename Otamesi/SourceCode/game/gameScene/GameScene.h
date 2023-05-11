#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "GameCamera.h"
#include "LightCamera.h"
#include "Player.h"
#include "Block.h"

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene :public BaseGameScene
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

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

private: //�����o�֐�

private: //�����o�ϐ�
	//�J����
	std::unique_ptr<GameCamera> camera;
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

	//obj���f��
	std::unique_ptr<ObjModel> modelPlayer;
	std::unique_ptr<ObjModel> modelBlock;
	std::unique_ptr<ObjModel> modelGoal;
	std::unique_ptr<ObjModel> modelSkydome;

	//�v���C���[
	std::unique_ptr<Player> player;

	//3�����}�b�v�p��3�����z��
	std::vector<std::vector<std::vector<int>>> mapChipNum;
	//�}�b�v�p�u���b�N
	std::vector<std::unique_ptr<Block>> blocks;

	//�V��
	std::unique_ptr<ObjObject3d> skydome;

	//�X�e�[�W�N���A�t���O
	bool isStageClear = false;

	int blockActTimer_ = 0;
};