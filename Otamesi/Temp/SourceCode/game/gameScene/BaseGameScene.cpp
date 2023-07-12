#include "BaseGameScene.h"
#include "SceneManager.h"
#include "SceneChangeEffect.h"
#include "JsonLoader.h"
#include "GameCamera.h"
#include "Player.h"

void BaseGameScene::SceneChangeStart(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime, const std::string& nextSceneName)
{
	//既にシーン変更状態なら抜ける
	if (isSceneChange) { return; }

	//シーン変更にかかる時間をセット
	sceneChangeTime = deepenTime + waitTime;
	//シーン変更状態にする
	isSceneChange = true;
	//次のシーン名をセット
	this->nextSceneName = nextSceneName;

	//シーン変更用演出生成
	SceneChangeEffect::CreateSceneChangeEffect(color, deepenTime, waitTime, returnTime);
}

void BaseGameScene::SceneChangeMode()
{
	//シーン変更状態でなければ抜ける
	if (!isSceneChange) { return; }

	//シーン変更タイマーを更新
	sceneChangeTimer++;
	//タイマーが指定した時間以下なら抜ける
	if (sceneChangeTimer < sceneChangeTime) { return; }

	//シーン切り替え
	SceneManager::GetInstance()->ChangeScene(nextSceneName);
}

void BaseGameScene::DeleteBinary()
{
	const int deleteNum = deleteOrderMaxNum + 2;
	for (int i = 0; i < deleteNum; i++) {
		std::string name = "Resources/binary/" + std::to_string(i) + ".binary";
		remove(name.c_str());
	}
}

void BaseGameScene::KeepBinary(GameCamera& _camera, Player& _player)
{
	//初期状態をbinary保存
	Vector3 cameraPos = _camera.GetEye();
	Vector3 cameraRota = _camera.GetRotation();
	Vector3 playerPos = _player.GetPosition();
	XMINT3 mapChip = _player.GetMapChipNumberPos();

	JsonLoader::SerializeBinary(std::to_string(orderNum), _camera.GetIs2D(), _player.GetMoveSurfacePhase(),
		{ mapChip.x,mapChip.y,mapChip.z }, { cameraPos.x,cameraPos.y,cameraPos.z },
		{ _camera.GetCameraXPosPhase(),_camera.GetCameraYPosPhase() },
		{ cameraRota.x,cameraRota.y,cameraRota.z }, { playerPos.x,playerPos.y,playerPos.z });
}

void BaseGameScene::Undo(GameCamera* _camera, Player* _player)
{
	if (orderNum == 0) { return; }

	orderNum--;
	bool is2D = false;
	int moveSurface = 0;
	std::array<int, 3> mapChip{};
	std::array<float, 3> cameraPos{}, cameraRota{}, playerPos{};
	std::array<int, 2> cameraPosPhase{};
	JsonLoader::DeserializeBinary(std::to_string(orderNum), &is2D, &moveSurface, &mapChip, &cameraPos,
		&cameraPosPhase, &cameraRota, &playerPos);

	if (_camera->GetIs2D() != is2D) {
		_camera->SetIs2D(is2D);
		_camera->SetDirtyProjection(true);
	}
	_player->SetMoveSurfacePhase(moveSurface);
	_player->SetMapChipNumberPos({ mapChip[0],mapChip[1],mapChip[2] });
	_camera->SetEye({ cameraPos[0],cameraPos[1],cameraPos[2] });
	_camera->SetCameraXPosPhase(cameraPosPhase[0]);
	_camera->SetCameraYPosPhase(cameraPosPhase[1]);
	_camera->SetRotation({ cameraRota[0],cameraRota[1],cameraRota[2] });
	_player->SetPosition({ playerPos[0],playerPos[1],playerPos[2] });
}

void BaseGameScene::Redo(GameCamera* _camera, Player* _player)
{
	if (orderNum == orderMaxNum){return;}
	orderNum++;
	bool is2D = false;
	int moveSurface = 0;
	std::array<int, 3> mapChip{};
	std::array<float, 3> cameraPos{}, cameraRota{}, playerPos{};
	std::array<int, 2> cameraPosPhase{};
	JsonLoader::DeserializeBinary(std::to_string(orderNum), &is2D, &moveSurface, &mapChip, &cameraPos,
		&cameraPosPhase, &cameraRota, &playerPos);

	if (_camera->GetIs2D() != is2D) {
		_camera->SetIs2D(is2D);
		_camera->SetDirtyProjection(true);
	}
	_player->SetMoveSurfacePhase(moveSurface);
	_player->SetMapChipNumberPos({ mapChip[0],mapChip[1],mapChip[2] });
	_camera->SetEye({ cameraPos[0],cameraPos[1],cameraPos[2] });
	_camera->SetCameraXPosPhase(cameraPosPhase[0]);
	_camera->SetCameraYPosPhase(cameraPosPhase[1]);
	_camera->SetRotation({ cameraRota[0],cameraRota[1],cameraRota[2] });
	_player->SetPosition({ playerPos[0],playerPos[1],playerPos[2] });
}
