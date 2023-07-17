#include "MapDataStageSelectManager.h"
#include "Input.h"
#include "StageManager.h"

MapDataStageSelectManager* MapDataStageSelectManager::Create()
{
	//�C���X�^���X����
	MapDataStageSelectManager* instance = new MapDataStageSelectManager();

	//����������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool MapDataStageSelectManager::Initialize()
{
	//���ݑI�𒆂̃X�e�[�W�̃}�b�v�𐶐�����
	std::unique_ptr<MapDataStageSelect> newMapData;
	newMapData.reset(MapDataStageSelect::Create(StageManager::GetSelectStage(), StageSelectBlockManager::BlockManagerPositionPhase::Center));
	mapDatas.push_back(std::move(newMapData));

	//�X�e�[�W�ԍ��\���pUI����
	const Vector2 uiPos = { 450, 85 };
	const float uiSize = 1;
	stageNumberUI.reset(StageNumberUI::Create(uiPos, uiSize, StageManager::GetSelectStage()));

	return true;
}

void MapDataStageSelectManager::Update()
{
	//�}�b�v�̃u���b�N�Ǘ��̍폜�t���O��true�Ȃ�폜����
	mapDatas.remove_if([](std::unique_ptr<MapDataStageSelect>& mapData) {
		return mapData->GetStageSelectBlockManager()->GetIsDelete();
		});


	//�I������X�e�[�W��ύX����
	ChangeStage();

	//�}�b�v�X�V
	for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
		mapData->Update();
	}

	//�X�e�[�W�ԍ��\���pUI�X�V
	stageNumberUI->Update();
}

void MapDataStageSelectManager::Draw()
{
	//�`��
	for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
		mapData->Draw();
	}
}

void MapDataStageSelectManager::DrawUI()
{
	//�X�e�[�W�ԍ��\���pUI�`��
	stageNumberUI->Draw();
}

void MapDataStageSelectManager::FrameReset()
{
	for (auto& i : mapDatas) {
		i->FrameReset();
}
}

void MapDataStageSelectManager::ChangeStage()
{
	if (menuFlag_) { return; }

	//�ύX����L�[���͂����Ă��Ȃ���Δ�����
	if (!(Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D) ||
		Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))) { return; }

	//���Ɉړ����ł���Δ�����
	for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
		if (mapData->GetStageSelectBlockManager()->GetIsMove()) { return; }
	}

	//���ɑI������X�e�[�W�̃}�b�v�𐶐�����
	std::unique_ptr<MapDataStageSelect> newMapData;

	//�E�L�[���������ꍇ�͎��̃X�e�[�W��I������
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_RIGHT)) {
		if (!StageManager::NextStageSelect()) { return; } //�X�e�[�W�ԍ��ɃG���[���������甲����

		//��ʊO(�E)�Ɏ��̃X�e�[�W�𐶐�����
		newMapData.reset(MapDataStageSelect::Create(StageManager::GetSelectStage(), StageSelectBlockManager::BlockManagerPositionPhase::OutscreenRight));
		//���ݑI�𒆂̃X�e�[�W�����Ɉړ�������
		for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
			mapData->GetStageSelectBlockManager()->MoveStart(StageSelectBlockManager::BlockManagerPositionPhase::OutscreenLeft);
		}
	}
	//���L�[���������ꍇ�͑O�̃X�e�[�W��I������
	else if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		if (!StageManager::PrevStageSelect()) { return; } //�X�e�[�W�ԍ��ɃG���[���������甲����

		//��ʊO(��)�Ɏ��̃X�e�[�W�𐶐�����
		newMapData.reset(MapDataStageSelect::Create(StageManager::GetSelectStage(), StageSelectBlockManager::BlockManagerPositionPhase::OutscreenLeft));
		//���ݑI�𒆂̃X�e�[�W���E�Ɉړ�������
		for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
			mapData->GetStageSelectBlockManager()->MoveStart(StageSelectBlockManager::BlockManagerPositionPhase::OutscreenRight);
		}
	}	
	mapDatas.push_back(std::move(newMapData));

	//�X�e�[�W�ԍ��̍X�V
	stageNumberUI->ChengeStageNum(StageManager::GetSelectStage());
}
