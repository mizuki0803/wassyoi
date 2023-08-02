#pragma once
#include "bird.h"
#include <list>

/// <summary>
/// 鳥管理
/// </summary>
class BirdManager
{
	/// <summary>
	/// 飛行を行う場所
	/// </summary>
	enum FlyPosition
	{
		Forward,
		Away,
		Left,
		Right,

		PosNum,
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="bodyModel">胴体モデル</param>
	/// <param name="wingModel">翼モデル</param>
	/// <param name="createInterval">生成間隔</param>
	/// <returns>鳥管理</returns>
	static BirdManager* Create(ObjModel* bodyModel, ObjModel* wingModel, const int32_t& createInterval);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: //メンバ関数
	/// <summary>
	/// 鳥生成
	/// </summary>
	void CreateBird();
	
private: //メンバ変数
	//鳥
	std::list<std::unique_ptr<Bird>> birds;
	//胴体モデル
	ObjModel* bodyModel;
	//翼モデル
	ObjModel* wingModel;
	//生成タイマー
	int32_t createTimer;
	//生成間隔
	int32_t createInterval;
};