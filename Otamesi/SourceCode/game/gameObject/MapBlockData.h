#pragma once

/// <summary>
/// �}�b�v�u���b�N�f�[�^
/// </summary>
class MapBlockData
{
public: //enum
	//�u���b�N�̎��
	enum BlockType
	{
		None,	//�Ȃɂ��Ȃ�
		Player,	//�v���C���[
		Goal,	//�S�[��
		Block,	//�u���b�N
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �w�肵���}�b�v�ԍ����u���b�N�ł��邩�`�F�b�N
	/// </summary>
	/// <param name="mapChipNum">�}�b�v�ԍ�</param>
	/// <returns>����</returns>
	static bool MapChipNumBlockCheck(const int mapChipNum);
};