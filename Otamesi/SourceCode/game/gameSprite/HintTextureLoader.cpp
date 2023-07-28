#include "HintTextureLoader.h"
#include "TextureManager.h"

std::array<Texture, 2> HintTextureLoader::textures;

void HintTextureLoader::LoadTextures(const int stageNum)
{
	//ヒントテクスチャ用ディレクトリパス
	const std::string directoryPath = "Resources/hintTexture/";
	//ステージ番号を文字列に変換する
	std::string stageName = std::to_string(stageNum);

	//各ステージヒントが2つあるので、2枚読み込む
	for (int i = 0; i < 2; i++) {
		//ヒント番号を文字列に変換する
		std::string hintNum = std::to_string(i + 1);
		//ファイルパスを結合
		std::string filename = stageName + "_" + hintNum + ".png";

		//テクスチャ読み込み
		TextureManager::LoadTexture(textures[i], directoryPath, filename);
	}
}
