#pragma once
#include "Texture.h"
#include <string>

class TextureManager
{
public: //�ÓI�����o�֐�	
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	/// <param name="texture">�ǂݍ��񂾃e�N�X�`����ۑ�����ꏊ</param> 
	/// <param name="directoryPath">�p�X</param>
	/// <param name="filename">�t�@�C����</param>
	static void LoadTexture(Texture& texture, const std::string& directoryPath, const std::string& filename);

	//setter
	static void SetDevice(ID3D12Device* device) { TextureManager::dev = device; };

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
};