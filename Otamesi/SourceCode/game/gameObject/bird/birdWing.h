#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ��(��)
/// </summary>
class BirdWing : public ObjObject3d
{
public: //enum
	//���̃^�C�v
	enum WingType
	{
		LeftWing,	//����
		RightWing,	//�E��
	};


public: //�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="parent">�e�I�u�W�F�N�g(������)</param>
	/// <param name="wingType">���̃^�C�v</param>
	/// <param name="position">���W</param>
	/// <returns>��(��)</returns>
	static BirdWing* Create(ObjModel* model, ObjObject3d* parent, WingType wingType, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �H�΂���
	/// </summary>
	void Flap();

private: //�����o�ϐ�
	//���̃^�C�v
	WingType wingType;
	//���̉�]�X�s�[�h
	float rotateSpeed = 0;
	//�����グ�Ă��邩
	bool isWingUp = true;
};