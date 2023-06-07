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
	//メニュー関係
	TextureManager::LoadTexture(textures[MenuBackScreen], directoryPath, "BackScreen.png");
	TextureManager::LoadTexture(textures[MenuFrame], directoryPath, "frame.png");
	//操作方法関係
	TextureManager::LoadTexture(textures[HowToPlayMove], directoryPath, "move.png");
	TextureManager::LoadTexture(textures[HowToPlayCamera], directoryPath, "camera.png");
	TextureManager::LoadTexture(textures[HowToPlayPause], directoryPath, "pause.png");

	TextureManager::LoadTexture(textures[ParanomaSky], directoryPath, "ParanomaSkyDorm.png");
}
