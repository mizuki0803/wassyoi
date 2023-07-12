#include "LightCamera.h"
#include "WindowApp.h"

void LightCamera::Initialize(const Vector3& eye, const Vector3& target)
{
	//カメラ初期化
	Camera::Initialize();

	//視点、注視点をセット
	this->eye = eye;
	this->target = target;
	up = { 0, 1, 0 };
}

void LightCamera::UpdateMatProjection()
{
	matProjection = XMMatrixOrthographicOffCenterLH(
		projectionMin.x, projectionMax.x,
		projectionMin.y, projectionMax.y,
		0.1f, 2000.0f
	);
}
