#pragma once
#include "ParticleManager.h"

/// <summary>
/// �p�[�e�B�N���G�~�b�^�[
/// </summary>
class ParticleEmitter final
{
private: //�V���O���g����
	//�R���X�g���N�^���B��
	ParticleEmitter() = default;
	//�f�X�g���N�^���B��
	~ParticleEmitter() = default;
public:
	//�R�s�[�R���X�g���N�^�𖳌���
	ParticleEmitter(const ParticleEmitter& particleEmitter) = delete;
	//������Z�q�𖳌���
	void operator = (const ParticleEmitter& particleEmitter) = delete;

public: //�e�N�X�`����
	enum TextureName {
		Circle,		//��
		Fluffy,	//��������
		Shine,		//�L���L��
		Curve,		//�Ȑ�

		ParticleTextureNum,	//�p�[�e�B�N���e�N�X�`���̖���
	};

public: //�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�p�[�e�B�N���G�~�b�^�[</returns>
	static ParticleEmitter* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// ��ĕ`��
	/// </summary>
	void DrawAll();

	/// <summary>
	/// �������p���o
	/// </summary>
	void DemoEffect();

	/// <summary>
	/// �������p���o
	/// </summary>
	void DemoEffect2();

	/// <summary>
	/// ���@�̃W�F�b�g���ˉ��o
	/// </summary>
	/// <param name="getTargetPos">�p�[�e�B�N�����Ǐ]������W�擾�֐�</param>
	/// <param name="playerMatWorld">���@���[���h�s��</param>
	/// <param name="playerSpeedPhase">���@�ړ����x�t�F�[�Y</param>
	void PlayerJet(std::function<Vector3()> getTargetPos, const XMMATRIX& playerMatWorld, const int playerSpeedPhase = 0);

	/// <summary>
	/// ���@�̎��S�W�F�b�g�������ˉ��o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void PlayerBlackSmokeJet(const Vector3& position);

	/// <summary>
	/// ���@�̗�����o���s�@�_(���F�C�p�[)
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�p�[�e�B�N���傫��</param>
	/// <param name="velocity">�p�[�e�B�N���ړ����x</param>
	void PlayerVapor(const Vector3& position, const float size, const Vector3& velocity = {});

	/// <summary>
	/// ���@�ً}���p���o
	/// </summary>
	/// <param name="getTargetPos">�p�[�e�B�N�����Ǐ]������W�擾�֐�</param>
	/// <param name="size">�p�[�e�B�N���傫��</param>
	void PlayerRolling(const int lifeTime, std::function<Vector3()> getTargetPos, const Vector3& localPos, const float rotation, const float size, const XMFLOAT4& color);

	/// <summary>
	/// �V���b�g���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	void Shot(const Vector3& position);

	/// <summary>
	/// �V���b�g���S���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void ShotDead(const Vector3& position, const float size);

	/// <summary>
	/// �`���[�W�V���b�g���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void ChargeShot(const Vector3& position, const float size);

	/// <summary>
	/// �`���[�W�V���b�g���S���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void ChargeShotDead(const Vector3& position, const float size);

	/// <summary>
	/// �Ή������o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	/// <param name="time">��������</param>
	void FireBall(const Vector3& position, const float size = 1, const int time = 10);

	/// <summary>
	/// �{�X�`���[�W���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void BossCharge(const Vector3& position, const float size);

	/// <summary>
	/// �{�X�`���[�W�V���b�g���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void BossChargeShot(const Vector3& position, const float size);

	/// <summary>
	/// �{�X�`���[�W�V���b�g���S���o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	void BossChargeShotDead(const Vector3& position, const float size);

	/// <summary>
	/// �������o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	/// <param name="time">��������</param>
	void Explosion(const Vector3& position, const float size = 1, const int time = 10);

	/// <summary>
	/// �{�X���S��p�������o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�傫��</param>
	/// <param name="isGround">�n�ʂ����݂��邩</param>
	void BossDeadExplosion(const Vector3& position, const float size, const bool isGround);

	/// <summary>
	/// �������o
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�p�[�e�B�N���傫��</param>
	/// <param name="time">��������</param>
	void BlackSmoke(const Vector3& position, const float size, const int time = 30);

	/// <summary>
	/// �A�C�e���̃L���L��
	/// </summary>
	/// <param name="position">�p�[�e�B�N���������W</param>
	/// <param name="size">�p�[�e�B�N���傫��</param>
	/// <param name="num">��x�ɏo����</param>
	void ItemShine(const Vector3& position, const float size, const int num);

	/// <summary>
	/// �S�폜
	/// </summary>
	void AllDelete();

private: //�����o�֐�
	/// <summary>
	/// �e�N�X�`���ǂݍ���
	/// </summary>
	void LoadTexture();

private: //�����o�ϐ�
	//�e�N�X�`��
	Texture textures[ParticleTextureNum];
	//�p�[�e�B�N���}�l�[�W���[(��)
	std::unique_ptr<ParticleManager> circleParticle;
	//�p�[�e�B�N���}�l�[�W���[(����)
	std::unique_ptr<ParticleManager> explosionParticle;
	//�p�[�e�B�N���}�l�[�W���[(����)
	std::unique_ptr<ParticleManager> blackSmokeParticle;
	//�p�[�e�B�N���}�l�[�W���[(�L���L��)
	std::unique_ptr<ParticleManager> shineParticle;
	//�p�[�e�B�N���}�l�[�W���[(�Ȑ�)
	std::unique_ptr<ParticleManager> curveParticle;
};
