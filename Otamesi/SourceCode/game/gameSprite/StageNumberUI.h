#pragma once
#include "NumberSprite.h"
#include <array>
#include <memory>

/// <summary>
/// �X�e�[�W�ԍ��\��UI
/// </summary>
class StageNumberUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="size">�傫��</param>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	/// <returns>�X�e�[�W�ԍ��\��UI</returns>
	static StageNumberUI* Create(const Vector2& position, const float size, const int stageNum);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="size">�傫��</param>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(const Vector2& position, const float size, const int stageNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �X�e�[�W�ԍ��̕ύX
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�ԍ�</param>
	void ChengeStageNum(const int stageNum);

private: //�ÓI�����o�ϐ�
	//����
	static const int digitNum = 2;

private: //�����o�ϐ�
	//�����X�v���C�g
	std::array<std::unique_ptr<NumberSprite>, digitNum> numbers;
};