#pragma once
#include "Player.h"
#include "GameCamera.h"
#include <vector>

/// <summary>
/// �v���C���[�s���\����Ǘ�
/// </summary>
class PlayerActionManager
{
public: //�ÓI�����o�֐�
	//setter
	static void SetMapChipNum(const std::vector<std::vector<std::vector<int>>>& mapChipNum) { PlayerActionManager::mapChipNum = mapChipNum; }

	/// <summary>
	/// �v���C���[��3D�ړ��\����
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <param name="cameraXPosPhase">�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y</param>
	/// <param name="cameraYPosPhase">�X�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool PlayerMoveCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase);

	/// <summary>
	/// �v���C���[��2D�ړ��\����
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <param name="cameraXPosPhase">�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool PlayerMoveCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase);

	/// <summary>
	/// 2D���_�ň�Ԏ�O�Ƀv���C���[�������ꍇ�A�ǂ̃u���b�N������O�ɂ����Ԃɂ���
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	static void PlayerFrontmost2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// ���݂̃J���������Ō����ꍇ�Ƀv���C���[�̏�����Ƀu���b�N�����݂��邩����
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool DirectionForwardBlockCheck(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// ���݂̃J���������Ō����ꍇ�Ƀv���C���[�̉������Ƀu���b�N�����݂��邩����
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool DirectionAwayBlockCheck(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 3�����ɖ߂�Ƃ��̑���ƂȂ�u���b�N�ɐڂ���}�b�v�ԍ���ݒ�
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	static void PlayerScaffoldReturn3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 3������Ԃ̃v���C���[���S�[�������̂��𔻒�
	/// </summary>
	///  <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool PlayerGoalCheck3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 2������Ԃ̃v���C���[���S�[�������̂��𔻒�
	/// </summary>
	///  <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool PlayerGoalCheck2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

private: //�ÓI�����o�֐�
	/// <summary>
	/// �L�[���͂ɑ΂���v���C���[3D�ړ��̕�����ݒ�
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <param name="cameraXPosPhase">�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y</param>
	/// <param name="cameraYPosPhase">�X�e�[�W���猩�Ă̏c���J�����ʒu�t�F�[�Y</param>
	static void PlayerMoveDirection3D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase, const int cameraYPosPhase);

	/// <summary>
	/// �L�[���͂ɑ΂���v���C���[2D�ړ��̕�����ݒ�
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="cameraXPosPhase">�X�e�[�W���猩�Ẳ����J�����ʒu�t�F�[�Y</param>
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	static void PlayerMoveDirection2D(XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase, const int cameraXPosPhase);

	/// <summary>
	/// 3D�ړ��\�ȃu���b�N�󋵂��𔻒�
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param> 
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool PlayerMoveBlockCheck3D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 2D�ړ��\�ȃu���b�N�󋵂��𔻒�
	/// </summary>
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param> 
	/// <param name="moveSurfacePhase">�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y</param>
	/// <returns>����</returns>
	static bool PlayerMoveBlockCheck2D(const XMINT3& mapChipNumberPlayerPos, const Player::MoveSurfacePhase moveSurfacePhase);

	/// <summary>
	/// 3D�ړ��̃u���b�N�ԂɃn���{�e�����݂����A�ړ��\���𔻒�
	/// </summary>
	/// <param name="mapChipNumberPlayerBeforePos">�ړ��O�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="mapChipNumberPlayerAfterPos">�ړ���v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <returns>����</returns>
	static bool PlayerMovePlaneCheck3D(const XMINT3& mapChipNumberPlayerBeforePos, const XMINT3& mapChipNumberPlayerAfterPos);

	/// <summary>
	/// �v���C���[�̈ʒu��\���}�b�v�ԍ����͈͓������`�F�b�N
	/// </summary> 
	/// <param name="mapChipNumberPlayerPos">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <returns>����</returns>
	static bool PlayerMoveMapChipNumWithinRangeCheck(const XMINT3& mapChipNumberPlayerPos);

private: //�ÓI�����o�ϐ�
	//3�����}�b�v�p��3�����z��
	static std::vector<std::vector<std::vector<int>>> mapChipNum;
};