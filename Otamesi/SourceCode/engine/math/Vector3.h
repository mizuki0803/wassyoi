#pragma once
#include <DirectXMath.h>

class Vector3
{
public:
	float x;	//x����
	float y;	//y����
	float z;	//z����

public: //�R���X�g���N�^
	Vector3();					//��x�N�g���Ƃ��Đ���
	Vector3(float x, float y, float z);	//x�����Ay�����Az�������w�肵�Ă̐���
	Vector3(const Vector3& vec3);
	Vector3(const float* array);

public:	//�ÓI�����o�֐�
	/// <summary>
	/// ���[�J���ړ��ł̃��[���h���W�擾
	/// </summary>
	/// <param name="distance">���S����̋���</param>
	/// <param name="m">��ƂȂ�s��</param>
	/// <returns>���[�J���ړ��ł̃��[���h���W</returns>
	static const Vector3 LocalTranslation(const Vector3& distance, const DirectX::XMMATRIX& m); 


	/// <summary>
	/// �����̊p�x�擾
	/// </summary>
	/// <param name="vec">���x�x�N�g��</param>
	/// <param name="isMinusYRotaFix">y���p�x�����̐��̏ꍇ���̐��ɏC�����邩</param>
	/// <returns>�����̊p�x</returns>
	static const Vector3 VelocityRotate(const Vector3& vec, const bool isMinusYRotaFix = false);
	
	/// <summary>
	/// 2�_�Ԃ̊p�x�擾
	/// </summary>
	/// <param name="v1">�_1</param>
	/// <param name="v2">�_2</param>
	/// <param name="isMinusYRotaFix">y���p�x�����̐��̏ꍇ���̐��ɏC�����邩</param>
	/// <returns>2�_�Ԃ̊p�x</returns>
	static const Vector3 BetweenPointRotate(const Vector3& v1, const Vector3& v2, const bool isMinusYRotaFix = false);

public: //�����o�֐�
	float length() const;					//�m����(����)�����߂�
	Vector3& normalize();					//���K������
	float dot(const Vector3& v) const;		//���ς����߂�
	Vector3 cross(const Vector3& v) const;	//�O�ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

//2�����Z�q�I�[�o�[���[�h
//�������Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���B
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);
const Vector3 MatrixTransformPosition(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 MatrixTransformDirection(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 MatrixTransformWDivision(const Vector3& v, const DirectX::XMMATRIX& m);
