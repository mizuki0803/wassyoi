#include "SpriteTextureLoader.h"
#include "TextureManager.h"

Texture SpriteTextureLoader::textures[SpriteTextureLoader::SpriteTextureNum];

void SpriteTextureLoader::TextureLoad()
{
	//�X�v���C�g�e�N�X�`���p�f�B���N�g���p�X
	const std::string directoryPath = "Resources/spriteTexture/";

	//�X�v���C�g�p�e�N�X�`���ǂݍ���
	TextureManager::LoadTexture(textures[DebugFont], directoryPath, "debugFont.png");
	TextureManager::LoadTexture(textures[White], directoryPath, "white1x1.png");
	TextureManager::LoadTexture(textures[Number], directoryPath, "number.png");
	TextureManager::LoadTexture(textures[Block], directoryPath, "block.png");
	//�^�C�g�����S
	TextureManager::LoadTexture(textures[TitleLogo], directoryPath, "title.png");
	//���j���[�֌W
	TextureManager::LoadTexture(textures[MenuBackScreen], directoryPath, "BackScreen.png");
	TextureManager::LoadTexture(textures[MenuFrame], directoryPath, "frame.png");
	TextureManager::LoadTexture(textures[SoundVolumeBar], directoryPath, "soundVolumeBar.png");
	TextureManager::LoadTexture(textures[SoundVolumePointer], directoryPath, "soundVolumePointer.png");
	//������@�֌W
	TextureManager::LoadTexture(textures[Husen], directoryPath, "husen.png");
	TextureManager::LoadTexture(textures[HusenL], directoryPath, "husen_L.png");
	TextureManager::LoadTexture(textures[HusenR], directoryPath, "husen_R.png");

	TextureManager::LoadTexture(textures[HowToPlayPlayer], directoryPath, "howToPlayPlayerText.png");
	TextureManager::LoadTexture(textures[HowToPlayCamera], directoryPath, "howToPlayCameraText.png");
	TextureManager::LoadTexture(textures[Hint1Text], directoryPath, "hint1Text.png");
	TextureManager::LoadTexture(textures[Hint2Text], directoryPath, "hint2Text.png");
	TextureManager::LoadTexture(textures[HowToPlayChengeDemension], directoryPath, "changeDemension.png");
	TextureManager::LoadTexture(textures[HowToPlayMenu], directoryPath, "stageSelect.png");
	//�X�e�[�W�N���A�֌W
	TextureManager::LoadTexture(textures[StageClear], directoryPath, "Clear_Front.png");
	TextureManager::LoadTexture(textures[ClearBack], directoryPath, "Clear_Back.png");
	TextureManager::LoadTexture(textures[PressSpace], directoryPath, "pressSpace.png");

	TextureManager::LoadTexture(textures[ParanomaSky], directoryPath, "ParanomaSkyDorm.png");
}
