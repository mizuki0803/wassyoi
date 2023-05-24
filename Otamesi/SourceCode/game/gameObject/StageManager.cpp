#include "StageManager.h"

StageManager::StageName StageManager::selectStage = StageManager::StageName::Stage1;
std::array<bool, StageManager::StageNum> StageManager::isClear;

bool StageManager::GetIsClear(const StageName stageName)
{
    return isClear[stageName];
}

void StageManager::StageClear(const StageName stageName)
{
    isClear[stageName] = true;
}
