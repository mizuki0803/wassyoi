#include "GameCamera.h"
#include "Input.h"
#include "Easing.h"
#include "Player.h"
#include "Audio.h"

const float GameCamera::rotate3DDistance = 2.0f;

const DirectX::XMMATRIX GameCamera::matProj2D = XMMatrixOrthographicOffCenterLH(
	-(float)WindowApp::window_width / 18.0f, (float)WindowApp::window_width / 18.0f,
	-WindowApp::window_height / 18.0f, WindowApp::window_height / 18.0f,
	0.0f, 2000.0f
);

const DirectX::XMMATRIX GameCamera::matProj3D = XMMatrixPerspectiveFovLH(
	XMConvertToRadians(60.0f),
	(float)WindowApp::window_width / WindowApp::window_height,
	0.1f, 2000.0f
);

GameCamera* GameCamera::Create(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos)
{
	//インスタンス生成
	GameCamera* instance = new GameCamera();

	// 初期化
	instance->Initialize(distanceStageCenter, stageCenterPos);

	return instance;
}

void GameCamera::Initialize(const XMFLOAT3& distanceStageCenter, const Vector3& stageCenterPos)
{
	//カメラ初期化
	Camera::Initialize();

	//ステージ中央からの距離をセット
	this->distanceStageCenter = distanceStageCenter;
	//ステージの中央座標をセット
	this->stageCenterPos = stageCenterPos;

	// 最初に動かす関数の設定
	phase_ = static_cast<int>(GamePhase::Play);
	// イージングの初期化
	easeData_ = std::make_unique<EaseData>(29);
	reStartEaseData_ = std::make_unique<EaseData>(29);
	//関数の設定
	CreateAct();

	//初期の回転角をセット
	rotation.x = rotate3DDistance;
	//カメラ位置フェーズを更新する
	CameraPosPhaseCheck();
}

void GameCamera::Update()
{
	func_[phase_]();
}

void GameCamera::PlayGame()
{
	//ステージクリア状態なら抜ける
	if (isStageClear || menuFlag_ || isCreateMove_ || hintFlag_) { return; }

	//トリガーフラグがtrue状態ならばfalseに直しておく
	if (isTriggerDimensionChange) { isTriggerDimensionChange = false; }

	//カメラ回転開始
	RotateStart();
	//カメラ回転
	Rotate();

	//次元切り替え
	ChanegeDimension();

	//座標更新
	position = UpdatePosition();

	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//視点、注視点を更新
	UpdateEyeTarget();
	//ビュー行列と射影行列の更新
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }
}

void GameCamera::GameStart()
{
}

void GameCamera::SetClearMode()
{
	//クリア状態にする
	isStageClear = true;

	//既に3Dの場合は何もしない状態にして抜ける
	if (!is2D) { 
		phase_ = static_cast<int>(GamePhase::None);
		return; 
	}

	//回転前回転角をセット
	rotateBefore = rotation;

	//回転後回転角をセット
	if (is2D) {
		rotateAfter = { rotation.x + rotate3DDistance, rotation.y, rotation.z };
	}
	else {
		rotateAfter = { rotation.x - rotate3DDistance, rotation.y, rotation.z };
	}
	dirtyProjection = true;

	//アクション用タイマーを初期化しておく
	actionTimer = 0;

	//クリア時に3次元に戻す状態にする
	phase_ = static_cast<int>(GamePhase::ClearReturn3D);
}

void GameCamera::ClearReturn3D()
{
	//タイマー更新
	actionTimer++;
	const float rotTime = 40; //次元切り替え回転にかかる時間

	//イージングに使用する変数(0～1を算出)
	const float time = actionTimer / rotTime;

	//回転させる
	rotation.x = Easing::OutCubic(rotateBefore.x, rotateAfter.x, time);
	rotation.y = Easing::OutCubic(rotateBefore.y, rotateAfter.y, time);
	rotation.z = Easing::OutCubic(rotateBefore.z, rotateAfter.z, time);
	//座標更新
	position = UpdatePosition();

	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//視点、注視点を更新
	UpdateEyeTarget();
	//ビュー行列と射影行列の更新
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }

	//プロジェクション行列のイージング
	if (is2D) {
		matProjection = Ease4x4_out(matProj2D, matProj3D, time);
	}

	//タイマーが指定した時間に満たなければ抜ける
	if (actionTimer < rotTime) { return; }

	//2D状態かフラグを切り替える
	if (is2D) { is2D = false; }
	else { is2D = true; }

	dirtyProjection = true;

	//次元に変更が完了したトリガーを立てる
	isTriggerDimensionChange = true;

	//行動を「何もしない」に戻す
	phase_ = static_cast<int>(GamePhase::None);
}

void GameCamera::ClearReturnRotate()
{
	easeData_->SetCount(29);
	//次のステージ開始のため、正面を向くようにイージングで回転させる
	rotation.x = Easing::InCubic(rotateBefore.x, rotateAfter.x, easeData_->GetTimeRate());
	rotation.y = Easing::InCubic(rotateBefore.y, rotateAfter.y, easeData_->GetTimeRate());
	rotation.z = Easing::InCubic(rotateBefore.z, rotateAfter.z, easeData_->GetTimeRate());
	//正式な座標を算出
	position = UpdatePosition();

	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//視点、注視点を更新
	UpdateEyeTarget();
	//ビュー行列と射影行列の更新
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }

	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		easeData_->SetCount(39);
		phase_ = static_cast<int>(GamePhase::None);
	}

	easeData_->Update();
}

void GameCamera::GameReStart()
{	
	Vector3 moveNum;
	if (!reStartEaseChangeFlag_)
	{
		moveNum.x = Easing::OutQuint(0.0f, -28.0f, reStartEaseData_->GetTimeRate());

		if (reStartEaseData_->GetEndFlag())
		{
			reStartEaseData_->Reset();
			reStartEaseChangeFlag_ = true;
		}
	}
	else
	{
		moveNum.x = Easing::InQuint(-28.0f, 0.0f, reStartEaseData_->GetTimeRate());
	}

	//回転角から計算した座標に移動量を加えて正式な座標を算出
	position = moveNum + UpdatePosition();

	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//視点、注視点を更新
	UpdateEyeTarget();
	//ビュー行列と射影行列の更新
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }

	//ステージを変更するので、ステージとカメラの距離をイージングで変更していく
	distanceStageCenter.x = Easing::InCubic(beforeDistanceStageCenter.x, afterDistanceStageCenter.x, easeData_->GetTimeRate());
	distanceStageCenter.y = Easing::InCubic(beforeDistanceStageCenter.y, afterDistanceStageCenter.y, easeData_->GetTimeRate());
	distanceStageCenter.z = Easing::InCubic(beforeDistanceStageCenter.z, afterDistanceStageCenter.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag() && reStartEaseData_->GetEndFlag() && reStartEaseChangeFlag_)
	{
		easeData_->Reset();
		reStartEaseData_->Reset();
		phase_ = static_cast<int>(GamePhase::None);
	}

	reStartEaseData_->Update();
	easeData_->Update();
}

void GameCamera::StayGame()
{
	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//視点、注視点を更新
	UpdateEyeTarget();
	//ビュー行列と射影行列の更新
	UpdateMatView();
	if (dirtyProjection) { UpdateMatProjection(); }
}

void GameCamera::CreateAct()
{
	func_.push_back([this] { return GameStart(); });
	func_.push_back([this] { return PlayGame(); });
	func_.push_back([this] { return ClearReturn3D(); });
	func_.push_back([this] { return ClearReturnRotate(); });
	func_.push_back([this] { return GameReStart(); });
	func_.push_back([this] { return StayGame(); });
}

void GameCamera::ChanegeDimensionStart()
{
	//回転前回転角をセット
	rotateBefore = rotation;

	//回転後回転角をセット
	if (is2D) {
		rotateAfter = { rotation.x + rotate3DDistance, rotation.y, rotation.z };
	}
	else {
		rotateAfter = { rotation.x - rotate3DDistance, rotation.y, rotation.z };
	}
	dirtyProjection = true;

	//アクション用タイマーを初期化しておく
	actionTimer = 0;

	//行動を「次元切り替え」にする
	actionPhase = ActionPhase::ChangeDimension;

	if (is2D) {
		Audio::GetInstance()->PlayWave(Audio::SoundName::d2_d3);
	} else {
		Audio::GetInstance()->PlayWave(Audio::SoundName::d3_d2);
	}
}

void GameCamera::SetClearResetAround()
{
	phase_ = static_cast<int>(GamePhase::ClearReturnRotate);

	//回転前回転角をセット
	rotateBefore = rotation;
	//回転後回転角をセット(0または360に近いほうに)
	const float aroundMax = 360;
	if (rotation.x - rotate3DDistance < aroundMax / 2) { rotateAfter.x = rotate3DDistance; }
	else { rotateAfter.x = aroundMax + rotate3DDistance; }
	if (rotation.y < aroundMax / 2) { rotateAfter.y = 0; }
	else { rotateAfter.y = aroundMax; }
	if (rotation.z < aroundMax / 2) { rotateAfter.z = 0; }
	else { rotateAfter.z = aroundMax; }
}

void GameCamera::SetReCreateMove(const XMFLOAT3& distanceStageCenter)
{
	// 保存する座標の更新
	beforeDistanceStageCenter = this->distanceStageCenter;
	afterDistanceStageCenter = distanceStageCenter;
	phase_ = static_cast<int>(GamePhase::Play);
	is2D = false;
	isTriggerDimensionChange = false;
	cameraEaseChangeFlag_ = false;
	reStartEaseChangeFlag_ = false;
	isStageClear = false;
	easeData_->Reset();
	reStartEaseData_->Reset();
	phase_ = static_cast<int>(GamePhase::ReStart);
}

void GameCamera::Reset()
{
	cameraXPosPhase = (int)CameraXPosPhase::Front;
	cameraYPosPhase = (int)CameraYPosPhase::Side;
	phase_ = static_cast<int>(GamePhase::Play);
	is2D = false;
	isTriggerDimensionChange = false;
	cameraEaseChangeFlag_ = false;
	reStartEaseChangeFlag_ = false;
	isStageClear = false;
	easeData_->Reset();
	reStartEaseData_->Reset();
}

void GameCamera::UpdateMatProjection()
{
	if (actionPhase == ActionPhase::ChangeDimension) { return; }

	//2D状態の場合は平行投影
	if (is2D) {
		matProjection = matProj2D;
	}
	//3D状態の場合は透視投影
	else {
		matProjection = matProj3D;
	}
}

void GameCamera::UpdateMatWorld(const XMMATRIX& matTrans)
{
	//回転　
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//子である自機用のワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
}

void GameCamera::UpdateEyeTarget()
{
	//視点をワールド座標に設定
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//カメラの回転を反映させる
	forward = MatrixTransformDirection(forward, matWorld);
	//視点から前方に進んだ位置を注視点に設定
	target = eye + forward;
	//天地が反転してもいいように上方向ベクトルも回転させる
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);
}

Vector3 GameCamera::UpdatePosition()
{
	//X,Y回転角をラジアンに直す
	const float angleX = XMConvertToRadians(rotation.x);
	const float angleY = XMConvertToRadians(rotation.y);
	//アンダーフローする可能性があるので、小数点を切り捨てる
	const float divNum = 1000;
	const float roundAngleX = floorf(angleX * divNum) / divNum;
	const float roundAngleY = floorf(angleY * divNum) / divNum;

	//X,Yラジアンを使用し、sin,cosを算出
	const float sinfAngleY = sinf(roundAngleY);
	const float cosfAngleY = cosf(roundAngleY);
	const float sinfAngleX = sinf(roundAngleX);
	const float cosfAngleX = cosf(roundAngleX);

	//計算結果を割り当てて座標をセット
	//Y座標はX回転角のsinを使用
	//X,Z座標はY回転角のsin,cosで計算し、X回転角(Y座標)のcosを乗算して算出
	Vector3 pos;
	pos.x = (-sinfAngleY * cosfAngleX) * distanceStageCenter.x + stageCenterPos.x;
	pos.y = sinfAngleX * distanceStageCenter.y + stageCenterPos.y;
	pos.z = (-cosfAngleY * cosfAngleX) * distanceStageCenter.z + stageCenterPos.z;

	return pos;
}

Vector3 GameCamera::InputRotateNum()
{
	//回転角に加算する値
	Vector3 addRot;
	//一度の回転で回る量
	const float rotNum = 90;

	//キー入力による回る量を設定
	if (Input::GetInstance()->PushKey(DIK_UP)) { addRot = { rotNum, 0, 0 }; }
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) { addRot = { -rotNum, 0, 0 }; }
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) { addRot = { 0, rotNum, 0 }; }
	else if (Input::GetInstance()->PushKey(DIK_RIGHT)) { addRot = { 0, -rotNum, 0 }; }

	//縦軸カメラ位置フェーズが逆さの横またはならば左右が逆になるので反転させておく
	if (cameraYPosPhase == CameraYPosPhase::ReverseSide || cameraYPosPhase == CameraYPosPhase::Buttom) { addRot.y = -addRot.y; }

	//現在の回転角に加算する量を足した値を返す
	return rotation + addRot;
}

void GameCamera::RotateStart()
{
	//プレイヤーの行動が「何もしない」以外なら抜ける
	if (!(player->GetActionPhase() == Player::ActionPhase::None)) { return; }
	//行動が「何もしない」以外なら抜ける
	if (!(actionPhase == ActionPhase::None)) { return; }
	//2D状態なら抜ける
	if (is2D) { return; }

	//入力がなければ抜ける
	if (!(Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_DOWN) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_RIGHT))) { return; }

	//回転前回転角をセット
	rotateBefore = rotation;
	//回転後回転角をセット
	rotateAfter = InputRotateNum();

	//アクション用タイマーを初期化しておく
	actionTimer = 0;

	//行動を「カメラ回転」にする
	actionPhase = ActionPhase::Rotation;
}

void GameCamera::Rotate()
{
	//行動が「カメラ回転」以外なら抜ける
	if (!(actionPhase == ActionPhase::Rotation)) { return; }

	//タイマー更新
	actionTimer++;
	//回転イージングに使用する変数(0～1を算出)
	const float time = (float)actionTimer / rotateTime;

	//回転させる
	rotation.x = Easing::OutQuart(rotateBefore.x, rotateAfter.x, time);
	rotation.y = Easing::OutQuart(rotateBefore.y, rotateAfter.y, time);
	rotation.z = Easing::OutQuart(rotateBefore.z, rotateAfter.z, time);

	//回転角が0～360以内に収まるように調整
	MaxMinRotate(rotation.x);
	MaxMinRotate(rotation.y);
	MaxMinRotate(rotation.z);

	//タイマーが指定した時間に満たなければ抜ける
	if (actionTimer < rotateTime) { return; }

	//カメラ位置フェーズを更新する
	CameraPosPhaseCheck();

	//行動を「何もしない」に戻す
	actionPhase = ActionPhase::None;
}

void GameCamera::MaxMinRotate(float& rotation)
{
	//0～360以内に収まるように調整
	const float rotMax = 360.0f;
	if (rotation >= rotMax) {
		rotation -= rotMax;
	}
	else if (rotation < 0) {
		rotation += rotMax;
	}
}

void GameCamera::CameraPosPhaseCheck()
{
	//3Dでの視点時にカメラを傾ける量を除いた角度
	const float rotX = rotation.x - rotate3DDistance;

	//X軸回転による縦軸カメラ位置フェーズを設定
	if ((int)(rotX) == 0) { cameraYPosPhase = CameraYPosPhase::Side; }
	else if ((int)(rotX) == 90) { cameraYPosPhase = CameraYPosPhase::Top; }
	else if ((int)(rotX) == 180) { cameraYPosPhase = CameraYPosPhase::ReverseSide; }
	else if ((int)(rotX) == 270) { cameraYPosPhase = CameraYPosPhase::Buttom; }
	else { assert(0); }

	//Y軸回転による横軸カメラ位置フェーズを設定
	if ((int)(rotX) == 0 || (int)(rotX) == 90 || (int)(rotX) == 270) {
		if ((int)rotation.y == 0) { cameraXPosPhase = CameraXPosPhase::Front; }
		else if ((int)rotation.y == 90) { cameraXPosPhase = CameraXPosPhase::Left; }
		else if ((int)rotation.y == 180) { cameraXPosPhase = CameraXPosPhase::Back; }
		else if ((int)rotation.y == 270) { cameraXPosPhase = CameraXPosPhase::Right; }
		else { assert(0); }
	}
	else if ((int)(rotX) == 180) {//X軸回転で180度を向いているときのみは上下が逆になる
		if ((int)rotation.y == 0) { cameraXPosPhase = CameraXPosPhase::Back; }
		else if ((int)rotation.y == 90) { cameraXPosPhase = CameraXPosPhase::Right; }
		else if ((int)rotation.y == 180) { cameraXPosPhase = CameraXPosPhase::Front; }
		else if ((int)rotation.y == 270) { cameraXPosPhase = CameraXPosPhase::Left; }
		else { assert(0); }
	}
	else {
		assert(0);
	}
}

void GameCamera::ChanegeDimension()
{
	//行動が「次元切り替え」以外なら抜ける
	if (!(actionPhase == ActionPhase::ChangeDimension)) { return; }

	//タイマー更新
	actionTimer++;
	const float rotTime = 40; //次元切り替え回転にかかる時間

	//イージングに使用する変数(0～1を算出)
	const float time = actionTimer / rotTime;

	//回転させる
	rotation.x = Easing::OutCubic(rotateBefore.x, rotateAfter.x, time);
	rotation.y = Easing::OutCubic(rotateBefore.y, rotateAfter.y, time);
	rotation.z = Easing::OutCubic(rotateBefore.z, rotateAfter.z, time);

	//プロジェクション行列のイージング
	if (is2D) {
		matProjection = Ease4x4_out(matProj2D, matProj3D, time);
	}
	else {
		matProjection = Ease4x4_in(matProj3D, matProj2D, time);
	}

	//タイマーが指定した時間に満たなければ抜ける
	if (actionTimer < rotTime) { return; }

	//2D状態かフラグを切り替える
	if (is2D) { is2D = false; }
	else { is2D = true; }

	dirtyProjection = true;

	//次元に変更が完了したトリガーを立てる
	isTriggerDimensionChange = true;

	//行動を「何もしない」に戻す
	actionPhase = ActionPhase::None;
}

void GameCamera::SetEaseData(const int count)
{
	if (easeData_ == nullptr)
	{
		easeData_ = std::make_unique<EaseData>(count);
	}
	else
	{
		easeData_->SetCount(count);
	}
}

XMMATRIX GameCamera::Ease4x4_in(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer)
{
	//4x4に変換
	XMFLOAT4X4 a, b;
	XMStoreFloat4x4(&a, _mat1);
	XMStoreFloat4x4(&b, _mat2);

	//イージング
	XMFLOAT4X4 out4x4 = {};
	out4x4._11 = Easing::OutQuart(a._11, b._11, _timer);
	out4x4._12 = Easing::OutQuart(a._12, b._12, _timer);
	out4x4._13 = Easing::OutQuart(a._13, b._13, _timer);
	out4x4._14 = Easing::OutQuart(a._14, b._14, _timer);
	out4x4._21 = Easing::OutQuart(a._21, b._21, _timer);
	out4x4._22 = Easing::OutQuart(a._22, b._22, _timer);
	out4x4._23 = Easing::OutQuart(a._23, b._23, _timer);
	out4x4._24 = Easing::OutQuart(a._24, b._24, _timer);
	out4x4._31 = Easing::OutQuart(a._31, b._31, _timer);
	out4x4._32 = Easing::OutQuart(a._32, b._32, _timer);
	out4x4._33 = Easing::OutQuart(a._33, b._33, _timer);
	out4x4._34 = Easing::OutQuart(a._34, b._34, _timer);
	out4x4._41 = Easing::OutQuart(a._41, b._41, _timer);
	out4x4._42 = Easing::OutQuart(a._42, b._42, _timer);
	out4x4._43 = Easing::OutQuart(a._43, b._43, _timer);
	out4x4._44 = Easing::OutQuart(a._44, b._44, _timer);

	return XMLoadFloat4x4(&out4x4);
}

XMMATRIX GameCamera::Ease4x4_out(const XMMATRIX& _mat1, const XMMATRIX& _mat2, const float _timer)
{
	//4x4に変換
	XMFLOAT4X4 a, b;
	XMStoreFloat4x4(&a, _mat1);
	XMStoreFloat4x4(&b, _mat2);

	XMFLOAT4X4 out4x4 = {};
	out4x4._11 = Easing::InQuart(a._11, b._11, _timer);
	out4x4._12 = Easing::InQuart(a._12, b._12, _timer);
	out4x4._13 = Easing::InQuart(a._13, b._13, _timer);
	out4x4._14 = Easing::InQuart(a._14, b._14, _timer);
	out4x4._21 = Easing::InQuart(a._21, b._21, _timer);
	out4x4._22 = Easing::InQuart(a._22, b._22, _timer);
	out4x4._23 = Easing::InQuart(a._23, b._23, _timer);
	out4x4._24 = Easing::InQuart(a._24, b._24, _timer);
	out4x4._31 = Easing::InQuart(a._31, b._31, _timer);
	out4x4._32 = Easing::InQuart(a._32, b._32, _timer);
	out4x4._33 = Easing::InQuart(a._33, b._33, _timer);
	out4x4._34 = Easing::InQuart(a._34, b._34, _timer);
	out4x4._41 = Easing::InQuart(a._41, b._41, _timer);
	out4x4._42 = Easing::InQuart(a._42, b._42, _timer);
	out4x4._43 = Easing::InQuart(a._43, b._43, _timer);
	out4x4._44 = Easing::InQuart(a._44, b._44, _timer);

	return XMLoadFloat4x4(&out4x4);
}
