#pragma once
#include "Vector3.h"
#include "Vector2.h"

class Easing
{
public:
	//���������^��
	static float LerpFloat(const float start, const float end, const float time);
	static Vector3 LerpVec3(const Vector3& v1, const Vector3& v2, float time);
	static Vector2 LerpVec2(const Vector2& v1, const Vector2& v2, float time);

	//���ʐ��`���
	static Vector3 Slerp(const Vector3& v1, const Vector3& v2, float time);

	//sin
	static float InSine(const float start, const float end, const float time);
	static float OutSine(const float start, const float end, const float time);
	static float InOutSine(const float start, const float end, const float time);

	//2��
	static float InQuad(const float start, const float end, const float time);
	static float OutQuad(const float start, const float end, const float time);
	static float InOutQuad(const float start, const float end, const float time);

	//3��
	static float InCubic(const float start, const float end, const float time);
	static float OutCubic(const float start, const float end, const float time);
	static float InOutCubic(const float start, const float end, const float time);

	//4��
	static float InQuart(const float start, const float end, const float time);
	static float OutQuart(const float start, const float end, const float time);
	static float InOutQuart(const float start, const float end, const float time);

	//5��
	static float InQuint(const float start, const float end, const float time);
	static float OutQuint(const float start, const float end, const float time);
	static float InOutQuint(const float start, const float end, const float time);

	//Expo
	static float InExpo(const float start, const float end, const float time);
	static float OutExpo(const float start, const float end, const float time);
	static float InOutExpo(const float start, const float end, const float time);

	//Circ
	static float InCirc(const float start, const float end, const float time);
	static float OutCirc(const float start, const float end, const float time);
	static float InOutCirc(const float start, const float end, const float time);

	//Back
	static float InBack(const float start, const float end, const float time);
	static float OutBack(const float start, const float end, const float time);
	static float InOutBack(const float start, const float end, const float time);

	//Elastic
	static float InElastic(const float start, const float end, const float time);
	static float OutElastic(const float start, const float end, const float time);
	static float InOutElastic(const float start, const float end, const float time);

	//Bounce
	static float InBounce(const float start, const float end, const float time);
	static float OutBounce(const float start, const float end, const float time);
	static float InOutBounce(const float start, const float end, const float time);

private:
	/// <summary>
	/// Bounce�v�Z�p
	/// </summary>
	/// <param name="time">�C�[�W���O����(0�`1)</param>
	/// <returns>�C�[�W���O�v�Z����</returns>
	static float OutBounceCalculate(float time);
};