#pragma once
#include "InstanceObject.h"

class BackGround
{
private:

	enum class transform {
		translation,
		rotation,
		scaling,
		size,
	};

	enum class moveStep {
		up,
		down,
		end,
		non
	};

	struct clearMoveInfo {
		int easingNum;//開始番号
		moveStep step;//状態
		float move;//移動距離
		float timer;//時間
	};

	struct playMoveInfo {
		float beforeMove;//前の位置
		float afterMove;//後の距離
		float move;//現在地
		float timer;//時間
	};

public:

	/// <summary>
	/// 生成
	/// </summary>
	/// <returns></returns>
	static BackGround* Create();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 影用光源ライトから見た視点での描画
	/// </summary>
	void DrawLightCameraView();

	/// <summary>
	/// frameごとの初期化
	/// </summary>
	void FrameReset();
	
	/// <summary>
	/// クリアフラグセット
	/// </summary>
	void SetStageClearFlag();

private:

	/// <summary>
	/// ゲーム中の移動セット
	/// </summary>
	void SetPlayMove();

	/// <summary>
	/// ゲーム中の移動
	/// </summary>
	void PlayMove();

	/// <summary>
	/// オブジェクトごとのセット
	/// </summary>
	void SetStageClear();

	/// <summary>
	/// ステージclear演出
	/// </summary>
	void StageClear();

private:

	//objモデル
	std::unique_ptr<ObjModel> model;

	//描画用クラス
	static const int instanceNum = 4;
	std::unique_ptr<InstanceObject> block[instanceNum];

	//オブジェクトの状態
	std::vector<std::array<DirectX::XMFLOAT3, int(transform::size)>> info;
	//プレイ時の移動処理
	std::vector<playMoveInfo> playMoveInfo;
	//移動オブジェクトをセットするか
	int setMoveTimer;
	//クリア時の移動処理用
	std::vector<clearMoveInfo> clearMoveInfo;
	//クリア時にオブジェクトが動き出しているか
	bool isClearMove;
	//clear処理フラグ
	bool isClear;
	//クリア時の現在のオブジェクト移動番号
	int objectMoveNum;

};

