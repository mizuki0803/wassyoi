#pragma once
#include "Vector3.h"
#include <vector>

/// <summary>
/// �X�v���C�����
/// </summary>
class Spline
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// n + 2�̓_���g���āAP1�`Pn�̊Ԃ��X�v���C����Ԃ���
	/// </summary>
	/// <param name="points">����_�̏W��</param>
	/// <param name="startIndex">��Ԃ����Ԃ̓Y����</param>
	/// <param name="t">���Ԍo�ߗ�</param>
	/// <returns></returns>
	static Vector3 splinePosition(const std::vector<Vector3>& points, int startIndex, float t);
};