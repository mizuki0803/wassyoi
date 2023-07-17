#include "HintTextureLoader.h"
#include "TextureManager.h"

std::array<Texture, 2> HintTextureLoader::textures;

void HintTextureLoader::LoadTextures(const int stageNum)
{
	//�q���g�e�N�X�`���p�f�B���N�g���p�X
	const std::string directoryPath = "Resources/hintTexture/";
	//�X�e�[�W�ԍ��𕶎���ɕϊ�����
	std::string stageName = std::to_string(stageNum);

	//�e�X�e�[�W�q���g��2����̂ŁA2���ǂݍ���
	for (int i = 0; i < 2; i++) {
		//�q���g�ԍ��𕶎���ɕϊ�����
		std::string hintNum = std::to_string(i + 1);
		//�t�@�C���p�X������
		std::string filename = stageName + "_" + hintNum + ".png";

		//�e�N�X�`���ǂݍ���
		TextureManager::LoadTexture(textures[i], directoryPath, filename);
	}
}
