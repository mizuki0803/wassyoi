#pragma once
#include "BirdBody.h"
#include "birdWing.h"
#include <memory>
#include <array>

/// <summary>
/// ��
/// </summary>
class Bird
{
public: //enum
	/// <summary>
	/// ���U�t�F�[�Y
	/// </summary>
	enum LifePhase
	{
		Born,	//���܂��
		Alive,	//�����Ă���
		Fadeout	//�����Ă���
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">�ړ����x</param>
	/// <returns>��</returns>
	static Bird* Create(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="position">���W</param>
	/// <param name="velocity">�ړ����x</param>
	/// <returns>����</returns>
	bool Initialize(ObjModel* bodyModel, ObjModel* wingModel, const Vector3& position, const Vector3& velocity);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//getter
	bool GetIsDead() { return isDead; }

private: //�����o�֐�
	/// <summary>
	/// �Y�܂��
	/// </summary>
	void BornAction();

	/// <summary>
	/// ���S�m�F
	/// </summary>
	void DeadCheck();

	/// <summary>
	/// �����Ă���
	/// </summary>
	void FadeoutAction();

private: //�ÓI�����o�ϐ�
	//���S������W���E
	static float deadLine;
	//���U�t�F�[�Y�̍s���֐�
	static void (Bird::* lifePhasefuncTable[])();

private: //�����o�ϐ�
	//����
	std::unique_ptr<BirdBody> body;
	//��
	std::array<std::unique_ptr<BirdWing>, 2> wings;
	//���U�t�F�[�Y
	LifePhase lifePhase = LifePhase::Born;
	//�s���^�C�}�[
	int16_t actionTimer = 0;
	//���S�t���O
	bool isDead = false;
};