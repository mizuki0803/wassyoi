#include "Spline.h"

Vector3 Spline::splinePosition(const std::vector<Vector3>& points, int startIndex, float t)
{
	//��Ԃ��ׂ��_�̐�
	int n = (int)points.size() - 2;

	if (n < 2) { return {}; }
	if (startIndex > n) { return points[n]; } //P0�̒l��Ԃ�
	if (startIndex < 1) { return points[1]; } //P1�̒l��Ԃ�

	// P0�`P3 �̐���_���擾���� �� P1�`P2 ���Ԃ���
	Vector3 p0 = points[startIndex - 1];
	Vector3 p1 = points[startIndex];
	Vector3 p2 = points[startIndex + 1];
	Vector3 p3 = points[startIndex + 2];

	//Catmull-Rom �̎��ɂ����
	Vector3 position =
		0.5f * (2 * p1 + (-p0 + p2) * t +
			(2 * p0 - 5 * p1 + 4 * p2 - p3) * t * t +
			(-p0 + 3 * p1 - 3 * p2 + p3) * t * t * t);


	return position;
}
