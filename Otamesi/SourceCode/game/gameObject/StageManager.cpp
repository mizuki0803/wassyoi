#include "StageManager.h"
#include <cassert>

int StageManager::selectStage = 1;
std::array<bool, StageManager::stageNum> StageManager::isClear;
bool StageManager::isAllStageClear = false;

bool StageManager::GetIsClear(const int selectStage)
{
	//存在しないステージ番号の場合は警告を出す
	if (selectStage < 0 || selectStage > stageNum - 1) {
		assert(0);
	}

	return isClear[selectStage];
}

void StageManager::StageClear()
{
	isClear[selectStage] = true;
}

void StageManager::SetSelectStage(const int selectStage)
{
	//存在しないステージ番号の場合は警告を出す
	if (selectStage < 0 || selectStage > stageNum - 1) {
		assert(0);
	}

	//引数のステージ番号を選択中にする
	StageManager::selectStage = selectStage;
}

bool StageManager::NextStageSelect()
{
	//現在選択中のステージを次に移動
	selectStage++;

	//最大ステージ数を超えないようにする
	if (selectStage > stageNum - 1) {
		selectStage = stageNum - 1;
		return false;
	}

	return true;
}

bool StageManager::PrevStageSelect()
{
	//現在選択中のステージを次に移動
	selectStage--;

	//ステージ1より下にならないようにする
	if (selectStage < 1) {
		selectStage = 1;
		return false;
	}

	return true;
}

bool StageManager::AllStageClearTriggerCheck()
{
	//既にすべてのステージをクリアした状態なら抜ける
	if (isAllStageClear) { return false; }

	//全てのステージをクリアしたかチェック
	for (int i = 1; i <= stageNum - 1; i++) {
		if (!(GetIsClear(i))) { return false; }
	}

	//初めて全てのステージをクリアしたときのみ特別なステージに進める
	selectStage = 100;
	//全てのステージをクリアした状態にする
	isAllStageClear = true;

	return true;
}
