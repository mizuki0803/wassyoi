#include "MapBlockData.h"

bool MapBlockData::MapChipNumBlockCheck(const int mapChipNum)
{
    //�}�b�v�ԍ����u�S�[���v�ł����true��Ԃ�
    if (mapChipNum == (int)BlockType::Goal) { return true; }
    //�}�b�v�ԍ����u�u���b�N�v�ł����true��Ԃ�
    if (mapChipNum == (int)BlockType::Block) { return true; }

    //�S�Ēʉ߂ł��Ȃ����false��Ԃ�
    return false;
}
