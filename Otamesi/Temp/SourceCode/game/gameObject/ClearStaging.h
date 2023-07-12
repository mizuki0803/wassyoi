#pragma once
#include "Sprite.h"
#include "EaseData.h"
#include <memory>
#include <vector>
#include <functional>

class ClearStaging
{
public: //enum
	/// <summary>
	/// �S�̗���
	/// </summary>
	enum class MovePhase
	{
		Start,			//�J�n
		intermediate1,	//�ړ�
		intermediate2,	//�폜
		End,			//�I��
		None,	//�������Ȃ�
	};

public: //�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <returns></returns>
	static std::unique_ptr<ClearStaging> Create();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �t���O�̐ݒ�
	/// </summary>
	/// <param name="flag">�t���O</param>
	void SetPushFlag(bool flag) { pushFlag_ = flag; }
	/// <summary>
	/// �s���̐ݒ�
	/// </summary>
	/// <param name="movePhase">�s��</param>
	void SetMovePhase(MovePhase movePhase) { phase_ = static_cast<int>(movePhase); }

private: //�����o�֐�
	/// <summary>
	/// ����
	/// </summary>
	void StartMove();
	/// <summary>
	/// ���Ԃ̓���1
	/// </summary>
	void Intermediate1Move();
	/// <summary>
	/// ���Ԃ̓���2
	/// </summary>
	void Intermediate2Move();
	/// <summary>
	/// �I���̓���
	/// </summary>
	void EndMove();

private: //�����o�ϐ�
	// �C�[�W���O
	std::unique_ptr<EaseData> easeData_;
	// �w�i
	std::unique_ptr<Sprite> backScreen_;
	// �e�L�X�g
	std::unique_ptr<Sprite> textSprite_;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_;
	// �{�^������������
	bool pushFlag_ = false;
};

