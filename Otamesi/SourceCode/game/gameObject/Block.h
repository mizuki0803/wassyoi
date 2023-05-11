#pragma once
#include "ObjObject3d.h"
#include "EaseData.h"

/// <summary>
/// �}�b�v�p�u���b�N
/// </summary>
class Block : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�u���b�N</returns>
	static Block* Create(ObjModel* model, const Vector3& position);

	//getter
	static float GetBlockSize() { return Block::blockSize; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �Q�[���J�n��
	/// </summary>
	void PlayStratMove();

	/// <summary>
	/// �C�[�W���O���I�������
	/// </summary>
	/// <returns>�I���t���O</returns>
	bool IsEaseEndFlag() { return easeData_->GetEndFlag(); }
	/// <summary>
	/// �C�[�W���O�f�[�^�̐ݒ�
	/// </summary>
	/// <param name="count">�t���[���̃J�E���g</param>
	void SetEaseData(const int count);
	/// <summary>
	/// �C�[�W���O���J�n���鎞�̈ʒu�̕ۑ�
	/// </summary>
	/// <param name="position"></param>
	void SetBlockStratPos(const Vector3& position) { blockStratPos_ = position; }
	/// <summary>
	/// �C�[�W���O���I�����鎞�̈ʒu�̕ۑ�
	/// </summary>
	/// <param name="position"></param>
	void SetBlockEndPos(const Vector3& position) { blockEndPos_ = position; }

private: //�ÓI�����o�ϐ�
	//�u���b�N��̑傫��
	static const float blockSize;

private: // �����o�ϐ�
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_ = 0;

#pragma region
	// �J�n�ʒu
	Vector3 blockStratPos_;
	// �I���ʒu
	Vector3 blockEndPos_;
	// �C�[�W���O�f�[�^
	std::unique_ptr<EaseData> easeData_;
#pragma endregion �C�[�W���O�֌W
};