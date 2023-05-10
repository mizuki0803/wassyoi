#pragma once
#include "Vector3.h"
#include <vector>

/// <summary>
/// スプライン補間
/// </summary>
class Spline
{
public: //静的メンバ関数
	/// <summary>
	/// n + 2個の点を使って、P1～Pnの間をスプライン補間する
	/// </summary>
	/// <param name="points">制御点の集合</param>
	/// <param name="startIndex">補間する区間の添え字</param>
	/// <param name="t">時間経過率</param>
	/// <returns></returns>
	static Vector3 splinePosition(const std::vector<Vector3>& points, int startIndex, float t);
};