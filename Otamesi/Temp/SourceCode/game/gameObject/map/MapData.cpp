#include "MapData.h"
#include "Block.h"
#include "JsonLoader.h"
#include <sstream>

bool MapData::CreateMap(const int selectStageNum)
{
	//jsonマップデータ読み込み
	std::ostringstream  fileName;
	fileName << selectStageNum;
	if (!LoadMapData(fileName.str())) {
		return false;
	}
	//マップブロック生成
	CreateMapBlock();

	Block::StaticInitialize();

	return true;
}

bool MapData::LoadMapData(const std::string& fileName)
{
	//マップデータ情報読み取り
	std::vector<std::vector<std::vector<int>>> inputmap;

	std::array<float, 3> inCameraDist = {};
	if (!JsonLoader::DeserializeJsonMap("Resources/mapdata/" + fileName + ".json",
		&inCameraDist, &installationSurface, &inputmap)) {
		return false;
	}

	//カメラ距離保存
	cameraDist = { inCameraDist[0],inCameraDist[1], inCameraDist[2] };

	//マップのサイズを取得
	const int marginNum = 2; //ゲームの仕様上、上下左右前後に1マスの余白を作る
	mapSize = { int(inputmap[0][0].size()) + marginNum, int(inputmap.size()) + marginNum, int(inputmap[0].size()) + marginNum };

	//余白ありの大きさでマップデータを生成。まずは0で埋める
	mapChipNum.resize(mapSize.x);
	for (int x = 0; x < mapSize.x; x++) {
		mapChipNum[x].resize(mapSize.y);
		for (int y = 0; y < mapSize.y; y++) {
			mapChipNum[x][y].resize(mapSize.z);
		}
	}

	//余白ではない部分の要素番号に読み込んだマップデータ情報の番号を割り当てていく
	for (int x = 0; x < mapSize.x - marginNum; x++) {
		for (int y = 0; y < mapSize.y - marginNum; y++) {
			for (int z = 0; z < mapSize.z - marginNum; z++) {
				mapChipNum[x + 1][y + 1][z + 1] = inputmap[y][z][x];
			}
		}
	}

	//マップの中心をずらす値をセット
	const float blockSize = Block::GetBlockSize();
	shiftPos = { blockSize / 2 * (mapSize.x - 1), blockSize / 2 * (mapSize.y - 1), blockSize / 2 * (mapSize.z - 1) };

	return true;
}
