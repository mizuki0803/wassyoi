#include "MapBlockData.h"

bool MapBlockData::MapChipNumBlockCheck(const int mapChipNum)
{
    //マップ番号が「ゴール」であればtrueを返す
    if (mapChipNum == (int)BlockType::Goal) { return true; }
    //マップ番号が「ブロック」であればtrueを返す
    if (mapChipNum == (int)BlockType::Block) { return true; }

    //全て通過できなければfalseを返す
    return false;
}
