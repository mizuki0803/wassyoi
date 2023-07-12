#include "Camera.h"
#include "WindowApp.h"


void Camera::Initialize()
{
	//カメラアングルの更新
	UpdateAngle();
	//ビュー行列の生成
	UpdateMatView();
	//透視投影による射影行列の生成
	UpdateMatProjection();
}

void Camera::Update()
{
	if (dirtyAngle || dirtyView) {
		if (dirtyAngle) {
			//カメラアングルの更新
			UpdateAngle();
			//フラグを下げる
			dirtyAngle = false;
		}

		//ビュー行列の生成
		UpdateMatView();
		//フラグを下げる
		dirtyView = false;
	}

	if (dirtyProjection) {
		//透視投影による射影行列の生成
		UpdateMatProjection();
		//フラグを下げる
		dirtyProjection = false;
	}
}

void Camera::UpdateAngle()
{
	//視点回転
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(angle.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(angle.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(angle.y));

	XMVECTOR targetEyeV = { 0.0f, 0.0f, -distance, 1.0f };
	targetEyeV = XMVector3TransformNormal(targetEyeV, matRot);

	//カメラはtargetを追従する
	XMVECTOR targetV = { target.x, target.y, target.z, 0 };
	XMVECTOR eyeV = targetV + targetEyeV;

	//計算したeyeVをeyeにセットする
	SetEye({ eyeV.m128_f32[0], eyeV.m128_f32[1], eyeV.m128_f32[2] });
}

void Camera::UpdateMatView()
{
	//視点座標
	XMVECTOR eyePosition = XMLoadFloat3(&XMFLOAT3(eye.x, eye.y, eye.z));
	//注視店座標
	XMVECTOR targetPosition = XMLoadFloat3(&XMFLOAT3(target.x, target.y, target.z));
	//(仮の)上方向
	XMVECTOR upVector = XMLoadFloat3(&XMFLOAT3(up.x, up.y, up.z));

	//カメラのZ軸(視点方向)
	XMVECTOR cameraAxisZ = XMVectorSubtract(targetPosition, eyePosition);

	//0ベクトルだと向きが定まらないので除外
	assert(!XMVector3Equal(cameraAxisZ, XMVectorZero()));
	assert(!XMVector3IsInfinite(cameraAxisZ));
	assert(!XMVector3Equal(upVector, XMVectorZero()));
	assert(!XMVector3IsInfinite(upVector));

	//ベクトルを正規化
	cameraAxisZ = XMVector3Normalize(cameraAxisZ);

	//カメラのX軸(右方向)
	XMVECTOR cameraAxisX;

	//X軸は上方向→Z軸の外積で求まる
	cameraAxisX = XMVector3Cross(upVector, cameraAxisZ);

	//ベクトルを正規化
	cameraAxisX = XMVector3Normalize(cameraAxisX);

	//カメラのX軸(上方向)
	XMVECTOR cameraAxisY;

	//X軸はZ軸→X軸の外積で求まる
	cameraAxisY = XMVector3Cross(cameraAxisZ, cameraAxisX);

	//カメラ回転行列
	XMMATRIX matCameraRot;
	//カメラ座標系→ワールド座標系の変換行列
	matCameraRot.r[0] = cameraAxisX;
	matCameraRot.r[1] = cameraAxisY;
	matCameraRot.r[2] = cameraAxisZ;
	matCameraRot.r[3] = XMVectorSet(0, 0, 0, 1);

	//転置により逆行列(逆回転)を計算
	matView = XMMatrixTranspose(matCameraRot);

	//視点座標に-1を掛けた座標
	XMVECTOR reverseEyePosition = XMVectorNegate(eyePosition);
	//カメラの位置からワールド原点へのベクトル(カメラ座標系)
	XMVECTOR tX = XMVector3Dot(matCameraRot.r[0], reverseEyePosition); //X成分
	XMVECTOR tY = XMVector3Dot(matCameraRot.r[1], reverseEyePosition); //Y成分
	XMVECTOR tZ = XMVector3Dot(matCameraRot.r[2], reverseEyePosition); //Z成分
	//一つのベクトルにまとめる
	XMVECTOR translation = XMVectorSet(tX.m128_f32[0], tY.m128_f32[1], tZ.m128_f32[2], 1.0f);

	//ビュー行列に平行移動成分を設定
	matView.r[3] = translation;

#pragma region 全方向ビルボード行列の計算
	//ビルボード行列
	matBillboard.r[0] = cameraAxisX;
	matBillboard.r[1] = cameraAxisY;
	matBillboard.r[2] = cameraAxisZ;
	matBillboard.r[3] = XMVectorSet(0, 0, 0, 1);
#pragma region

#pragma region Y軸回りビルボード行列の計算
	//カメラX軸、Y軸、Z軸
	XMVECTOR ybillCameraAxisX, ybillCameraAxisY, ybillCameraAxisZ;

	//X軸は共通
	ybillCameraAxisX = cameraAxisX;
	//Y軸はワールド座標系のy軸
	ybillCameraAxisY = XMVector3Normalize(upVector);
	//Z軸はX軸→Y軸の外積で求まる
	ybillCameraAxisZ = XMVector3Cross(ybillCameraAxisX, ybillCameraAxisY);

	//Y軸回りビルボード行列
	matBillboardY.r[0] = ybillCameraAxisX;
	matBillboardY.r[1] = ybillCameraAxisY;
	matBillboardY.r[2] = ybillCameraAxisZ;
	matBillboardY.r[3] = XMVectorSet(0, 0, 0, 1);
}

void Camera::UpdateMatProjection()
{
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),				//上下画角60度
		(float)WindowApp::window_width / WindowApp::window_height,	//アスペクト比（画面横幅 / 画面縦幅）
		0.1f, 2000.0f							//前端、奥端
	);
}

void Camera::MoveVector(const Vector3& move)
{
	//視点と注視点を同じ量動かす
	MoveEyeVector(move);
	MoveTargetVector(move);
}

void Camera::MoveEyeVector(const Vector3& move)
{
	Vector3 eye_moved = GetEye();

	eye_moved += move;

	SetEye(eye_moved);
}

void Camera::MoveTargetVector(const Vector3& move)
{
	Vector3 target_moved = GetTarget();

	target_moved += move;

	SetTarget(target_moved);
}
