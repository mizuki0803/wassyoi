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
	/// �s���̐ݒ�
	/// </summary>
	/// <param name="movePhase">�s��</param>
	void SetMovePhase(MovePhase movePhase) { phase_ = static_cast<int>(movePhase); }
	/// <summary>
	/// �I���t���O�̎擾
	/// </summary>
	/// <returns>�I���t���O</returns>
	bool GetEndFlag() { return endFlag_; }
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

private: //�����o�֐�
	/// <summary>
	/// ����
	/// </summary>
	void StartMove();
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
	// �I���t���O
	bool endFlag_ = false;
};

