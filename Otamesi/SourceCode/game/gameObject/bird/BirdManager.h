#pragma once
#include "bird.h"
#include <list>

/// <summary>
/// ���Ǘ�
/// </summary>
class BirdManager
{
	/// <summary>
	/// ��s���s���ꏊ
	/// </summary>
	enum FlyPosition
	{
		Forward,
		Away,
		Left,
		Right,

		PosNum,
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="bodyModel">���̃��f��</param>
	/// <param name="wingModel">�����f��</param>
	/// <param name="createInterval">�����Ԋu</param>
	/// <returns>���Ǘ�</returns>
	static BirdManager* Create(ObjModel* bodyModel, ObjModel* wingModel, const int32_t& createInterval);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void CreateBird();
	
private: //�����o�ϐ�
	//��
	std::list<std::unique_ptr<Bird>> birds;
	//���̃��f��
	ObjModel* bodyModel;
	//�����f��
	ObjModel* wingModel;
	//�����^�C�}�[
	int32_t createTimer;
	//�����Ԋu
	int32_t createInterval;
};