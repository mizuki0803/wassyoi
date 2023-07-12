#pragma once
#include "Block.h"
#include "EaseData.h"
#include <memory>

/// <summary>
/// �X�e�[�W�p�u���b�N
/// </summary>
class StageBlock : public Block
{
public: //enum
	/// <summary>
	/// �Q�[���̑S��
	/// </summary>
	enum class GamePhase
	{
		Start,	//�J�n
		Move,	//�ړ�
		Delete,	//�폜
		ReStart,//�ēx����
		None,	//�������Ȃ�
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="_blockType">���f���ԍ�</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <param name="shiftPos">�}�b�v�𒆐S�ɂ��炷�l</param>
	/// <returns>�X�e�[�W�p�u���b�N</returns>
	static StageBlock* Create(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_blockType">���f���ԍ�</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <param name="shiftPos">�}�b�v�𒆐S�ɂ��炷�l</param>
	/// <returns>����</returns>
	bool Initialize(const int _blockType, const XMINT3& mapChipNum, const Vector3& shiftPos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �Q�[���J�n��
	/// </summary>
	void PlayStratMove();
	/// <summary>
	/// �Đ������̈ړ�
	/// </summary>
	void ReStratMove();
	/// <summary>
	/// �폜���̈ړ�
	/// </summary>
	void DeleteMove();
	/// <summary>
	/// �Đ������̒ǉ������u���b�N�̈ړ�
	/// </summary>
	void ReCreateMove();

	/// <summary>
	/// �Đ����̐ݒ�
	/// </summary>
	/// <param name="phase">�s���^�C�v</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	void ReCreate(const GamePhase phase, const XMINT3& mapChipNum);

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

	/// <summary>
	/// �폜�t���O�̎擾
	/// </summary>
	/// <returns>�폜�t���O</returns>
	bool GetDeleteFlag() { return deleteFlag_; }

	/// <summary>
	/// �s���^�C�v�̐ݒ�
	/// </summary>
	/// <param name="phase">�s���^�C�v</param>
	void SetGamePhase(GamePhase phase) { phase_ = static_cast<int>(phase); }

	/// <summary>
	/// �v���C���[����̃R�[���o�b�N
	/// </summary>
	void ReAction();

	/// <summary>
	/// �����_���̌v�Z
	/// </summary>
	/// <param name="a">�ŏ��l</param>
	/// <param name="b">�ő�l</param>
	const float RandCalculate(float a, float b);

protected: // �����o�ϐ�
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_;
	//�폜�p�̃t���O
	bool deleteFlag_ = false;

#pragma region
	// �J�n�ʒu
	Vector3 blockStratPos_;
	// �I���ʒu
	Vector3 blockEndPos_;
	// �ړ���
	Vector3 vec_ = {};
	// �C�[�W���O�f�[�^
	std::unique_ptr<EaseData> easeData_;
	// �ړ��ʗp�C�[�W���O�f�[�^
	std::unique_ptr<EaseData> vecEaseData_;
#pragma endregion �C�[�W���O�֌W
};