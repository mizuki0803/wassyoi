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
		Block,			//�u���b�N
		MenuBackScreen, //���j���[��ʗp�̔w�i
		MenuFrame,		//���j���[�̘g�g��
		HowToPlayMove,	//������@(�ړ�)
		HowToPlayCamera,//������@(�J����)
		HowToPlayPause,	//������@(�|�[�Y)
		ParanomaSky,	// �X�J�C�h�[���p

		SpriteTextureNum,	//�X�v���C�g�e�N�X�`���̖���
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// �e�N�X�`���S�ǂݍ���
	/// </summary>
	static void TextureLoad();

	//getter
	static Texture GetTexture(const TextureName textureName) { return textures[textureName]; };

private: //�ÓI�����o�ϐ�
	//�e�N�X�`��
	static Texture textures[SpriteTextureNum];
};