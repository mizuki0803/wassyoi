#pragma once
#include "Sprite.h"
#include <array>
#include <memory>

/// <summary>
/// �X�e�[�W�I���V�[���p�̍��E���
/// </summary>
class StageSelectArrow
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�X�e�[�W�I���V�[���p�̍��E���</returns>
	static StageSelectArrow* Create();

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I�𒆂̃X�e�[�W�Ŗ��X�v���C�g��`�悷�邩����
	/// </summary>
	void SelectStageCheckDraw();

private: //�����o�ϐ�
	//���X�v���C�g
	std::array<std::unique_ptr<Sprite>, 2> arrows;
	//�`��t���O
	std::array<bool, 2> isDraws;
};
