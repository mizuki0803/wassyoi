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
	TextureManager::LoadTexture(textures[MenuFrame], directoryPath, "menuFrame.png");
	TextureManager::LoadTexture(textures[SoundVolumeBar], directoryPath, "soundVolumeBar.png");
	TextureManager::LoadTexture(textures[SoundVolumePointer], directoryPath, "soundVolumePointer.png");
	//操作方法関係
	TextureManager::LoadTexture(textures[Husen], directoryPath, "husen.png");
	TextureManager::LoadTexture(textures[MenuIconDef], directoryPath, "MenuIcon1.png");
	TextureManager::LoadTexture(textures[MenuIconArrow], directoryPath, "MenuIcon2.png");
	TextureManager::LoadTexture(textures[HusenL], directoryPath, "husen_L.png");
	TextureManager::LoadTexture(textures[HusenR], directoryPath, "husen_R.png");
	TextureManager::LoadTexture(textures[HusenH], directoryPath, "husen_hint.png");

	TextureManager::LoadTexture(textures[HowToPlayPlayer], directoryPath, "howToPlayPlayerText.png");
	TextureManager::LoadTexture(textures[HowToPlayCamera], directoryPath, "howToPlayCameraText.png");
	TextureManager::LoadTexture(textures[Hint1Text], directoryPath, "hint1Text.png");
	TextureManager::LoadTexture(textures[Hint2Text], directoryPath, "hint2Text.png");
	TextureManager::LoadTexture(textures[HowToPlayChengeDemension], directoryPath, "changeDemension.png");
	TextureManager::LoadTexture(textures[UnDoReDo], directoryPath, "UndoRedo.png");
	TextureManager::LoadTexture(textures[HowToPlayMenu], directoryPath, "menuText.png");
	//ステージクリア関係
	TextureManager::LoadTexture(textures[StageClear], directoryPath, "Clear_Front.png");
	TextureManager::LoadTexture(textures[ClearBack], directoryPath, "Clear_Back.png");
	TextureManager::LoadTexture(textures[PressSpace], directoryPath, "pressSpace.png");

	TextureManager::LoadTexture(textures[ParanomaSky], directoryPath, "ParanomaSkyDorm.png");
}
