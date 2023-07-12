#pragma once
#include "ObjObject3d.h"
#include "GameCamera.h"
#include "EaseData.h"
#include "PlayerEffect.h"


/// <summary>
/// �v���C���[
/// </summary>
class Player : public ObjObject3d
{
public:
	friend class PlayerEffect;

	//enum
	/// <summary>
	/// �s���t�F�[�Y
	/// </summary>
	enum class ActionPhase
	{
		None,	//�������Ȃ�
		MovePos,	//���W�ړ�
	};

	/// <summary>
	/// �Q�[���t�F�[�Y
	/// </summary>
	enum class GamePhase
	{
		GamePlay,	//�Q�[��
		Start,		//�J�n
		ReStart,	//�ĊJ�n
	};

	/// <summary>
	/// �㉺���E�O��A�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y
	/// </summary>
	enum class MoveSurfacePhase
	{
		Upward,			//�����
		Downward,		//������
		FacingLeft,		//������
		FacingRight,	//�E����
		FacingForward,	//��O����
		FacingAway,		//������
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="mapChipNum">�v���C���[�ʒu��\���}�b�v�ԍ�</param>
	/// <param name="shiftPos">�}�b�v�𒆐S�ɂ��炷�l</param>
	/// <param name="gameCamera">�Q�[���J����</param>
	/// <returns>�v���C���[</returns>
	static Player* Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& shiftPos, GameCamera* gameCamera, ObjModel* effectModel);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �Q�[����
	/// </summary>
	void PlayGame();
	/// <summary>
	/// �Q�[���J�n
	/// </summary>
	void GameStart();
	/// <summary>
	/// �Q�[���ĊJ�n
	/// </summary>
	void GameReStart();

	/// <summary>
	/// �֐��̐ݒ�
	/// </summary>
	void CreateAct();

	void ReCreate(const XMINT3& mapChipNum);

	void Draw();
	void EffectDraw();

	//getter
	const bool GetIsGoal() { return isGoal; }
	const bool GetIsMove() { return isMove; }
	const bool GetNowMove() { return bool(actionPhase); }
	const bool GetIsStartMove() { return isStartMove; }
	const ActionPhase GetActionPhase() { return actionPhase; }
	const int GetMoveSurfacePhase() { return int(moveSurfacePhase); }
	const XMINT3 GetMapChipNumberPos() { return mapChipNumberPos; }
	//setter
	void SetMoveSurfacePhase(const int _moveSurfacePhase) { moveSurfacePhase = MoveSurfacePhase(_moveSurfacePhase); }
	void SetMapChipNumberPos(const XMINT3& _mapChipNumberPos) { mapChipNumberPos = _mapChipNumberPos; }

private: //�����o�֐�
	/// <summary>
	/// ���W�ړ��J�n
	/// </summary>
	void MovePosStart();

	/// <summary>
	/// ���W�ړ�
	/// </summary>
	void MovePos();

	/// <summary>
	/// �����؂�ւ��J�n
	/// </summary>
	void ChanegeDimensionStart();

	/// <summary>
	/// �X�e�[�W�N���A�̃`�F�b�N
	/// </summary>
	void StageClearCheck();

	/// <summary>
	/// �}�b�v�ԍ��̍��W���擾
	/// </summary>
	Vector3 GetMapChipPos(const XMINT3& mapChipNumberPos);

	/// <summary>
	/// �C�[�W���O�f�[�^�̐ݒ�
	/// </summary>
	/// <param name="count">�t���[���̃J�E���g</param>
	void SetEaseData(const int count);
	/// <summary>
	/// �C�[�W���O���J�n���鎞�̈ʒu�̕ۑ�
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerStratPos(const Vector3& position) { playerStratPos_ = position; }
	/// <summary>
	/// �C�[�W���O���I�����鎞�̈ʒu�̕ۑ�
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerEndPos(const Vector3& position) { playerEndPos_ = position; }
	/// <summary>
	/// �s���̐ݒ�
	/// </summary>
	/// <param name="gamePhase"></param>
	void SetGamePhase(GamePhase gamePhase) { phase_ = static_cast<int>(gamePhase); }

private: //�ÓI�����o�ϐ�
	//�v���C���[�̑傫��
	static const float playerSize;

private: //�����o�ϐ�

	std::array<std::unique_ptr<PlayerEffect>, 3> effect;


	//�v���C���[�ʒu��\���}�b�v�ԍ�
	XMINT3 mapChipNumberPos;
	//�}�b�v�̒��S�ɂ��炷�l
	Vector3 shiftPos;
	//�Q�[���J����
	GameCamera* gameCamera = nullptr;
	//�A�N�V����
	ActionPhase actionPhase = ActionPhase::None;
	//�A�N�V�����p�^�C�}�[
	int32_t actionTimer = 0;
	//�u���b�N�̂ǂ̖ʈړ����邩�t�F�[�Y
	MoveSurfacePhase moveSurfacePhase = MoveSurfacePhase::Upward;
	//�ړ��O���W
	Vector3 moveBeforePos;
	//�ړ��ڕW���W
	Vector3 moveAfterPos;
	//�S�[��������
	bool isGoal = false;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_ = static_cast<int>(GamePhase::Start);
#pragma region
	// �J�n�ʒu
	Vector3 playerStratPos_;
	// �I���ʒu
	Vector3 playerEndPos_;
	// �C�[�W���O�f�[�^
	std::unique_ptr<EaseData> easeData_;
	// �C�[�W���O�̃��Z�b�g�t���O
	bool resetFlag_ = false;
#pragma endregion �C�[�W���O�֌W

	//�ړ��������I������^�C�~���O
	bool isMove;

	//�ŏ��̈ړ����I��������̃t���O
	bool isStartMove;
};