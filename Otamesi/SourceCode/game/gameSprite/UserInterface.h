#pragma once
#include "Menu.h"
#include <array>

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
	bool GetMenuFlag() { return menuFlag_; }

private: //�����o�ϐ�
	// �C�[�W���O�i�s
	float easeTimer_ = 0.0f;
	// ���j���[�p�̔w�i
	std::unique_ptr<Sprite> menuBackScreen_;
	// ���j���[�p�̘g�g��
	std::array<std::unique_ptr<Menu>, 3> menuframe_;
	// �s���t���O
	bool menuFlag_ = false;
};

