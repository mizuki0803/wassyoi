#include "PlayerActionManager.h"
#include "Input.h"
#include "MapBlockData.h"

std::vector<std::vector<std::vector<int>>> PlayerActionManager::mapChipNum;

bool PlayerActionManager::PlayerMoveCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//����p�Ƀv���C���[�ʒu��\���}�b�v�ԍ������ϐ��ɓn��
	XMINT3 judgeNumberPlayerPos = mapChipNumberPlayerPos;

	//�S�Ă̐ڒn�ʂɑ΂���ړ�������ݒ� (�L�[���͂ɉ����ăv���C���[�ʒu��\���}�b�v�ԍ����ړ� �Ȃ��J�����̌����ɂ��ړ�������ύX������)
	PlayerMoveDirection3D(judgeNumberPlayerPos, moveSurfacePhase, cameraXPosPhase, cameraYPosPhase);

	//���݂���}�b�v�ԍ����łȂ���Δ�����
	if (!PlayerMoveMapChipNumWithinRangeCheck(judgeNumberPlayerPos)) { return false; }
	//�u���b�N�̔z�u���ړ��\�ȏ󋵂łȂ��Ȃ甲����
	if (!PlayerMoveBlockCheck3D(judgeNumberPlayerPos, moveSurfacePhase)) { return false; }

	//�ړ��\�Ȃ画��p�ϐ��ŕύX�����l���v���C���[�ʒu��\���}�b�v�ԍ��ɃZ�b�g
	mapChipNumberPlayerPos = judgeNumberPlayerPos;

	return true;
}

bool PlayerActionManager::PlayerMoveCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase)
{
	//����p�Ƀv���C���[�ʒu��\���}�b�v�ԍ������ϐ��ɓn��
	XMINT3 judgeNumberPlayerPos = mapChipNumberPlayerPos;

	//�S�Ă̐ڒn�ʂɑ΂���ړ�������ݒ� (�L�[���͂ɉ����ăv���C���[�ʒu��\���}�b�v�ԍ����ړ� �Ȃ��J�����̌����ɂ��ړ�������ύX������)
	PlayerMoveDirection2D(judgeNumberPlayerPos, moveSurfacePhase, cameraXPosPhase);

	//���݂���}�b�v�ԍ����łȂ���Δ�����
	if (!PlayerMoveMapChipNumWithinRangeCheck(judgeNumberPlayerPos)) { return false; }
	//�u���b�N�̔z�u���ړ��\�ȏ󋵂łȂ��Ȃ甲����
	if (!PlayerMoveBlockCheck2D(judgeNumberPlayerPos, moveSurfacePhase)) { return false; }

	//�ړ��\�Ȃ画��p�ϐ��ŕύX�����l���v���C���[�ʒu��\���}�b�v�ԍ��ɃZ�b�g
	mapChipNumberPlayerPos = judgeNumberPlayerPos;

	return true;
}

void PlayerActionManager::PlayerFrontmost2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//�v���C���[����ʎ�O�Ƀu���b�N�����݂��Ȃ���΃v���C���[����ʂ̈�Ԏ�O�ɔz�u
	//�ڒn�ʂ��u��v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��Ƀu���b�N�v�����邩�`�F�b�N
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		mapChipNumberPlayerPos.y = (int)mapChipNum[mapChipNumberPlayerPos.x].size() - 1;
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		mapChipNumberPlayerPos.y = 0;
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u�E�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		mapChipNumberPlayerPos.x = 0;
	}
	//�ڒn�ʂ��u�E�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		mapChipNumberPlayerPos.x = (int)mapChipNum.size() - 1;
	}
	//�ڒn�ʂ��u��O�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		mapChipNumberPlayerPos.z = 0;
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��O�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		mapChipNumberPlayerPos.z = (int)mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size() - 1;
	}
}

bool PlayerActionManager::DirectionForwardBlockCheck(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//�ڒn�ʂ��u��v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��Ƀu���b�N�v�����邩�`�F�b�N
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = mapChipNumberPlayerPos.y - 1; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		for (int i = mapChipNumberPlayerPos.x - 1; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u�E�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u�E�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u��O�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��O�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		for (int i = mapChipNumberPlayerPos.z - 1; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i])) {
				return true;
			}
		}
	}

	//�S�Ẵ`�F�b�N��ʉ߂ł��Ȃ����false��Ԃ�
	return false;
}

bool PlayerActionManager::DirectionAwayBlockCheck(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//�ڒn�ʂ��u��v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		for (int i = mapChipNumberPlayerPos.y - 1; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u�E�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u�E�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = mapChipNumberPlayerPos.x - 1; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	//�ڒn�ʂ��u��O�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i])) {
				return true;
			}
		}		
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��O�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = mapChipNumberPlayerPos.z - 1; i >= 0; --i) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i])) {
				return true;
			}
		}
	}

	//�S�Ẵ`�F�b�N��ʉ߂ł��Ȃ����false��Ԃ�
	return false;
}

void PlayerActionManager::PlayerScaffoldReturn3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//�ڒn�ʂ��u��v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		for (int i = mapChipNumberPlayerPos.y; i >= 0; --i) {
			//�u���b�N�łȂ���Δ�΂�
			if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z]))) { continue; }

			//�ŏ��Ɍ��������u���b�N�ɐڒn����ԍ��Ƀv���C���[���Z�b�g����
			mapChipNumberPlayerPos.y = i + 1;

			return;
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			//�u���b�N�łȂ���Δ�΂�
			if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z]))) { continue; }

			//�ŏ��Ɍ��������u���b�N�ɐڒn����ԍ��Ƀv���C���[���Z�b�g����
			mapChipNumberPlayerPos.y = i - 1;

			return;
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u�E�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			//�u���b�N�łȂ���Δ�΂�
			if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z]))) { continue; }

			//�ŏ��Ɍ��������u���b�N�ɐڒn����ԍ��Ƀv���C���[���Z�b�g����
			mapChipNumberPlayerPos.x = i - 1;

			return;
		}
	}
	//�ڒn�ʂ��u�E�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = mapChipNumberPlayerPos.x; i >= 0; --i) {
			//�u���b�N�łȂ���Δ�΂�
			if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z]))) { continue; }

			//�ŏ��Ɍ��������u���b�N�ɐڒn����ԍ��Ƀv���C���[���Z�b�g����
			mapChipNumberPlayerPos.x = i + 1;

			return;
		}
	}
	//�ڒn�ʂ��u��O�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			//�u���b�N�łȂ���Δ�΂�
			if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i]))) { continue; }

			//�ŏ��Ɍ��������u���b�N�ɐڒn����ԍ��Ƀv���C���[���Z�b�g����
			mapChipNumberPlayerPos.z = i - 1;

			return;
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��O�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = mapChipNumberPlayerPos.z; i >= 0; --i) {
			//�u���b�N�łȂ���Δ�΂�
			if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i]))) { continue; }

			//�ŏ��Ɍ��������u���b�N�ɐڒn����ԍ��Ƀv���C���[���Z�b�g����
			mapChipNumberPlayerPos.z = i + 1;

			return;
		}
	}
}

bool PlayerActionManager::PlayerGoalCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//����ƂȂ�u���b�N���S�[���Ȃ��true��Ԃ�
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y - 1][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y + 1][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		if (mapChipNum[mapChipNumberPlayerPos.x + 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		if (mapChipNum[mapChipNumberPlayerPos.x - 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z + 1] == MapBlockData::BlockType::Goal) {
			return true;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z - 1] == MapBlockData::BlockType::Goal) {
			return true;
		}
	}

	//�S�Ẵ`�F�b�N���N���A�ł��Ȃ����false��Ԃ�
	return false;
}

bool PlayerActionManager::PlayerGoalCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//�v���C���[�̃}�b�v�ԍ��̉��ɂ���u���b�N���`�F�b�N���A
	//��ԍŏ��ɔ��������u���b�N���S�[���ł����true�����A
	//����ȊO�̃u���b�N�Ȃ��false��Ԃ�

	//�ڒn�ʂ��u��v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		for (int i = mapChipNumberPlayerPos.y - 1; i >= 0; --i) {
			if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z])) {
				return false;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = mapChipNumberPlayerPos.y; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z])) {
				return false;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u�E�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		for (int i = mapChipNumberPlayerPos.x; i < mapChipNum.size(); i++) {
			if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
				return false;
			}
		}
	}
	//�ڒn�ʂ��u�E�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = mapChipNumberPlayerPos.x - 1; i >= 0; --i) {
			if (mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z] == MapBlockData::BlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
				return false;
			}
		}
	}
	//�ڒn�ʂ��u��O�v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u���Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		for (int i = mapChipNumberPlayerPos.z; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::BlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i])) {
				return false;
			}
		}
	}
	//�ڒn�ʂ��u���v�̏ꍇ�̓v���C���[�̃}�b�v�ԍ����u��O�Ƀu���b�N�v�����邩�`�F�b�N
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = mapChipNumberPlayerPos.z - 1; i >= 0; --i) {
			if (mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i] == MapBlockData::BlockType::Goal) {
				return true;
			}
			else if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i])) {
				return false;
			}
		}
	}

	//�S�Ẵ`�F�b�N��ʉ߂ł��Ȃ����false��Ԃ�
	return false;
}

void PlayerActionManager::PlayerMoveDirection3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase)
{
	//�v���C���[�̐ڒn�ʂ���A�܂��͉��̏ꍇ
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward || moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (Input::GetInstance()->PushKey(DIK_W)) {
			//�c���J�����ʒu�����ȊO�̂Ƃ�
			if (!(cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom)) {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) { mapChipNumberPlayerPos.z++; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) { mapChipNumberPlayerPos.x--; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) { mapChipNumberPlayerPos.z--; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) { mapChipNumberPlayerPos.x++; }
			}
			//�c���J�����ʒu�����̂Ƃ�
			else {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) { mapChipNumberPlayerPos.z--; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) { mapChipNumberPlayerPos.x++; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) { mapChipNumberPlayerPos.z++; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) { mapChipNumberPlayerPos.x--; }
			}
		}
		else if (Input::GetInstance()->PushKey(DIK_S)) {
			//�c���J�����ʒu�����ȊO�̂Ƃ�
			if (!(cameraYPosPhase == (int)GameCamera::CameraYPosPhase::Buttom)) {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) { mapChipNumberPlayerPos.z--; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) { mapChipNumberPlayerPos.x++; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) { mapChipNumberPlayerPos.z++; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) { mapChipNumberPlayerPos.x--; }
			}
			//�c���J�����ʒu�����̂Ƃ�
			else {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) { mapChipNumberPlayerPos.z++; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) { mapChipNumberPlayerPos.x--; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) { mapChipNumberPlayerPos.z--; }
				else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) { mapChipNumberPlayerPos.x++; }
			}
		}
		else if (Input::GetInstance()->PushKey(DIK_D)) {
			if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) { mapChipNumberPlayerPos.x++; }
			else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) { mapChipNumberPlayerPos.z++; }
			else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) { mapChipNumberPlayerPos.x--; }
			else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) { mapChipNumberPlayerPos.z--; }
		}
		else if (Input::GetInstance()->PushKey(DIK_A)) {
			if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) { mapChipNumberPlayerPos.x--; }
			else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) { mapChipNumberPlayerPos.z--; }
			else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) { mapChipNumberPlayerPos.x++; }
			else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) { mapChipNumberPlayerPos.z++; }
		}
	}
	//�v���C���[�̐ڒn�ʂ��O�㍶�E�̏ꍇ
	else {
		//�ڒn�ʂ��O�㍶�E�̏ꍇ��WS�ŏ㉺�ړ�
		if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.y--; }

		//���E�ړ��̔���
		else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft || moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
			if (Input::GetInstance()->PushKey(DIK_D)) {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front ||
					cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
					mapChipNumberPlayerPos.z++;
				}
				else { mapChipNumberPlayerPos.z--; }

			}
			else if (Input::GetInstance()->PushKey(DIK_A)) {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front ||
					cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
					mapChipNumberPlayerPos.z--;
				}
				else { mapChipNumberPlayerPos.z++; }
			}
		}
		else {
			if (Input::GetInstance()->PushKey(DIK_D)) {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front ||
					cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
					mapChipNumberPlayerPos.x++;
				}
				else { mapChipNumberPlayerPos.x--; }

			}
			else if (Input::GetInstance()->PushKey(DIK_A)) {
				if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front ||
					cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
					mapChipNumberPlayerPos.x--;
				}
				else { mapChipNumberPlayerPos.x++; }
			}
		}
	}
}

void PlayerActionManager::PlayerMoveDirection2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase)
{
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.z++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.z--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.x++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.x--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.x--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.x++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.z++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.z--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.z--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.z++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.x--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.x++; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.x++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.x--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.z--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.z++; }
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Front) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.z--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.z++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.x++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.x--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Right) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.x++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.x--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.z++; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.z--; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Back) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.z++; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.z--; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.x--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.x++; }
		}
		else if (cameraXPosPhase == (int)GameCamera::CameraXPosPhase::Left) {
			if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.x--; }
			else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.x++; }
			else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.z--; }
			else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.z++; }
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.z--; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.z++; }
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.z++; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.z--; }
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.x++; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.x--; }
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		if (Input::GetInstance()->PushKey(DIK_W)) { mapChipNumberPlayerPos.y++; }
		else if (Input::GetInstance()->PushKey(DIK_S)) { mapChipNumberPlayerPos.y--; }
		else if (Input::GetInstance()->PushKey(DIK_D)) { mapChipNumberPlayerPos.x--; }
		else if (Input::GetInstance()->PushKey(DIK_A)) { mapChipNumberPlayerPos.x++; }
	}
}

bool PlayerActionManager::PlayerMoveBlockCheck3D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//�ړ���̃}�b�v�ԍ��Ƀu���b�N�����݂���Έړ��ł����ɔ�����
	if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
		return false;
	}

	//����ƂȂ�u���b�N�����݂��Ȃ���Δ�����
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward) {
		if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y - 1][mapChipNumberPlayerPos.z]))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y + 1][mapChipNumberPlayerPos.z]))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft) {
		if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x + 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z]))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x - 1][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z]))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward) {
		if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z + 1]))) {
			return false;
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		if (!(MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z - 1]))) {
			return false;
		}
	}

	//�S�Ẵ`�F�b�N��ʉ߂�����true��Ԃ�
	return true;
}

bool PlayerActionManager::PlayerMoveBlockCheck2D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase)
{
	//2D�\�������āA�ړ���̎��Ƀu���b�N�����݂��Ă���Βʉ�(true)
	if (moveSurfacePhase == Player::MoveSurfacePhase::Upward || moveSurfacePhase == Player::MoveSurfacePhase::Downward) {
		for (int i = 0; i < mapChipNum[mapChipNumberPlayerPos.x].size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][i][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingLeft || moveSurfacePhase == Player::MoveSurfacePhase::FacingRight) {
		for (int i = 0; i < mapChipNum.size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[i][mapChipNumberPlayerPos.y][mapChipNumberPlayerPos.z])) {
				return true;
			}
		}
	}
	else if (moveSurfacePhase == Player::MoveSurfacePhase::FacingForward || moveSurfacePhase == Player::MoveSurfacePhase::FacingAway) {
		for (int i = 0; i < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size(); i++) {
			if (MapBlockData::MapChipNumBlockCheck(mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y][i])) {
				return true;
			}
		}
	}

	//�S�Ẵ`�F�b�N��ʉ߂ł��Ȃ����false��Ԃ�
	return false;
}

bool PlayerActionManager::PlayerMoveMapChipNumWithinRangeCheck(const XMINT3& mapChipNumberPlayerPos)
{
	//x,y,z���S�Ă��A�}�b�v�ԍ��͈͓��łȂ���Δ�����
	if (!(mapChipNumberPlayerPos.x >= 0 && mapChipNumberPlayerPos.x < mapChipNum.size())) { return false; }
	if (!(mapChipNumberPlayerPos.y >= 0 && mapChipNumberPlayerPos.y < mapChipNum[mapChipNumberPlayerPos.x].size())) { return false; }
	if (!(mapChipNumberPlayerPos.z >= 0 && mapChipNumberPlayerPos.z < mapChipNum[mapChipNumberPlayerPos.x][mapChipNumberPlayerPos.y].size())) { return false; }

	//�S�Ẵ`�F�b�N��ʉ߂�����true��Ԃ�
	return true;
}
