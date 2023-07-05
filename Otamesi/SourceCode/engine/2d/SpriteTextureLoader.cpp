#include "SpriteTextureLoader.h"
#include "TextureManager.h"

Texture SpriteTextureLoader::textures[SpriteTextureLoader::SpriteTextureNum];

void SpriteTextureLoader::TextureLoad()
{
	//スプライトテクスチャ用ディレクトリパス
	const std::string directoryPath = "Resources/spriteTexture/";

	//スプライト用テクスチャ読み込み
	TextureManager::LoadTexture(textures[DebugFont], directoryPath, "debugFont.png");
	TextureManager::LoadTexture(textures[White], directoryPath, "white1x1.png");
	TextureManager::LoadTexture(textures[Number], directoryPath, "number.png");
	TextureManager::LoadTexture(textures[Block], directoryPath, "block.png");
	//タイトルロゴ
	TextureManager::LoadTexture(textures[TitleLogo], directoryPath, "title.png");
	//メニュー関係
	TextureManager::LoadTexture(textures[MenuBackScreen], directoryPath, "BackScreen.png");
	TextureManager::LoadTexture(textures[MenuFrame], directoryPath, "frame.png");
	TextureManager::LoadTexture(textures[SoundVolumeBar], directoryPath, "soundVolumeBar.png");
	TextureManager::LoadTexture(textures[SoundVolumePointer], directoryPath, "soundVolumePointer.png");
	//操作方法関係
	TextureManager::LoadTexture(textures[HowToPlayMove], directoryPath, "move.png");
	TextureManager::LoadTexture(textures[HowToPlayCamera], directoryPath, "camera.png");
	TextureManager::LoadTexture(textures[HowToPlayChengeDemension], directoryPath, "changeDemension.png");
	TextureManager::LoadTexture(textures[HowToPlayUndo], directoryPath, "undo.png");
	TextureManager::LoadTexture(textures[HowToPlayRedo], directoryPath, "redo.png");
	TextureManager::LoadTexture(textures[HowToPlayReset], directoryPath, "reset.png");
	TextureManager::LoadTexture(textures[HowToPlayStageSelect], directoryPath, "stageSelect.png");
	//ステージクリア関係
	TextureManager::LoadTexture(textures[StageClear], directoryPath, "Clear_Front.png");
	TextureManager::LoadTexture(textures[ClearBack], directoryPath, "Clear_Back.png");
	TextureManager::LoadTexture(textures[PressSpace], directoryPath, "pressSpace.png");

	TextureManager::LoadTexture(textures[ParanomaSky], directoryPath, "ParanomaSkyDorm.png");
}
