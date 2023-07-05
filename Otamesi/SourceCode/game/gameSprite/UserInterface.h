#pragma once
#include "Menu.h"
#include "DrawerSprite.h"
#include <array>
#include <vector>
#include <functional>

class UserInterface
{
public: //enum
	/// <summary>
	/// �����o���X�v���C�g�̖�����
	/// </summary>
	enum DrawerSpriteName
	{
		HowToPlayMenu,		//���j���[
		Hint1,				//�q���g1
		Hint2,				//�q���g2
		HowToPlayPlayer,	//�v���C���[����
		HowToPlayCamera,	//�J��������

		DrawerSpriteNum,	//�����o���X�v���C�g��
	};

	/// <summary>
	/// ���j���[�̑S��
	/// </summary>
	enum class MenuPhase
	{
		Start,		//�J�n
		Selection,	//�I��
	};

public: //�����o�֐�
	/// <summary>
	/// ����
	/// </summary>
	static std::unique_ptr<UserInterface> Create();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// ���̕`��
	/// </summary>
	void Draw();

	// ���j���[�֌W�̏���
	void MenuUpdate();
	// 
	void MenuReset();
	//
	void MenuOpen();
	//
	void MenuSelection();

	/// <summary>
	/// �����o���X�v���C�g�̊J��Ԃ����Z�b�g
	/// </summary>
	void DrawerSpriteReset();

	//�t���O�̐ݒ�A�擾
	void SetMenuFlag(bool flag) { menuFlag_ = flag; drawerSprites[HowToPlayMenu]->MoveStart(); }
	void SetNotMove(bool flag) { notMove_ = flag; }

	bool GetMenuFlag() { return menuFlag_; }

private: //�����o�֐�
	/// <summary>
	/// �L�[���͂ɂ������o���X�v���C�g�ړ��J�n
	/// </summary>
	void DrawerSpriteMoveStartKey();

private: //�����o�ϐ�
	//�����p�����o���X�v���C�g
	std::array<std::unique_ptr<DrawerSprite>, DrawerSpriteNum> drawerSprites;
	// �C�[�W���O�i�s
	float easeTimer_ = 0.0f;
	// ���j���[�p�̔w�i
	std::unique_ptr<Sprite> menuBackScreen_;
	// ���j���[�p�̘g�g��
	std::array<std::unique_ptr<Menu>, 3> menuframe_;
	// �s���t���O
	bool menuFlag_ = false;
	// �s���������Ȃ�
	bool notMove_ = false;
	// ���j���[�I��
	int selectionNumber_ = 0;
	// �I���\��
	bool selectionFlag_ = false;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> menuFunc_;
	// �֐��̔ԍ�
	size_t menuPhase_ = static_cast<int>(MenuPhase::Start);
	// ���̑傫��
	float soundVolume_ = 0.1f;
};

