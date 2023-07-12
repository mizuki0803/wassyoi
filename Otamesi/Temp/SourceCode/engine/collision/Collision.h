#pragma once
#include "CollisionShape.h"
#include "Vector3.h"
#include "Vector2.h"

/// <summary>
/// �Փ˔���
/// </summary>
class Collision
{
public:
	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="posA">�ՓˑΏ�A�̍��W</param>
	/// <param name="posB">�ՓˑΏ�B�̍��W</param>
	/// <param name="radiusA">�ՓˑΏ�A�̔��a</param>
	/// <param name="radiusB">�ՓˑΏ�B�̔��a</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphereToSphere(const Vector3& posA, const Vector3& posB, const float radiusA, const float radiusB);

	/// <summary>
	/// �~�Ɖ~�̓����蔻��
	/// </summary>
	/// <param name="posA">�ՓˑΏ�A�̍��W</param>
	/// <param name="posB">�ՓˑΏ�B�̍��W</param>
	/// <param name="radiusA">�ՓˑΏ�A�̔��a</param>
	/// <param name="radiusB">�ՓˑΏ�B�̔��a</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckCircleToCircle(const Vector2& posA, const Vector2& posB, const float radiusA, const float radiusB);

	/// <summary>
	/// �l�p�Ǝl�p�̓����蔻��
	/// </summary>
	/// <param name="posA">�ՓˑΏ�A�̍��W</param>
	/// <param name="posB">�ՓˑΏ�B�̍��W</param>
	/// <param name="radiusA">�ՓˑΏ�A�̑傫��</param>
	/// <param name="radiusB">�ՓˑΏ�B�̑傫��</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSquareToSquare(const Vector2& posA, const Vector2& posB, const Vector2& sizeA, const Vector2& sizeB);


	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphere1">��1</param>
	/// <param name="sphere2">��2</param>
	/// <param name="inter">��_(���ʏ�̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_(���ʏ�̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_(�o�͗p)</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_(�O�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���Ɩ@���t���l�p�`�̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�l�p�`</param>
	/// <param name="inter">��_(�l�p�`��̍ŋߐړ_)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Square(const Sphere& sphere, const Square& square,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="plane">����</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="ray">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="distance">����(�o�͗p)</param>
	/// <param name="inter">��_(�o�͗p)</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
};