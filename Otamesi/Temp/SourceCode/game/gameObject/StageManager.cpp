#include "StageManager.h"
#include <cassert>

int StageManager::selectStage = 1;
std::array<bool, StageManager::stageNum> StageManager::isClear;

bool StageManager::GetIsClear(const int selectStage)
{
	//���݂��Ȃ��X�e�[�W�ԍ��̏ꍇ�͌x�����o��
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
	//���݂��Ȃ��X�e�[�W�ԍ��̏ꍇ�͌x�����o��
	if (selectStage < 0 || selectStage > stageNum - 1) {
		assert(0);
	}

	//�����̃X�e�[�W�ԍ���I�𒆂ɂ���
	StageManager::selectStage = selectStage;
}

bool StageManager::NextStageSelect()
{
	//���ݑI�𒆂̃X�e�[�W�����Ɉړ�
	selectStage++;

	//�ő�X�e�[�W���𒴂��Ȃ��悤�ɂ���
	if (selectStage > stageNum - 1) {
		selectStage = stageNum - 1;
		return false;
	}

	return true;
}

bool StageManager::PrevStageSelect()
{
	//���ݑI�𒆂̃X�e�[�W�����Ɉړ�
	selectStage--;

	//�X�e�[�W1��艺�ɂȂ�Ȃ��悤�ɂ���
	if (selectStage < 1) {
		selectStage = 1;
		return false;
	}

	return true;
}
