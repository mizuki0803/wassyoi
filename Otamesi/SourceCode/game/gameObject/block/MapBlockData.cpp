#include "MapBlockData.h"

bool MapBlockData::MapChipNumBlockCheck(const int mapChipNum)
{
    //マップ番号が「ゴール」であればtrueを返す
    if (mapChipNum == (int)MapBlockType::Goal) { return true; }
    //マップ番号が「ブロック」であればtrueを返す
    if (mapChipNum == (int)MapBlockType::Block) { return true; }

    //全て通過できなければfalseを返す
    return false;
}

bool MapBlockData::MapChipNumBlockPlaneCheck(const int mapChipNum, MapBlockType planeType)
{
    //マップ番号がブロック(ゴール)であればtrueを返す
    if (MapChipNumBlockCheck(mapChipNum)) { return true; }
    //マップ番号が引数に使用しているハリボテタイプならばtrueを返す
    if (mapChipNum == (int)planeType) { return true; }

    //全て通過できなければfalseを返す
    return false;
}
