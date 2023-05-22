#pragma once
#include "ObjObject3d.h"
#include "EaseData.h"

/// <summary>
/// �}�b�v�p�u���b�N
/// </summary>
class Block : public ObjObject3d
{
public: //enum
	/// <summary>
	/// �Q�[���̑S��
	/// </summary>
	enum class GamePhase
	{
		Strat,	//�J�n
		None,	//�������Ȃ�
	};

	/// <summary>
	/// �u���b�N�̎��
	/// </summary>
	enum class BlockType
	{
		Block,
		Plane,
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="model">���f��</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <returns>�u���b�N</returns>
	static Block* Create(ObjModel* model, const XMINT3& mapChipNum);

	//getter
	static float GetBlockSize() { return Block::blockSize; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <returns>����</returns>
	bool Initialize(ObjModel* model, const XMINT3& mapChipNum);

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
	/// <summary>
	/// �֐��̐ݒ�
	/// </summary>
	void CreateAct();
	/// <summary>
	/// �ǂ̍s�������Ă��邩�̎擾
	/// </summary>
	/// <returns></returns>
	int GetActPhase() { return static_cast<int>(phase_); }

protected: //�ÓI�����o�ϐ�
	//�u���b�N��̑傫��
	static const float blockSize;

protected: // �����o�ϐ�
	//�u���b�N�̎��
	BlockType blockType;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_ = static_cast<int>(GamePhase::Strat);

#pragma region
	// �J�n�ʒu
	Vector3 blockStratPos_;
	// �I���ʒu
	Vector3 blockEndPos_;
	// �C�[�W���O�f�[�^
	std::unique_ptr<EaseData> easeData_;
#pragma endregion �C�[�W���O�֌W
};