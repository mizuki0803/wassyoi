#include "PlayerActionManager.h"
#include "Input.h"
#include "MapBlockData.h"

std::vector<std::vector<std::vector<int>>> PlayerActionManager::mapChipNum;

// XMINT3同士の加算処理
const DirectX::XMINT3 operator+(const DirectX::XMINT3& lhs, const DirectX::XMINT3& rhs)
{
	XMINT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

bool PlayerActionManager::PlayerMoveCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//判定用にプレイヤー位置を表すマップ番号を仮変数に渡す
	XMINT3 judgeNumberPlayerPos = mapChipNumberPlayerPos;

	//全ての接地面に対する移動方向を設定 (キー入力に応じてプレイヤー位置を表すマップ番号を移動 なおカメラの向きにより移動方向を変更させる)
	judgeNumberPlayerPos = judgeNumberPlayerPos + PlayerMoveDirection3D(moveSurfacePhase, cameraXPosPhase, cameraYPosPhase);

	//存在するマップ番号内でなければ抜ける
	if (!PlayerMoveMapChipNumWithinRangeCheck(judgeNumberPlayerPos)) { return false; }
	//ブロックの配置が移動可能な状況でないなら抜ける
	if (!PlayerMoveBlockCheck3D(judgeNumberPlayerPos, moveSurfacePhase)) { return false; }
	//移動するブロック間にハリボテが存在したら通過できずに抜ける
	if (!PlayerMovePlaneCheck3D(mapChipNumberPlayerPos, judgeNumberPlayerPos)) { return false; }

	//移動可能なら判定用変数で変更した値をプレイヤー位置を表すマップ番号にセット
	mapChipNumberPlayerPos = judgeNumberPlayerPos;

	// 移動先のブロックをアクション

	return true;
}

bool PlayerActionManager::PlayerMoveCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//判定用にプレイヤー位置を表すマップ番号を仮変数に渡す
	XMINT3 judgeNumberPlayerPos = mapChipNumberPlayerPos;

	//全ての接地面に対する移動方向を設定 (キー入力に応じてプレイヤー位置を表すマップ番号を移動 なおカメラの向きにより移動方向を変更させる)
	judgeNumberPlayerPos = judgeNumberPlayerPos + PlayerMoveDirection2D(moveSurfacePhase, cameraXPosPhase, cameraYPosPhase);

	//存在するマップ番号内でなければ抜ける
	if (!PlayerMoveMapChipNumWithinRangeCheck(judgeNumberPlayerPos)) { return false; }
	//ブロックの配置が移動可能な状況でないなら抜ける
	if (!PlayerMoveBlockCheck2D(judgeNumberPlayerPos, moveSurfacePhase)) { return false; }

	//移動可能なら判定用変数で変更した値をプレイヤー位置を表すマップ番号にセット
	mapChipNumberPlayerPos = judgeNumberPlayerPos;

	return true;
}

void PlayerActionManager::PlayerFrontmost2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//プレイヤーより画面手前にブロックが存在しなければプレイヤーを画面の一番手前に配置
	//接地面が「上」の場合はプレイヤーのマップ番号より「上にブロック」があるかチェック
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		mapChipNumberPlayerPos.y = (int)mapChipNum[mapChipNumberPlayerPos.x].size() - 1;
	}
	//接地面が「下」の場合はプレイヤーのマップ番号より「下にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		mapChipNumberPlayerPos.y = 0;
	}
	//接地面が「左」の場合はプレイヤーのマップ番号より「右にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		mapChipNumberPlayerPos.x = 0;
	}
	//接地面が「右」の場合はプレイヤーのマップ番号より「左にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		mapChipNumberPlayerPos.x = (int)mapChipNum.size() - 1;
	}
	//接地面が「手前」の場合はプレイヤーのマップ番号より「奥にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		mapChipNumberPlayerPos.z = 0;
	}
	//接地面が「奥」の場合はプレイヤーのマップ番号より「手前にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		mapChipNumberPlayerPos.z = (int)mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size() - 1;
	}
}

bool PlayerActionManager::DirectionForwardBlockCheck(const XMINT3& mapChipNumberPlayerPos, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//ブロックより先に裏面のハリボテが検出されたらfalseを返す

	//カメラY軸が「上」の場合はプレイヤーのマップ番号より「上にブロック」があるかチェック
	if (cameraYPosPhase == GameCamera::CameraYPosPhase::Top) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::UpPlane) ||
				mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::DownPlane) {
				return true;
			}
		}
	}
	//カメラY軸が「下」の場合はプレイヤーのマップ番号より「下にブロック」があるかチェック
	else if (cameraYPosPhase == GameCamera::CameraYPosPhase::Buttom) {
		for (int i = mapChipNumberPlayerPos.y; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::DownPlane) ||
				mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::UpPlane) {
				return true;
			}
		}
	}
	//カメラX軸が「左」の場合はプレイヤーのマップ番号より「左にブロック」があるかチェック
	else if (cameraXPosPhase == GameCamera::CameraXPosPhase::Left) {
		for (int i = mapChipNumberPlayerPos.x; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::LeftPlane) ||
				mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::RightPlane) {
				return true;
			}
		}
	}
	//カメラX軸が「右」の場合はプレイヤーのマップ番号より「右にブロック」があるかチェック
	else if (cameraXPosPhase == GameCamera::CameraXPosPhase::Right) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::RightPlane) ||
				mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::LeftPlane) {
				return true;
			}
		}
	}
	//カメラX軸が「手前」の場合はプレイヤーのマップ番号より「手前にブロック」があるかチェック
	else if (cameraXPosPhase == GameCamera::CameraXPosPhase::Front) {
		for (int i = mapChipNumberPlayerPos.z; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::ForwardPlane) ||
				mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::AwayPlane) {
				return true;
			}
		}
	}
	//カメラX軸が「奥」の場合はプレイヤーのマップ番号より「奥にブロック」があるかチェック
	else if (cameraXPosPhase == GameCamera::CameraXPosPhase::Back) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::AwayPlane) ||
				mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::ForwardPlane) {
				return true;
			}
		}
	}

	//全てのチェックを通過できなければfalseを返す
	return false;
}

bool PlayerActionManager::DirectionAwayBlockCheck(const XMINT3& mapChipNumberPlayerPos, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//ブロックより先に裏面のハリボテが検出されたらfalseを返す

	//カメラY軸が「上」の場合はプレイヤーのマップ番号より「下にブロック」があるかチェック
	if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
		for (int i = mapChipNumberPlayerPos.y; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::UpPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::DownPlane) {
				return false;
			}
		}
	}
	//カメラY軸が「下」の場合はプレイヤーのマップ番号より「上にブロック」があるかチェック
	else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::DownPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::UpPlane) {
				return false;
			}
		}
	}
	//カメラX軸が「左」の場合はプレイヤーのマップ番号より「右にブロック」があるかチェック
	else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::LeftPlane)) {
				return true;
			}
			else if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::RightPlane) {
				return false;
			}
		}
	}
	//カメラX軸が「右」の場合はプレイヤーのマップ番号より「左にブロック」があるかチェック
	else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
		for (int i = mapChipNumberPlayerPos.x; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::RightPlane)) {
				return true;
			}
			else if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::LeftPlane) {
				return false;
			}
		}
	}
	//カメラX軸が「手前」の場合はプレイヤーのマップ番号より「奥にブロック」があるかチェック
	else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::ForwardPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::AwayPlane) {
				return false;
			}
		}
	}
	//カメラX軸が「奥」の場合はプレイヤーのマップ番号より「手前にブロック」があるかチェック
	else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
		for (int i = mapChipNumberPlayerPos.z; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::AwayPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::ForwardPlane) {
				return false;
			}
		}
	}

	//全てのチェックを通過できなければfalseを返す
	return false;
}

void PlayerActionManager::PlayerScaffoldReturn3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//接地面が「上」の場合はプレイヤーのマップ番号より「下にブロック」があるかチェック
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		for (int i = mapChipNumberPlayerPos.y; i >= 0; --i) {
			//ブロックでなければ飛ばす
			if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::UpPlane))) { continue; }

			//最初に見つかったブロックに接地する番号にプレイヤーをセットする
			mapChipNumberPlayerPos.y = i + 1;

			return;
		}
	}
	//接地面が「下」の場合はプレイヤーのマップ番号より「上にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			//ブロックでなければ飛ばす
			if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::DownPlane))) { continue; }

			//最初に見つかったブロックに接地する番号にプレイヤーをセットする
			mapChipNumberPlayerPos.y = i - 1;

			return;
		}
	}
	//接地面が「左」の場合はプレイヤーのマップ番号より「右にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			//ブロックでなければ飛ばす
			if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::LeftPlane))) { continue; }

			//最初に見つかったブロックに接地する番号にプレイヤーをセットする
			mapChipNumberPlayerPos.x = i - 1;

			return;
		}
	}
	//接地面が「右」の場合はプレイヤーのマップ番号より「左にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = mapChipNumberPlayerPos.x; i >= 0; --i) {
			//ブロックでなければ飛ばす
			if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::RightPlane))) { continue; }

			//最初に見つかったブロックに接地する番号にプレイヤーをセットする
			mapChipNumberPlayerPos.x = i + 1;

			return;
		}
	}
	//接地面が「手前」の場合はプレイヤーのマップ番号より「奥にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			//ブロックでなければ飛ばす
			if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::ForwardPlane))) { continue; }

			//最初に見つかったブロックに接地する番号にプレイヤーをセットする
			mapChipNumberPlayerPos.z = i - 1;

			return;
		}
	}
	//接地面が「奥」の場合はプレイヤーのマップ番号より「手前にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = mapChipNumberPlayerPos.z; i >= 0; --i) {
			//ブロックでなければ飛ばす
			if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::AwayPlane))) { continue; }

			//最初に見つかったブロックに接地する番号にプレイヤーをセットする
			mapChipNumberPlayerPos.z = i + 1;

			return;
		}
	}
}

bool PlayerActionManager::PlayerGoalCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//足場となるブロックがゴールならばtrueを返す
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y - 1][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y + 1][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		if (mapChipNum[mapChipNumberPlayerPos.x + 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		if (mapChipNum[mapChipNumberPlayerPos.x - 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z + 1] == MapBlockData::MapBlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z - 1] == MapBlockData::MapBlockType::Goal) {
			return true;
		}
	}

	//全てのチェックをクリアできなければfalseを返す
	return false;
}

bool PlayerActionManager::PlayerGoalCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//プレイヤーのマップ番号の下にあるブロックをチェックし、
	//一番最初に発見したブロックがゴールであればtrueをし、
	//それ以外のブロックならばfalseを返す

	//接地面が「上」の場合はプレイヤーのマップ番号より「下にブロック」があるかチェック
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		for (int i = mapChipNumberPlayerPos.y - 1; i >= 0; --i) {
			if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::UpPlane)) {
				return false;
			}
		}
	}
	//接地面が「下」の場合はプレイヤーのマップ番号より「上にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::DownPlane)) {
				return false;
			}
		}
	}
	//接地面が「左」の場合はプレイヤーのマップ番号より「右にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::LeftPlane)) {
				return false;
			}
		}
	}
	//接地面が「右」の場合はプレイヤーのマップ番号より「左にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = mapChipNumberPlayerPos.x - 1; i >= 0; --i) {
			if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::RightPlane)) {
				return false;
			}
		}
	}
	//接地面が「手前」の場合はプレイヤーのマップ番号より「奥にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::ForwardPlane)) {
				return false;
			}
		}
	}
	//接地面が「奥」の場合はプレイヤーのマップ番号より「手前にブロック」があるかチェック
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = mapChipNumberPlayerPos.z - 1; i >= 0; --i) {
			if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::AwayPlane)) {
				return false;
			}
		}
	}

	//全てのチェックを通過できなければfalseを返す
	return false;
}

XMINT3 PlayerActionManager::PlayerMoveDirection3D(const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//返り値になる値
	XMINT3 moveNum{};

	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Top) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y--; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Side) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.y++; }
			}
			else if (cameraYPosPhase == (int)GameCamera::CameraYPosPhase::ReverseSide) {
				if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y--; }
				else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y++; }
				else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
				else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
			}
		}
	}

	//移動させた値を返す
	return moveNum;
}

XMINT3 PlayerActionManager::PlayerMoveDirection2D(const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//返り値になる値
	XMINT3 moveNum{};

	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.z++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.z--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.x--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.x++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z--; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z++; }
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.z++; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.z--; }
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x++; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x--; }
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		if (Input::GetInstance()->PushKey(DIK_W)) { moveNum.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { moveNum.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { moveNum.x--; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { moveNum.x++; }
	}

	//縦軸カメラ位置フェーズが逆さの横ならば左右が逆になるので反転させておく
	if (cameraYPosPhase == GameCamera::CameraYPosPhase::ReverseSide) {
		moveNum.x = -moveNum.x;
		moveNum.y = -moveNum.y;
		moveNum.z = -moveNum.z;
	}

	//移動させた値を返す
	return moveNum;
}

bool PlayerActionManager::PlayerMoveBlockCheck3D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//移動先のマップ番号にブロックが存在すれば移動できずに抜ける(裏向きハリボテも踏めないので不可)
	//足場となるブロックが存在しなければ抜ける

	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::DownPlane)) {
			return false;
		}
		else if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y - 1][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::UpPlane))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::UpPlane)) {
			return false;
		}
		else if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y + 1][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::DownPlane))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::RightPlane)) {
			return false;
		}
		else if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x + 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::LeftPlane))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::LeftPlane)) {
			return false;
		}
		else if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x - 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::RightPlane))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::AwayPlane)) {
			return false;
		}
		else if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z + 1], MapBlockData::MapBlockType::ForwardPlane))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::ForwardPlane)) {
			return false;
		}
		else if (!(MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z - 1], MapBlockData::MapBlockType::AwayPlane))) {
			return false;
		}
	}

	//全てのチェックを通過したらtrueを返す
	return true;
}

bool PlayerActionManager::PlayerMoveBlockCheck2D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//2D表現をして、移動先の軸にブロックが存在していれば通過(true)
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		for (int i = (int)mapChipNum[mapChipNumberPlayerPos.x].size() - 1; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::UpPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::DownPlane) {
				return false;
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = 0; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::DownPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::UpPlane) {
				return false;
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		for (int i = 0; i < mapChipNum.size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::LeftPlane)) {
				return true;
			}
			else if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::RightPlane) {
				return false;
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = (int)mapChipNum.size() - 1; i >= 0; i--) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z], MapBlockData::MapBlockType::RightPlane)) {
				return true;
			}
			else if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::MapBlockType::LeftPlane) {
				return false;
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		for (int i = 0; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::ForwardPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::AwayPlane) {
				return false;
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = (int)mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size() - 1; i >= 0; i--) {
			if (MapBlockData::MapChipNumBlockPlaneCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i], MapBlockData::MapBlockType::AwayPlane)) {
				return true;
			}
			else if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::MapBlockType::ForwardPlane) {
				return false;
			}
		}
	}

	//全てのチェックを通過できなければfalseを返す
	return false;
}

bool PlayerActionManager::PlayerMovePlaneCheck3D(const XMINT3& mapChipNumberPlayerBeforePos, const XMINT3& mapChipNumberPlayerAfterPos)
{
	//移動前と移動後を比べて、移動方向を算出
	const XMINT3 moveDirection = { mapChipNumberPlayerAfterPos.x - mapChipNumberPlayerBeforePos.x,
		mapChipNumberPlayerAfterPos.y - mapChipNumberPlayerBeforePos.y,
		mapChipNumberPlayerAfterPos.z - mapChipNumberPlayerBeforePos.z
	};

	//移動方向する方向にハリボテが存在するかチェック
	//1.移動前のマップ位置の要素番号が、壁になるように設置されているハリボテならfalse
	//2.移動後のマップ位置の要素番号が、壁になるように設置されているハリボテならfalse

	const int moveNum = 1;	//一度の移動で動けるブロックの量は1
	if (moveDirection.y == moveNum) { //移動方向が上
		if (mapChipNum[mapChipNumberPlayerBeforePos.x][mapChipNumberPlayerBeforePos.y][mapChipNumberPlayerBeforePos.z] == MapBlockData::MapBlockType::UpPlane) { return false; }
		else if (mapChipNum[mapChipNumberPlayerAfterPos.x][mapChipNumberPlayerAfterPos.y][mapChipNumberPlayerAfterPos.z] == MapBlockData::MapBlockType::DownPlane) { return false; }
	}
	else if (moveDirection.y == -moveNum) { //移動方向が下
		if (mapChipNum[mapChipNumberPlayerBeforePos.x][mapChipNumberPlayerBeforePos.y][mapChipNumberPlayerBeforePos.z] == MapBlockData::MapBlockType::DownPlane) { return false; }
		else if (mapChipNum[mapChipNumberPlayerAfterPos.x][mapChipNumberPlayerAfterPos.y][mapChipNumberPlayerAfterPos.z] == MapBlockData::MapBlockType::UpPlane) { return false; }
	}
	else if (moveDirection.x == -moveNum) { //移動方向が左
		if (mapChipNum[mapChipNumberPlayerBeforePos.x][mapChipNumberPlayerBeforePos.y][mapChipNumberPlayerBeforePos.z] == MapBlockData::MapBlockType::LeftPlane) { return false; }
		else if (mapChipNum[mapChipNumberPlayerAfterPos.x][mapChipNumberPlayerAfterPos.y][mapChipNumberPlayerAfterPos.z] == MapBlockData::MapBlockType::RightPlane) { return false; }
	}
	else if (moveDirection.x == moveNum) { //移動方向が右
		if (mapChipNum[mapChipNumberPlayerBeforePos.x][mapChipNumberPlayerBeforePos.y][mapChipNumberPlayerBeforePos.z] == MapBlockData::MapBlockType::RightPlane) { return false; }
		else if (mapChipNum[mapChipNumberPlayerAfterPos.x][mapChipNumberPlayerAfterPos.y][mapChipNumberPlayerAfterPos.z] == MapBlockData::MapBlockType::LeftPlane) { return false; }
	}
	else if (moveDirection.z == -moveNum) { //移動方向が手前
		if (mapChipNum[mapChipNumberPlayerBeforePos.x][mapChipNumberPlayerBeforePos.y][mapChipNumberPlayerBeforePos.z] == MapBlockData::MapBlockType::ForwardPlane) { return false; }
		else if (mapChipNum[mapChipNumberPlayerAfterPos.x][mapChipNumberPlayerAfterPos.y][mapChipNumberPlayerAfterPos.z] == MapBlockData::MapBlockType::AwayPlane) { return false; }
	}
	else if (moveDirection.z == moveNum) { //移動方向が奥
		if (mapChipNum[mapChipNumberPlayerBeforePos.x][mapChipNumberPlayerBeforePos.y][mapChipNumberPlayerBeforePos.z] == MapBlockData::MapBlockType::AwayPlane) { return false; }
		else if (mapChipNum[mapChipNumberPlayerAfterPos.x][mapChipNumberPlayerAfterPos.y][mapChipNumberPlayerAfterPos.z] == MapBlockData::MapBlockType::ForwardPlane) { return false; }
	}

	//全てのチェックを通過できればtrueを返す
	return true;
}

bool PlayerActionManager::PlayerMoveMapChipNumWithinRangeCheck(const XMINT3& mapChipNumberPlayerPos)
{
	//x,y,z軸全てが、マップ番号範囲内でなければ抜ける
	if (!(mapChipNumberPlayerPos.x >= 0 && mapChipNumberPlayerPos.x < mapChipNum.size())) { return false; }
	if (!(mapChipNumberPlayerPos.y >= 0 && mapChipNumberPlayerPos.y < mapChipNum[mapChipNumberPlayerPos.x].size())) { return false; }
	if (!(mapChipNumberPlayerPos.z >= 0 && mapChipNumberPlayerPos.z < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size())) { return false; }

	//全てのチェックを通過したらtrueを返す
	return true;
}
