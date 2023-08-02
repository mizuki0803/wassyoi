#include "MapDataStageSelectManager.h"
#include "Input.h"
#include "StageManager.h"
#include "Audio.h"

MapDataStageSelectManager* MapDataStageSelectManager::Create()
{
	//インスタンス生成
	MapDataStageSelectManager* instance = new MapDataStageSelectManager();

	//初期化処理
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool MapDataStageSelectManager::Initialize()
{
	//現在選択中のステージのマップを生成する
	std::unique_ptr<MapDataStageSelect> newMapData;
	newMapData.reset(MapDataStageSelect::Create(StageManager::GetSelectStage(), StageSelectBlockManager::BlockManagerPositionPhase::Center));
	mapDatas.push_back(std::move(newMapData));

	//ステージ番号表示用UI生成
	const Vector2 uiPos = { 450, 85 };
	const float uiSize = 1;
	stageNumberUI.reset(StageNumberUI::Create(uiPos, uiSize, StageManager::GetSelectStage()));

	return true;
}

void MapDataStageSelectManager::Update()
{
	//マップのブロック管理の削除フラグがtrueなら削除する
	mapDatas.remove_if([](std::unique_ptr<MapDataStageSelect>& mapData) {
		return mapData->GetStageSelectBlockManager()->GetIsDelete();
		});

	//�X�e�[�W��ύX���Ă��Ȃ���Ԃɖ߂�
	if (isChangeStage) {
		isChangeStage = false;
	}


	//選択するステージを変更する
	ChangeStage();

	//マップ更新
	for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
		mapData->Update();
	}

	//ステージ番号表示用UI更新
	stageNumberUI->Update();
}

void MapDataStageSelectManager::Draw()
{
	//描画
	for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
		mapData->Draw();
	}
}

void MapDataStageSelectManager::DrawUI()
{
	//ステージ番号表示用UI描画
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

	//変更するキー入力をしていなければ抜ける
	if (!(Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D) ||
		Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT))) { return; }

	//既に移動中であれば抜ける
	for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
		if (mapData->GetStageSelectBlockManager()->GetIsMove()) { return; }
	}

	//次に選択するステージのマップを生成する
	std::unique_ptr<MapDataStageSelect> newMapData;

	//右キーを押した場合は次のステージを選択する
	if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_RIGHT)) {
		if (!StageManager::NextStageSelect()) { return; } //ステージ番号にエラーが生じたら抜ける

		//画面外(右)に次のステージを生成する
		newMapData.reset(MapDataStageSelect::Create(StageManager::GetSelectStage(), StageSelectBlockManager::BlockManagerPositionPhase::OutscreenRight));
		//現在選択中のステージを左に移動させる
		for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
			mapData->GetStageSelectBlockManager()->MoveStart(StageSelectBlockManager::BlockManagerPositionPhase::OutscreenLeft);
		}
	}
	//左キーを押した場合は前のステージを選択する
	else if (Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_LEFT)) {
		if (!StageManager::PrevStageSelect()) { return; } //ステージ番号にエラーが生じたら抜ける

		//画面外(左)に次のステージを生成する
		newMapData.reset(MapDataStageSelect::Create(StageManager::GetSelectStage(), StageSelectBlockManager::BlockManagerPositionPhase::OutscreenLeft));
		//現在選択中のステージを右に移動させる
		for (const std::unique_ptr<MapDataStageSelect>& mapData : mapDatas) {
			mapData->GetStageSelectBlockManager()->MoveStart(StageSelectBlockManager::BlockManagerPositionPhase::OutscreenRight);
		}
	}	
	mapDatas.push_back(std::move(newMapData));

	//ステージ番号の更新
	stageNumberUI->ChengeStageNum(StageManager::GetSelectStage());

	//�X�e�[�W��ύX����
	isChangeStage = true;

	//se再生
	Audio::GetInstance()->PlayWave(Audio::SoundName::stage_change);
}
