#pragma once
#include "Texture.h"

/// <summary>
/// �X�v���C�g�e�N�X�`���ǂݍ��݃N���X
/// </summary>
class SpriteTextureLoader
{
public: //�e�N�X�`����
	enum TextureName {
		DebugFont,		//�f�o�b�O�p�t�H���g
		White,			//�^����
		Number,			//����
		TitleLogo,		//�^�C�g�����S
		Block,			//�u���b�N
		MenuBackScreen, //���j���[��ʗp�̔w�i
		MenuFrame,		//���j���[�̘g�g��
		SoundVolumeBar,	//���ʕύX�p�o�[
		SoundVolumePointer,	//���ʕύX�p�|�C���^�[
		Husen,
		HusenL,
		HusenR,
		HowToPlayMove,	//������@(�ړ�)
		HowToPlayCamera,//������@(�J����)
		HowToPlayChengeDemension,//������@(�����ύX)
		HowToPlayReset,	//������@(���Z�b�g)
		HowToPlayStageSelect,	//������@(�X�e�[�W�Z���N�g)
		Hint1,	//�q���g1
		Hint2,	//�q���g2
		StageClear,		//�X�e�[�W�N���A
		ClearBack,		//�X�e�[�W�N���A�̔w�i
		PressSpace,		//PRESS SPACE
		ParanomaSky,	// �X�J�C�h�[���p

		SpriteTextureNum,	//�X�v���C�g�e�N�X�`���̖���
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// �e�N�X�`���S�ǂݍ���
	/// </summary>
	static void TextureLoad();

	//getter
	static Texture& GetTexture(const TextureName textureName) { return textures[textureName]; };

private: //�ÓI�����o�ϐ�
	//�e�N�X�`��
	static Texture textures[SpriteTextureNum];
};