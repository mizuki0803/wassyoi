#include "GameCamera.h"
#include "Input.h"
#include "Easing.h"

GameCamera* GameCamera::Create(float distanceStageCenter, const Vector3& stageCenterPos)
{
	//インスタンス生成
	GameCamera* instance = new GameCamera();

	// 初期化
	instance->Initialize(distanceStageCenter, stageCenterPos);

	return instance;
}

void GameCamera::Initialize(float distanceStageCenter, const Vector3& stageCenterPos)
{
	//カメラ初期化
	Camera::Initialize();

	//ステージ中央からの距離をセット
	this->distanceStageCenter = distanceStageCenter;
	//ステージ中央座標をセット
	this->stageCenterPos = stageCenterPos;
	SetTarget(stageCenterPos);

	//ステージ中央の座標をカメラの基準にする
	for (int i = 0; i < (int)CameraYPosPhase::YPosPhaseNum; i++) {
		for (int j = 0; j < (int)CameraXPosPhase::XPosPhaseNum; j++) {
			phaseEyePositions3D[i][j] = stageCenterPos;
			phaseEyePositions2D[i][j] = stageCenterPos;
		}
	}

	const float eyeYRatio = 0.1f; //ステージ中央からの距離に対するY座標の割合
	const float topButtomDifference3D = 0.01f;	//完全な真上や真下{ 0, 〇〇, 0 }にしてしまうと正しく描画されないので差分を設定
	const float topButtomDifference2D = 0.5f;	//完全な真上や真下{ 0, 〇〇, 0 }にしてしまうと正しく描画されないので差分を設定

	//ステージ中央からの距離を基準に視点位置を設定
	//縦軸カメラ位置がTopの場合の視点位置3D
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Front] += { 0, distanceStageCenter, -topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Right] += { topButtomDifference3D, distanceStageCenter, 0 };
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Back] += { 0, distanceStageCenter, topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Left] += { -topButtomDifference3D, distanceStageCenter, 0 };
	//縦軸カメラ位置がSideの場合の視点位置3D
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Front] += { 0, distanceStageCenter* eyeYRatio, -distanceStageCenter };
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Right] += { distanceStageCenter, distanceStageCenter* eyeYRatio, 0 };
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Back] += { 0, distanceStageCenter* eyeYRatio, distanceStageCenter };
	phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Left] += { -distanceStageCenter, distanceStageCenter* eyeYRatio, 0 };
	//縦軸カメラ位置がButtomの場合の視点位置3D
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Front] += { 0, -distanceStageCenter, -topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Right] += { topButtomDifference3D, -distanceStageCenter, 0 };
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Back] += { 0, -distanceStageCenter, topButtomDifference3D };
	phaseEyePositions3D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Left] += { -topButtomDifference3D, -distanceStageCenter, 0 };

	//縦軸カメラ位置がTopの場合の視点位置2D
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Front] += { 0, distanceStageCenter, -topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Right] += { topButtomDifference2D, distanceStageCenter, 0 };
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Back] += { 0, distanceStageCenter, topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Top][(int)CameraXPosPhase::Left] += { -topButtomDifference2D, distanceStageCenter, 0 };
	//縦軸カメラ位置がSideの場合の視点位置2D
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Front] += { 0, 0, -distanceStageCenter };
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Right] += { distanceStageCenter, 0, 0 };
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Back] += { 0, 0, distanceStageCenter };
	phaseEyePositions2D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Left] += { -distanceStageCenter, 0, 0 };
	//縦軸カメラ位置がButtomの場合の視点位置2D
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Front] += { 0, -distanceStageCenter, -topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Right] += { topButtomDifference2D, -distanceStageCenter, 0 };
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Back] += { 0, -distanceStageCenter, topButtomDifference2D };
	phaseEyePositions2D[(int)CameraYPosPhase::Buttom][(int)CameraXPosPhase::Left] += { -topButtomDifference2D, -distanceStageCenter, 0 };

	//開始時の視点をセット(X:手前 Y:横)
	SetEye(phaseEyePositions3D[(int)CameraYPosPhase::Side][(int)CameraXPosPhase::Front]);
}

void GameCamera::Update()
{
	//トリガーフラグがtrue状態ならばfalseに直しておく
	if (isTriggerDimensionChange) { isTriggerDimensionChange = false; }

	//カメラ位置移動開始
	ChanegeCameraPosStart();
	//カメラ位置移動
	ChanegeCameraPos();

	//次元切り替え
	ChanegeDimension();

	//カメラ更新
	Camera::Update();
}

void GameCamera::ChanegeDimensionStart()
{
	//2D状態かフラグを切り替える
	if (is2D) {
		//is2D = false;
		//移動目標視点をセット
		moveAfterEye = phaseEyePositions3D[cameraYPosPhase][cameraXPosPhase];
	}
	else {
		//is2D = true;
		//移動目標視点をセット
		moveAfterEye = phaseEyePositions2D[cameraYPosPhase][cameraXPosPhase];
	}
	dirtyProjection = true;

	//移動前視点をセット
	moveBeforeEye = eye;

	//アクション用タイマーを初期化しておく
	actionTimer = 0;

	//行動を「次元切り替え」にする
	actionPhase = ActionPhase::ChangeDimension;
}

void GameCamera::UpdateMatProjection()
{
	//2D状態の場合は平行投影
	if (is2D) {
		matProjection = XMMatrixOrthographicOffCenterLH(
			-128, 128,
			-72, 72,
			0.1f, 2000.0f
		);
	}
	//3D状態の場合は透視投影
	else {
		Camera::UpdateMatProjection();
	}
}

void GameCamera::ChanegeCameraPosStart()
{
	//行動が「何もしない」以外なら抜ける
	if (!(actionPhase == ActionPhase::None)) { return; }
	//2D状態なら抜ける
	if (is2D) { return; }

	//横軸移動も縦軸も開始しなければ抜ける
	if (!(ChanegeCameraXPosStart() || ChanegeCameraYPosStart())) { return; }

	//移動前視点をセット
	moveBeforeEye = eye;
	//移動目標視点をセット
	moveAfterEye = phaseEyePositions3D[cameraYPosPhase][cameraXPosPhase];

	//アクション用タイマーを初期化しておく
	actionTimer = 0;

	//行動を「視点移動」にする
	actionPhase = ActionPhase::MoveEye;
}

bool GameCamera::ChanegeCameraXPosStart()
{
	//縦軸カメラ位置フェーズが「Side」でなければ抜ける
	//if (!(cameraYPosPhase == (int)CameraYPosPhase::Side)) { return false; }
	//移動キー入力がなければfalseを返し抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_LEFT) || Input::GetInstance()->TriggerKey(DIK_RIGHT))) { return false; }

	//キー入力に応じてステージから見ての横軸カメラ位置フェーズを変更
	if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		//ステージから見ての横軸カメラ位置フェーズが0(Front)の場合は一周した扱いにして、3(Left)にしておく
		if (cameraXPosPhase == (int)CameraXPosPhase::Front) { cameraXPosPhase = (int)CameraXPosPhase::Left; }
		//それ以外の場合は現在いるカメラ位置から左に移動させる
		else { cameraXPosPhase--; }
	}
	else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		//ステージから見ての横軸カメラ位置フェーズが3(Left)の場合は一周した扱いにして、0(Front)にしておく
		if (cameraXPosPhase == (int)CameraXPosPhase::Left) { cameraXPosPhase = (int)CameraXPosPhase::Front; }
		//それ以外の場合は現在いるカメラ位置から右に移動させる
		else { cameraXPosPhase++; }
	}

	//横軸カメラ移動を開始するのでtrueを返す
	return true;
}

bool GameCamera::ChanegeCameraYPosStart()
{
	//移動キー入力がなければfalseを返し抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_UP) || Input::GetInstance()->TriggerKey(DIK_DOWN))) { return false; }

	//キー入力に応じてステージから見ての縦軸カメラ位置フェーズを変更
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		//ステージから見ての縦軸カメラ位置フェーズが0(Top)の場合はこれ以上上を向かないので、falseを返し抜ける
		if (cameraYPosPhase == (int)CameraYPosPhase::Top) {
			if (cameraXPosPhase == (int)CameraXPosPhase::Front) { cameraXPosPhase = (int)CameraXPosPhase::Back; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Right) { cameraXPosPhase = (int)CameraXPosPhase::Left; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Back) { cameraXPosPhase = (int)CameraXPosPhase::Front; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Left) { cameraXPosPhase = (int)CameraXPosPhase::Right; }

			cameraYPosPhase = (int)CameraYPosPhase::Side;
		}
		//それ以外の場合は現在いるカメラ位置から上に移動させる
		else { cameraYPosPhase--; }
	}
	else if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		//ステージから見ての縦軸カメラ位置フェーズが2(Buttom)の場合はこれ以上下を向かないので、falseを返し抜ける
		if (cameraYPosPhase == (int)CameraYPosPhase::Buttom) {
			if (cameraXPosPhase == (int)CameraXPosPhase::Front) { cameraXPosPhase = (int)CameraXPosPhase::Back; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Right) { cameraXPosPhase = (int)CameraXPosPhase::Left; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Back) { cameraXPosPhase = (int)CameraXPosPhase::Front; }
			else if (cameraXPosPhase == (int)CameraXPosPhase::Left) { cameraXPosPhase = (int)CameraXPosPhase::Right; }

			cameraYPosPhase = (int)CameraYPosPhase::Side;
		}
		//それ以外の場合は現在いるカメラ位置から下に移動させる
		else { cameraYPosPhase++; }
	}

	//横軸カメラ移動を開始するのでtrueを返す
	return true;
}

void GameCamera::ChanegeCameraPos()
{
	//行動が「視点移動」以外なら抜ける
	if (!(actionPhase == ActionPhase::MoveEye)) { return; }

	//タイマー更新
	actionTimer++;
	const float moveTime = 30; //視点移動にかかる時間

	//視点移動イージングに使用する変数(0～1を算出)
	const float time = actionTimer / moveTime;
	//視点を移動させる
	Vector3 easeEye;
	easeEye.x = Easing::OutQuad(moveBeforeEye.x, moveAfterEye.x, time);
	easeEye.y = Easing::OutQuad(moveBeforeEye.y, moveAfterEye.y, time);
	easeEye.z = Easing::OutQuad(moveBeforeEye.z, moveAfterEye.z, time);
	SetEye(easeEye);

	//タイマーが指定した時間に満たなければ抜ける
	if (actionTimer < moveTime) { return; }

	//行動を「何もしない」に戻す
	actionPhase = ActionPhase::None;
}

void GameCamera::ChanegeDimension()
{
	//行動が「次元切り替え」以外なら抜ける
	if (!(actionPhase == ActionPhase::ChangeDimension)) { return; }

	//タイマー更新
	actionTimer++;
	const float moveTime = 40; //次元切り替え移動にかかる時間

	//イージングに使用する変数(0～1を算出)
	const float time = actionTimer / moveTime;
	//視点を移動させる
	Vector3 easeEye;
	easeEye.x = Easing::OutCubic(moveBeforeEye.x, moveAfterEye.x, time);
	easeEye.y = Easing::OutCubic(moveBeforeEye.y, moveAfterEye.y, time);
	easeEye.z = Easing::OutCubic(moveBeforeEye.z, moveAfterEye.z, time);
	SetEye(easeEye);

	//タイマーが指定した時間に満たなければ抜ける
	if (actionTimer < moveTime) { return; }

	//2D状態かフラグを切り替える
	if (is2D) { is2D = false; }
	else { is2D = true; }

	dirtyProjection = true;

	//次元に変更が完了したトリガーを立てる
	isTriggerDimensionChange = true;

	//行動を「何もしない」に戻す
	actionPhase = ActionPhase::None;
}
