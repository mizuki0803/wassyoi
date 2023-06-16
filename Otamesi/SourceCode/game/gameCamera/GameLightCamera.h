#pragma once
#include "LightCamera.h"

/// <summary>
/// �Q�[���Ɏg�p��������J����
/// </summary>
class GameLightCamera : public LightCamera
{
public: //�ÓI�����o�ϐ�
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="centerDistance">���S����̋���</param>
	/// <returns>�Q�[���Ɏg�p��������J����</returns>
	static GameLightCamera* Create(float centerDistance);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="centerDistance">���S����̋���</param>
	void Initialize(float centerDistance);

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// ���S�����ɉ�]
	/// </summary>
	void Around();

private: //�����o�ϐ�
	//���S����̋���
	float centerDistance;
	//��]�Ɏd�l�����]�p
	float aroundRotate = 0;
};