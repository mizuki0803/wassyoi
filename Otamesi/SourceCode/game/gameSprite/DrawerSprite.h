#pragma once
#include "Sprite.h"

/// <summary>
/// �����o���ړ��X�v���C�g
/// </summary>
class DrawerSprite : public Sprite
{
public: //enum
	//�����o���ŉB���ꏊ
	enum HidePlace
	{
		Left,
		Right,
		Up,
		Down,
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="drawerKey">�����o���̊J�Ɏg�p����L�[</param>
	/// <param name="hidePlace">�����o���ŉB���ꏊ</param>
	/// <param name="posY">Y���W</param>
	/// <param name="stickoutNum">���Ă���ꍇ�ɉ�ʓ��ɂ͂ݏo����</param>
	/// <param name="isOpenDrawer">�����o�����J���Ă����Ԃ�</param>
	/// <returns>�����o���ړ��X�v���C�g</returns>
	static DrawerSprite* Create(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer = false);


public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="drawerKey">�����o���̊J�Ɏg�p����L�[</param>
	/// <param name="hidePlace">�����o���ŉB���ꏊ</param>
	/// <param name="posY">Y���W</param>
	/// <param name="stickoutNum">���Ă���ꍇ�ɉ�ʓ��ɂ͂ݏo����</param>
	/// <param name="isOpenDrawer">�����o�����J���Ă����Ԃ�</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �J�ړ��J�n
	/// </summary>
	void MoveStart();

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	//gettre
	BYTE GetDrawerKey() { return drawerKey; }

private: //�����o�֐�
	/// <summary>
	/// �J�ړ�
	/// </summary>
	void Move();

private: //�ÓI�����o�ϐ�
	//�J�ړ��ɂ����鎞��
	static const int moveTime = 30;

private: //�����o�ϐ�
	//�����o�����J���Ă����Ԃ�
	bool isOpenDrawer = false;
	//�V�[���J�n���̊J��ԕۑ��p
	bool isOpenDrawerSceneStart;
	//�����o���̊J�Ɏg�p����L�[
	BYTE drawerKey;
	//�J���Ă���ꍇ�̍��W
	Vector2 openStatePos;
	//���Ă���ꍇ�̍��W
	Vector2 closedStatePos;
	//���Ă���ꍇ�ɉ�ʓ��ɂ͂ݏo����
	float stickoutNum;
	//�J�ړ��J�n�����W
	Vector2 moveStartPos;
	//�J�ړ��p�^�C�}�[
	int32_t moveTimer = 0;
	//�J�ړ�����
	bool isMoveDrawer = false;
};