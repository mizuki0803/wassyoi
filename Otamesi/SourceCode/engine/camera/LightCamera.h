#pragma once
#include "Camera.h"
#include "Vector2.h"

class LightCamera : public Camera
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="eye">���_</param>
	/// <param name="target">�����_</param>
	void Initialize(const Vector3& eye, const Vector3& target = { 0, 0, 0 });

	/// <summary>
	/// �ˉe�s����X�V
	/// </summary>
	void UpdateMatProjection() override;

	//setter
	void SetEyeTarget(const Vector3& eye, const Vector3& target) {
		this->eye = eye;
		this->target = target;
		dirtyView = true;
	}
	void SetProjectionNum(const Vector2& projectionMax, const Vector2& projectionMin) {
		this->projectionMax = projectionMax;
		this->projectionMin = projectionMin;
		dirtyProjection = true;
	}

private: //�����o�ϐ�
	//�v���W�F�N�V�����̍ő�l
	Vector2 projectionMax = { 100, 100 };
	//�v���W�F�N�V�����̍ŏ��l
	Vector2 projectionMin = { -100, -100 };
};