#include "MapBlockData.h"

bool MapBlockData::MapChipNumBlockCheck(const int mapChipNum)
{
    //�}�b�v�ԍ����u�S�[���v�ł����true��Ԃ�
    if (mapChipNum == (int)MapBlockType::Goal) { return true; }
    //�}�b�v�ԍ����u�u���b�N�v�ł����true��Ԃ�
    if (mapChipNum == (int)MapBlockType::Block) { return true; }

    //�S�Ēʉ߂ł��Ȃ����false��Ԃ�
    return false;
}

bool MapBlockData::MapChipNumBlockPlaneCheck(const int mapChipNum, MapBlockType planeType)
{
    //�}�b�v�ԍ����u���b�N(�S�[��)�ł����true��Ԃ�
    if (MapChipNumBlockCheck(mapChipNum)) { return true; }
    //�}�b�v�ԍ��������Ɏg�p���Ă���n���{�e�^�C�v�Ȃ��true��Ԃ�
    if (mapChipNum == (int)planeType) { return true; }

    //�S�Ēʉ߂ł��Ȃ����false��Ԃ�
    return false;
}
