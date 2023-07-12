#pragma once
#include "CollisionShape.h"
#include "Vector3.h"
#include "Vector2.h"

/// <summary>
/// 衝突判定
/// </summary>
class Collision
{
public:
	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="posA">衝突対象Aの座標</param>
	/// <param name="posB">衝突対象Bの座標</param>
	/// <param name="radiusA">衝突対象Aの半径</param>
	/// <param name="radiusB">衝突対象Bの半径</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphereToSphere(const Vector3& posA, const Vector3& posB, const float radiusA, const float radiusB);

	/// <summary>
	/// 円と円の当たり判定
	/// </summary>
	/// <param name="posA">衝突対象Aの座標</param>
	/// <param name="posB">衝突対象Bの座標</param>
	/// <param name="radiusA">衝突対象Aの半径</param>
	/// <param name="radiusB">衝突対象Bの半径</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckCircleToCircle(const Vector2& posA, const Vector2& posB, const float radiusA, const float radiusB);

	/// <summary>
	/// 四角と四角の当たり判定
	/// </summary>
	/// <param name="posA">衝突対象Aの座標</param>
	/// <param name="posB">衝突対象Bの座標</param>
	/// <param name="radiusA">衝突対象Aの大きさ</param>
	/// <param name="radiusB">衝突対象Bの大きさ</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSquareToSquare(const Vector2& posA, const Vector2& posB, const Vector2& sizeA, const Vector2& sizeB);


	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphere1">球1</param>
	/// <param name="sphere2">球2</param>
	/// <param name="inter">交点(平面上の最近接点)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点(平面上の最近接点)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点(出力用)</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point,
		const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点(三角形上の最近接点)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 球と法線付き四角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">四角形</param>
	/// <param name="inter">交点(四角形上の最近接点)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Square(const Sphere& sphere, const Square& square,
		DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="ray">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="distance">距離(出力用)</param>
	/// <param name="inter">交点(出力用)</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
		float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);
};