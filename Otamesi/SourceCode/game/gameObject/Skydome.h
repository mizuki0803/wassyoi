#pragma once
#include "ObjObject3d.h"

/// <summary>
/// �V��
/// </summary>
class Skydome : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�V��</returns>
	static Skydome* Create(ObjModel* model);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//setter
	void SetIsRotate(const bool isRotate) { this->isRotate = isRotate; }

private: //�����o�֐�
	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

private: //�ÓI�����o�ϐ�
	//�T�C�Y
	static const float size;
	//��]�X�s�[�h
	static const float rotSpeed;

private: //�����o�ϐ�
	//��]���邩
	bool isRotate = false;
};