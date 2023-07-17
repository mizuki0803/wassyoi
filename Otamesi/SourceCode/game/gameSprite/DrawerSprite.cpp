#include "DrawerSprite.h"
#include "Input.h"
#include "Easing.h"
#include "WindowApp.h"

DrawerSprite* DrawerSprite::Create(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//インスタンス生成
	DrawerSprite* instance = new DrawerSprite();

	//初期化処理
	instance->Initialize(texture, drawerKey, hidePlace, posY, stickoutNum, isOpenDrawer);

	return instance;
}

bool DrawerSprite::Initialize(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//引き出しの開閉に使用するキーをセット
	this->drawerKey = drawerKey;

	//隠れる場所に応じてアンカーポイントをセット
	Vector2 anchorpoint;
	if (hidePlace == HidePlace::Left) { anchorpoint = { 1.0f, 0.0f }; }
	else if (hidePlace == HidePlace::Right) { anchorpoint = { 0.0f, 0.0f }; }

	//スプライト生成
	if (!Sprite::Initialize(texture, position, anchorpoint, false, false)) {
		return false;
	}

	//閉じている場合の座標をセット
	if (hidePlace == HidePlace::Left) { closedStatePos = { stickoutNum, posY }; }
	else if (hidePlace == HidePlace::Right) { closedStatePos = { WindowApp::window_width - stickoutNum, posY }; }
	//開いている場合の座標をセット
	openStatePos = closedStatePos;
	if (hidePlace == HidePlace::Left) { openStatePos.x += size.x - stickoutNum-10; }
	else if (hidePlace == HidePlace::Right) { openStatePos.x -= size.x - stickoutNum-10; }

	//開始時に引き出しを開いている状態かセット
	isOpenDrawerSceneStart = isOpenDrawer;
	this->isOpenDrawer = isOpenDrawer;

	//開始時の開閉状態に合わせて座標をセット
	if (isOpenDrawer) { position = openStatePos; }
	else { position = closedStatePos; }

	return true;
}

void DrawerSprite::Update()
{
	//開閉移動
	Move();

	//スプライト更新
	Sprite::Update();
}

void DrawerSprite::MoveStart()
{
	//移動開始時座標をセット
	moveStartPos = position;

	//タイマーを初期化
	moveTimer = 0;
	//開閉開始中状態にする
	isMoveDrawer = true;

	//開閉の状態を反転させる
	if (isOpenDrawer) { isOpenDrawer = false; }
	else { isOpenDrawer = true; }
}

void DrawerSprite::Reset()
{
	//座標と開閉状態をシーン開始状態に戻す
	isOpenDrawer = isOpenDrawerSceneStart;
	if (isOpenDrawer) { position = openStatePos; }
	else { position = closedStatePos; }
	isMoveDrawer = false;
}

void DrawerSprite::Move()
{
	//開閉移動中でなければ抜ける
	if (!isMoveDrawer) { return; }

	//タイマー更新
	moveTimer++;
	//座標移動イージングに使用する変数(0～1を算出)
	const float time = moveTimer / (float)moveTime;

	//移動させる
	//開く
	if (isOpenDrawer) {
		position.x = Easing::OutQuint(moveStartPos.x, openStatePos.x, time);
		position.y = Easing::OutQuint(moveStartPos.y, openStatePos.y, time);
	}
	//閉じる
	else {
		position.x = Easing::OutQuint(moveStartPos.x, closedStatePos.x, time);
		position.y = Easing::OutQuint(moveStartPos.y, closedStatePos.y, time);
	}

	//タイマーが指定した時間に満たなければ抜ける
	if (moveTimer < moveTime) { return; }

	//完了したので移動状態を解除する
	isMoveDrawer = false;
}
