#pragma once
#include <DirectXMath.h>

class Vector3
{
public:
	float x;	//x成分
	float y;	//y成分
	float z;	//z成分

public: //コンストラクタ
	Vector3();					//零ベクトルとして生成
	Vector3(float x, float y, float z);	//x成分、y成分、z成分を指定しての生成
	Vector3(const Vector3& vec3);
	Vector3(const float* array);

public:	//静的メンバ関数
	/// <summary>
	/// ローカル移動でのワールド座標取得
	/// </summary>
	/// <param name="distance">中心からの距離</param>
	/// <param name="m">基準となる行列</param>
	/// <returns>ローカル移動でのワールド座標</returns>
	static const Vector3 LocalTranslation(const Vector3& distance, const DirectX::XMMATRIX& m); 


	/// <summary>
	/// 方向の角度取得
	/// </summary>
	/// <param name="vec">速度ベクトル</param>
	/// <param name="isMinusYRotaFix">y軸角度が負の数の場合正の数に修正するか</param>
	/// <returns>方向の角度</returns>
	static const Vector3 VelocityRotate(const Vector3& vec, const bool isMinusYRotaFix = false);
	
	/// <summary>
	/// 2点間の角度取得
	/// </summary>
	/// <param name="v1">点1</param>
	/// <param name="v2">点2</param>
	/// <param name="isMinusYRotaFix">y軸角度が負の数の場合正の数に修正するか</param>
	/// <returns>2点間の角度</returns>
	static const Vector3 BetweenPointRotate(const Vector3& v1, const Vector3& v2, const bool isMinusYRotaFix = false);

public: //メンバ関数
	float length() const;					//ノルム(長さ)を求める
	Vector3& normalize();					//正規化する
	float dot(const Vector3& v) const;		//内積を求める
	Vector3 cross(const Vector3& v) const;	//外積を求める

	//単項演算子オーバーロード
	Vector3 operator+() const;
	Vector3 operator-() const;

	//代入演算子オーバーロード
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);
};

//2項演算子オーバーロード
//※いろんな引数(引数の型と順序)のパターンに対応するため、以下のように準備している。
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);
const Vector3 MatrixTransformPosition(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 MatrixTransformDirection(const Vector3& v, const DirectX::XMMATRIX& m);
const Vector3 MatrixTransformWDivision(const Vector3& v, const DirectX::XMMATRIX& m);
