#pragma once
#include "FrameWork.h"

/// <summary>
/// �Q�[���ŗL�̃N���X
/// </summary>
class AndosGame : public FrameWork
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �I��
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
};