#include "MapData.h"
#include "Block.h"
#include "JsonLoader.h"
#include <sstream>

bool MapData::CreateMap(const int selectStageNum)
{
	//json�}�b�v�f�[�^�ǂݍ���
	std::ostringstream  fileName;
	fileName << selectStageNum;
	if (!LoadMapData(fileName.str())) {
		return false;
	}
	//�}�b�v�u���b�N����
	CreateMapBlock();

	Block::StaticInitialize();

	return true;
}

bool MapData::LoadMapData(const std::string& fileName)
{
	//�}�b�v�f�[�^���ǂݎ��
	std::vector<std::vector<std::vector<int>>> inputmap;

	std::array<float, 3> inCameraDist = {};
	if (!JsonLoader::DeserializeJsonMap("Resources/mapdata/" + fileName + ".json",
		&inCameraDist, &installationSurface, &inputmap)) {
		return false;
	}

	//�J���������ۑ�
	cameraDist = { inCameraDist[0],inCameraDist[1], inCameraDist[2] };

	//�}�b�v�̃T�C�Y���擾
	const int marginNum = 2; //�Q�[���̎d�l��A�㉺���E�O���1�}�X�̗]�������
	mapSize = { int(inputmap[0][0].size()) + marginNum, int(inputmap.size()) + marginNum, int(inputmap[0].size()) + marginNum };

	//�]������̑傫���Ń}�b�v�f�[�^�𐶐��B�܂���0�Ŗ��߂�
	mapChipNum.resize(mapSize.x);
	for (int x = 0; x < mapSize.x; x++) {
		mapChipNum[x].resize(mapSize.y);
		for (int y = 0; y < mapSize.y; y++) {
			mapChipNum[x][y].resize(mapSize.z);
		}
	}

	//�]���ł͂Ȃ������̗v�f�ԍ��ɓǂݍ��񂾃}�b�v�f�[�^���̔ԍ������蓖�ĂĂ���
	for (int x = 0; x < mapSize.x - marginNum; x++) {
		for (int y = 0; y < mapSize.y - marginNum; y++) {
			for (int z = 0; z < mapSize.z - marginNum; z++) {
				mapChipNum[x + 1][y + 1][z + 1] = inputmap[y][z][x];
			}
		}
	}

	//�}�b�v�̒��S�����炷�l���Z�b�g
	const float blockSize = Block::GetBlockSize();
	shiftPos = { blockSize / 2 * (mapSize.x - 1), blockSize / 2 * (mapSize.y - 1), blockSize / 2 * (mapSize.z - 1) };

	return true;
}
