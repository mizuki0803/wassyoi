#include "Vector3.h"
#include <cmath>	//sqrt

using namespace DirectX;

Vector3::Vector3()
	:Vector3(0, 0, 0)
{
}

Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

Vector3::Vector3(const Vector3& vec3)
	: x(vec3.x), y(vec3.y), z(vec3.z)
{
}

Vector3::Vector3(const float* array)
	: x(array[0]), y(array[1]), z(array[2])
{
}

const Vector3 Vector3::LocalTranslation(const Vector3& distance, const DirectX::XMMATRIX& m)
{
	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(distance.x, distance.y, distance.z);
	//ワールド行列の合成
	XMMATRIX matWorld = {};
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
	//自機オブジェクトのワールド行列をかける
	matWorld *= m;

	//座標を取得
	return { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
}

const Vector3 Vector3::VelocityRotate(const Vector3& vec, const bool isMinusYRotaFix)
{
	Vector3 rota = {};
	rota.y = XMConvertToDegrees(std::atan2(vec.x, vec.z));
	//Y軸角度が負の数なのを修正する場合
	if (isMinusYRotaFix) {
		//負の数ならば角度修正
		if (rota.y <= 0) {
			rota.y += 360;
		}
	}
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rota.y));
	Vector3 distanceVecZ = MatrixTransformDirection(vec, matRot);
	rota.x = XMConvertToDegrees(std::atan2(-distanceVecZ.y, distanceVecZ.z));

	return rota;
}

const Vector3 Vector3::BetweenPointRotate(const Vector3& v1, const Vector3& v2, const bool isMinusYRotaFix)
{
	const Vector3 distanceVec = v1 - v2;

	return VelocityRotate(distanceVec, isMinusYRotaFix);
}

float Vector3::length() const
{
	return (float)sqrt(x * x + y * y + z * z);
}

Vector3& Vector3::normalize()
{
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross(const Vector3& v) const
{
	Vector3 temp;
	temp.x = this->y * v.z - this->z * v.y;
	temp.y = this->z * v.x - this->x * v.z;
	temp.z = this->x * v.y - this->y * v.x;
	return temp;
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	return v * s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

const Vector3 MatrixTransformPosition(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w,
	};

	return result;
}

const Vector3 MatrixTransformDirection(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2]) / w,
	};

	return result;
}

const Vector3 MatrixTransformWDivision(const Vector3& v, const DirectX::XMMATRIX& m)
{
	float w = v.x * m.r[0].m128_f32[3] + v.y * m.r[1].m128_f32[3] + v.z * m.r[2].m128_f32[3] + m.r[3].m128_f32[3];

	Vector3 result{
		(v.x * m.r[0].m128_f32[0] + v.y * m.r[1].m128_f32[0] + v.z * m.r[2].m128_f32[0] + m.r[3].m128_f32[0]) / w,
		(v.x * m.r[0].m128_f32[1] + v.y * m.r[1].m128_f32[1] + v.z * m.r[2].m128_f32[1] + m.r[3].m128_f32[1]) / w,
		(v.x * m.r[0].m128_f32[2] + v.y * m.r[1].m128_f32[2] + v.z * m.r[2].m128_f32[2] + m.r[3].m128_f32[2]) / w,
	};

	result /= result.z;

	return result;
}
