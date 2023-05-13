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
	/// <summary>
	/// �}�b�v�f�[�^�ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	bool LoadMapData(const std::string& fileName);

	/// <summary>
	/// 
	/// </summary>
	void BlockUpdate();

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

	//�}�b�v�̃T�C�Y
	XMINT3 mapSize;
	//3�����}�b�v�p��3�����z��
	std::vector<std::vector<std::vector<int>>> mapChipNum;
	//�}�b�v�p�u���b�N
	std::vector<std::unique_ptr<Block>> blocks;

	//�V��
	std::unique_ptr<ObjObject3d> skydome;

	//�X�e�[�W�N���A�t���O
	bool isStageClear = false;

	// �u���b�N���グ��^�C�}�[
	int blockActTimer_ = 0;
	// �u���b�N���グ�I�������
	bool isBolckUp_ = false;
};