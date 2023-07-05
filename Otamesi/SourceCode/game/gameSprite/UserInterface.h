#pragma once
#include "Menu.h"
#include "DrawerSprite.h"
#include <array>
#include <vector>

class UserInterface
{
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

	//�t���O�̐ݒ�A�擾
	void SetMenuFlag(bool flag) { menuFlag_ = flag; }
	void SetNotMove(bool flag) { notMove_ = flag; }

	bool GetMenuFlag() { return menuFlag_; }

private: //�����o�֐�
	/// <summary>
	/// �����o���X�v���C�g����
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="keyNumber">�J�L�[���蓖��</param>
	/// <param name="hidePlace">�����o���ŉB���ꏊ</param>
	/// <param name="posY">Y���W</param>
	/// <param name="stickoutNum">���Ă���ꍇ�ɉ�ʓ��ɂ͂ݏo����</param>
	/// <param name="isOpenDrawer">�����o�����J���Ă����Ԃ�</param>
	/// <returns>�����o���ړ��X�v���C�g</returns>
	void CreateNewDrawerSprite(const Texture& texture, BYTE keyNumber, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer = false);

private: //�����o�ϐ�
	//�����p�����o���X�v���C�g
	std::vector<std::unique_ptr<DrawerSprite>> drawerSprites;
	// �C�[�W���O�i�s
	float easeTimer_ = 0.0f;
	// ���j���[�p�̔w�i
	std::unique_ptr<Sprite> menuBackScreen_;
	// ���j���[�p�̘g�g��
	std::array<std::unique_ptr<Menu>, 3> menuframe_;
	// �s���t���O
	bool menuFlag_ = false;
	//
	bool notMove_ = false;
};

