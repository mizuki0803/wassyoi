#pragma once
#include "MapDataStageSelect.h"
#include "StageSelectBlockManager.h"
#include "StageNumberUI.h"

/// <summary>
/// ステージ選択シーン全ブロック関連の管理
/// </summary>
class MapDataStageSelectManager
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージ選択シーン全ブロック関連の管理</returns>
	static MapDataStageSelectManager* Create();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// frameごとの初期化
	/// </summary>
	void FrameReset();
	
	//getter
	bool GetIsChangeStage() { return isChangeStage; }

	//setter
	void SetNotMove(bool flag1) { menuFlag_ = flag1; }

private: //メンバ関数
	/// <summary>
	/// 選択するステージを変更する
	/// </summary>
	void ChangeStage();

private: //メンバ変数
	//1ステージ分のマップデータ群
	std::list<std::unique_ptr<MapDataStageSelect>> mapDatas;

	//ステージ番号表示用UI
	std::unique_ptr<StageNumberUI> stageNumberUI;


	//�X�e�[�W�ύX��������
	bool isChangeStage = false;
	bool menuFlag_ = false;
};