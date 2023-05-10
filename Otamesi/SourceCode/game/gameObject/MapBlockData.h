#pragma once

/// <summary>
/// マップブロックデータ
/// </summary>
class MapBlockData
{
public: //enum
	//ブロックの種類
	enum BlockType
	{
		None,	//なにもなし
		Player,	//プレイヤー
		Goal,	//ゴール
		Block,	//ブロック
	};

public: //静的メンバ関数
	/// <summary>
	/// 指定したマップ番号がブロックであるかチェック
	/// </summary>
	/// <param name="mapChipNum">マップ番号</param>
	/// <returns>成否</returns>
	static bool MapChipNumBlockCheck(const int mapChipNum);
};