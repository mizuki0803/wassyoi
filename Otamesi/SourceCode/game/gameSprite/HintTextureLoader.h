#pragma once
#include "Texture.h"
#include <array>

/// <summary>
/// �q���g�摜�ǂݍ���
/// </summary>
class HintTextureLoader
{
public: //�����o�֐�
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	static void LoadTextures(const int stageNum);

	//getter
	static Texture& GetTexture(const int num) { return textures[num]; }


private: //�����o�ϐ�
	//�e�N�X�`��
	static std::array<Texture, 2> textures;
};