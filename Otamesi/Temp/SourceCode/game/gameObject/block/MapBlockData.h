#pragma once

/// <summary>
/// �}�b�v�u���b�N�f�[�^
/// </summary>
class MapBlockData
{
public: //enum
	//�}�b�v�p�u���b�N�̎��
	enum MapBlockType
	{
		None,			//�Ȃɂ��Ȃ�
		Player,			//�v���C���[
		Goal,			//�S�[��
		Block,			//�u���b�N
		UpPlane,		//������n���{�e
		DownPlane,		//�������n���{�e
		LeftPlane,		//�������n���{�e
		RightPlane,		//�E�����n���{�e
		ForwardPlane,	//��O�����n���{�e
		AwayPlane,		//�������n���{�e
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// �w�肵���}�b�v�ԍ����u���b�N�ł��邩�`�F�b�N
	/// </summary>
	/// <param name="mapChipNum">�}�b�v�ԍ�</param>
	/// <returns>����</returns>
	static bool MapChipNumBlockCheck(const int mapChipNum);

	/// <summary>
	/// �w�肵���}�b�v�ԍ����u���b�N�܂��͂��̖ʂɓK�����n���{�e�ł��邩�`�F�b�N
	/// </summary>
	/// <param name="mapChipNum">�}�b�v�ԍ�</param>
	/// <returns>����</returns>
	static bool MapChipNumBlockPlaneCheck(const int mapChipNum, MapBlockType planeType);
};