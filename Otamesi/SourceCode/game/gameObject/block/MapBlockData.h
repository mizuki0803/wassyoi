#pragma once

/// <summary>
/// マップブロックデータ
/// </summary>
class MapBlockData
{
public: //enum
	//マップ用ブロックの種類
	enum MapBlockType
	{
		None,			//なにもなし
		Player,			//プレイヤー
		Goal,			//ゴール
		Block,			//ブロック
		UpPlane,		//上向きハリボテ
		DownPlane,		//下向きハリボテ
		LeftPlane,		//左向きハリボテ
		RightPlane,		//右向きハリボテ
		ForwardPlane,	//手前向きハリボテ
		AwayPlane,		//奥向きハリボテ
	};

public: //静的メンバ関数
	/// <summary>
	/// 指定したマップ番号がブロックであるかチェック
	/// </summary>
	/// <param name="mapChipNum">マップ番号</param>
	/// <returns>成否</returns>
	static bool MapChipNumBlockCheck(const int mapChipNum);

	/// <summary>
	/// 指定したマップ番号がブロックまたはその面に適したハリボテであるかチェック
	/// </summary>
	/// <param name="mapChipNum">マップ番号</param>
	/// <returns>成否</returns>
	static bool MapChipNumBlockPlaneCheck(const int mapChipNum, MapBlockType planeType);
};