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
	//������@�֌W
	TextureManager::LoadTexture(textures[HowToPlayMove], directoryPath, "move.png");
	TextureManager::LoadTexture(textures[HowToPlayCamera], directoryPath, "camera.png");
	TextureManager::LoadTexture(textures[HowToPlayUndo], directoryPath, "undo.png");
	TextureManager::LoadTexture(textures[HowToPlayRedo], directoryPath, "redo.png");
	TextureManager::LoadTexture(textures[HowToPlayReset], directoryPath, "reset.png");
	TextureManager::LoadTexture(textures[HowToPlayStageSelect], directoryPath, "stageSelect.png");
	//�X�e�[�W�N���A�֌W
	TextureManager::LoadTexture(textures[StageClear], directoryPath, "stageClear.png");
	TextureManager::LoadTexture(textures[PressEnter], directoryPath, "pressEnter.png");

	TextureManager::LoadTexture(textures[ParanomaSky], directoryPath, "ParanomaSkyDorm.png");
}
