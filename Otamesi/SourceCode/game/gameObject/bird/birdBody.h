#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ��(����)
/// </summary>
class BirdBody : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">���W</param>
	/// <param name="velocity">�ړ����x</param>
	/// <returns>��(����)</returns>
	static BirdBody* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// ��s�ړ�
	/// </summary>
	void FlyMove();

private: //�����o�ϐ�
	//�ړ����x
	Vector3 velocity;
};