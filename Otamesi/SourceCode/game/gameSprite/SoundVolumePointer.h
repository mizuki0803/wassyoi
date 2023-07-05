#pragma once
#include "Sprite.h"

/// <summary>
/// ���ʕύX�p�|�C���^�[�X�v���C�g
/// </summary>
class SoundVolumePointer : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="barLeftPos">���ʃo�[���[���W</param>
	/// <param name="barLength">���ʃo�[�̒���</param>
	/// <param name="startPercentage">�������̉��ʃp�[�Z���e�[�W</param>
	/// <returns>���ʕύX�p�|�C���^�[�X�v���C�g</returns>
	static SoundVolumePointer* Create(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="barLeftPos">���ʃo�[���[���W</param>
	/// <param name="barLength">���ʃo�[�̒���</param>
	/// <param name="startPercentage">�������̉��ʃp�[�Z���e�[�W</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, const Vector2 barLeftPos, float barLength, float startPercentage);

	/// <summary>
	/// ���ʊ����ύX
	/// </summary>
	void SetPercentage(float percentage);

private: //�����o�ϐ�
	//�o�[�̍��[���W
	Vector2 barLeftPos;
	//�o�[�̒���
	float barLength;
};