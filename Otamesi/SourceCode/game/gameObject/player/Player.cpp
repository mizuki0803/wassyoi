#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "Block.h"
#include "PlayerActionManager.h"


const float Player::playerSize = 3.5f;

Player* Player::Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& shiftPos, GameCamera* gameCamera, ObjModel* effectModel)
{
	//インスタンスを生成
	Player* instance = new Player();

	//モデルをセット
	assert(model);
	instance->model = model;

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	// 関数の設定
	instance->CreateAct();
	// イージングの設定
	instance->easeData_ = std::make_unique<EaseData>(60);
	//プレイヤー位置を表すマップ番号をセット
	instance->mapChipNumberPos = mapChipNum;
	//マップの中心をずらす値をセット
	instance->shiftPos = shiftPos;
	//初期座標をセット
	instance->SetPlayerEndPos(instance->GetMapChipPos(mapChipNum));
	Vector3 tempPos = instance->GetMapChipPos(mapChipNum);
	//位置をずらしてイージング
	instance->playerEndPos_ = tempPos;
	tempPos.y -= 100.0f;
	instance->playerStratPos_ = tempPos;
	//大きさをセット
	instance->scale = { playerSize, playerSize, playerSize };
	//ゲームカメラをセット
	instance->gameCamera = gameCamera;
	//最初の移動が終わったかのフラグ
	instance->isStartMove = false;

	// エフェクト読み込み
	for (int i = 0; i < instance->effect.size(); ++i)
	{
		instance->effect[i].reset(PlayerEffect::Create(effectModel, static_cast<float>(i)));
	}

	return instance;
}

void Player::Update()
{
	//行動
	func_[phase_]();

	//オブジェクト更新
	ObjObject3d::Update();

	// エフェクト更新
	for (auto& e : effect)
	{
		e->Update(this);
	}
}

void Player::ReCreate(const XMINT3& mapChipNum, const Vector3& shiftPos)
{
	SetGamePhase(GamePhase::ReStart);

	//マップの中心をずらす値をセット
	this->shiftPos = shiftPos;
	//プレイヤー位置を表すマップ番号をセット
	mapChipNumberPos = mapChipNum;
	//初期座標をセット
	SetPlayerEndPos(GetMapChipPos(mapChipNum));
	Vector3 tempPos = GetMapChipPos(mapChipNum);
	//位置をずらしてイージング
	playerStratPos_ = position;
	tempPos.x += 100.0f;
	playerEndPos_ = tempPos;

	easeData_->Reset();
}

void Player::PlayGame()
{
	//ゴールしていないときに動きをする
	if (isGoal) {
		return;
	}
	//frame最初の初期化
	isMove = false;

	//ゲームカメラの次元に変更が完了トリガーフラグがtrueなら
	if (gameCamera->GetIsTriggerDimensionChange()) {
		//2次元状態なら、プレイヤーの位置を画面手前に移動させる
		if (gameCamera->GetIs2D()) {
			PlayerActionManager::PlayerFrontmost2D(mapChipNumberPos, moveSurfacePhase);
			position = GetMapChipPos(mapChipNumberPos);
		}
		//ゴールしたのかを判定
		StageClearCheck();
	}

	//座標移動開始
	MovePosStart();
	//座標移動
	MovePos();

	//次元切り替え開始
	ChanegeDimensionStart();
}

void Player::GameStart()
{
	// イージングの計算
	position.x = Easing::OutBack(playerStratPos_.x, playerEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::OutBack(playerStratPos_.y, playerEndPos_.y, easeData_->GetTimeRate());
	position.z = Easing::OutBack(playerStratPos_.z, playerEndPos_.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		isStartMove = true;
		phase_ = static_cast<int>(GamePhase::GamePlay);
	}
	easeData_->Update();
	resetFlag_ = true;
}

void Player::GameReStart()
{
	// イージングの計算
	position.x = Easing::InCubic(playerStratPos_.x, playerEndPos_.x, easeData_->GetTimeRate());
	position.y = Easing::InCubic(playerStratPos_.y, playerEndPos_.y, easeData_->GetTimeRate());
	position.z = Easing::InCubic(playerStratPos_.z, playerEndPos_.z, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		phase_ = static_cast<int>(GamePhase::GamePlay);
	}
	easeData_->Update();
	resetFlag_ = true;
}

void Player::CreateAct()
{
	func_.push_back([this] { return PlayGame(); });
	func_.push_back([this] { return GameStart(); });
	func_.push_back([this] { return GameReStart(); });
}

void Player::Draw()
{
	// エフェクト読み込み
	for (auto& e : effect)
	{
		e->Draw();
	}

	ObjObject3d::Draw();

	isStartMove = false;
}

void Player::Reset()
{
	//開始状態に戻すためにフラグなどをリセット
	isGoal = false;
	phase_ = static_cast<int>(GamePhase::GamePlay);
}

void Player::MovePosStart()
{
	//行動が「何もしない」以外なら抜ける
	if (!(actionPhase == ActionPhase::None)) { return; }
	//ゲームカメラの行動が「何もしない」以外なら抜ける
	if (!(gameCamera->GetActionPhase() == GameCamera::ActionPhase::None)) { return; }
	//移動キー入力がなければ抜ける
	if (!(Input::GetInstance()->PushKey(DIK_W) || Input::GetInstance()->PushKey(DIK_S)
		|| Input::GetInstance()->PushKey(DIK_A) || Input::GetInstance()->PushKey(DIK_D))) {
		return;
	}

	//移動可能か判定
	if (gameCamera->GetIs2D()) {
		if (!PlayerActionManager::PlayerMoveCheck2D(mapChipNumberPos, moveSurfacePhase, gameCamera->GetCameraXPosPhase(), gameCamera->GetCameraYPosPhase())) { return; };
	}
	else {
		if (!PlayerActionManager::PlayerMoveCheck3D(mapChipNumberPos, moveSurfacePhase, gameCamera->GetCameraXPosPhase(), gameCamera->GetCameraYPosPhase())) { return; };
	}

	//移動前座標をセット
	moveBeforePos = position;
	//移動目標座標をセット
	moveAfterPos = GetMapChipPos(mapChipNumberPos);

	//アクション用タイマーを初期化しておく
	actionTimer = 0;

	//行動を「座標移動」にする
	actionPhase = ActionPhase::MovePos;
}

void Player::MovePos()
{
	//行動が「座標移動」以外なら抜ける
	if (!(actionPhase == ActionPhase::MovePos)) { return; }

	//タイマー更新
	actionTimer++;
	const float moveTime = 30; //座標移動にかかる時間

	//座標移動イージングに使用する変数(0～1を算出)
	const float time = actionTimer / moveTime;
	//プレイヤーを移動させる
	position.x = Easing::OutQuint(moveBeforePos.x, moveAfterPos.x, time);
	position.y = Easing::OutQuint(moveBeforePos.y, moveAfterPos.y, time);
	position.z = Easing::OutQuint(moveBeforePos.z, moveAfterPos.z, time);

	//タイマーが指定した時間に満たなければ抜ける
	if (actionTimer < moveTime) { return; }

	//移動したのでtrue
	isMove = true;

	//ゴールしたのかを判定
	StageClearCheck();

	//行動を「何もしない」に戻す
	actionPhase = ActionPhase::None;
}

void Player::ChanegeDimensionStart()
{
	//行動が「何もしない」以外なら抜ける
	if (!(actionPhase == ActionPhase::None)) { return; }
	//ゲームカメラの行動が「何もしない」以外なら抜ける
	if (!(gameCamera->GetActionPhase() == GameCamera::ActionPhase::None)) { return; }
	//キー入力がなければfalseを返し抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_SPACE))) { return; }

	//現在カメラの向いている方向の面に接地する
	MoveSurfacePhase judgeMoveSurfacePhase; //判定用変数
	if (gameCamera->GetCameraYPosPhase() == (int)GameCamera::CameraYPosPhase::Top) { judgeMoveSurfacePhase = MoveSurfacePhase::Upward; }
	else if (gameCamera->GetCameraYPosPhase() == (int)GameCamera::CameraYPosPhase::Buttom) { judgeMoveSurfacePhase = MoveSurfacePhase::Downward; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Front) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingForward; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Right) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingRight; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Back) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingAway; }
	else if (gameCamera->GetCameraXPosPhase() == (int)GameCamera::CameraXPosPhase::Left) { judgeMoveSurfacePhase = MoveSurfacePhase::FacingLeft; }

	//現在のカメラ方向で見た場合に、プレイヤーの手前にブロックがない かつ プレイヤーの奥にブロックが存在するか判定
	if (!(!PlayerActionManager::DirectionForwardBlockCheck(mapChipNumberPos, judgeMoveSurfacePhase) &&
		PlayerActionManager::DirectionAwayBlockCheck(mapChipNumberPos, judgeMoveSurfacePhase))) {
		return;
	}

	//判定用に使用した変数を接地面にする
	moveSurfacePhase = judgeMoveSurfacePhase;

	//2Dから3Dへ戻る場合、足場となるブロックに接地するマップ番号にプレイヤーを移動させる
	if (gameCamera->GetIs2D()) {
		PlayerActionManager::PlayerScaffoldReturn3D(mapChipNumberPos, moveSurfacePhase);

		//更新したマップ番号の座標に移動
		position = GetMapChipPos(mapChipNumberPos);
	}
	//ゲームカメラを次元切り替え状態にする
	gameCamera->ChanegeDimensionStart();
}

void Player::StageClearCheck()
{
	//2次元状態ゴールしたのかを判定
	if (gameCamera->GetIs2D()) {
		if (PlayerActionManager::PlayerGoalCheck2D(mapChipNumberPos, moveSurfacePhase)) {
			isGoal = true;
		}
	}
	//3次元状態ゴールしたのかを判定
	else {
		if (PlayerActionManager::PlayerGoalCheck3D(mapChipNumberPos, moveSurfacePhase)) {
			isGoal = true;
		}
	}
}

Vector3 Player::GetMapChipPos(const XMINT3& mapChipNumberPos)
{
	Vector3 mapChipPos = { mapChipNumberPos.x * Block::GetBlockSize(), mapChipNumberPos.y * Block::GetBlockSize(), mapChipNumberPos.z * Block::GetBlockSize() };
	return mapChipPos - shiftPos;
}

void Player::SetEaseData(const int count)
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
