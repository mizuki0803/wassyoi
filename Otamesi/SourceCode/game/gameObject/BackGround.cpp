#include "BackGround.h"
#include "Easing.h"
#include <math.h>
#include <random>

#include <JsonLoader.h>

/// <summary>
/// 乱数生成
/// 0から範囲までの乱数を出力
/// </summary>
/// <param name="_range">範囲</param>
/// <param name="_under">最低値</param>
/// <returns>乱数</returns>
static int Randomint(int _range, int _under = 0)
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand100(_under, _range);

	return rand100(mt);
}

BackGround* BackGround::Create()
{
	//新たなObjModel型のインスタンスのメモリを確保
	BackGround* model = new BackGround();

	//objファイルからデータ読み込み
	model->Initialize();

	return model;
}

void BackGround::Initialize()
{
	model.reset(ObjModel::LoadFromOBJ("backBlock"));
	for (int i = 0; i < instanceNum; ++i)
	{
		block[i].reset(InstanceObject::Create(model.get()));
		block[i]->SetIsShadowMap(true);
	}

	JsonLoader::LoadNlohmannJson("back", &info);

	//イージング用変数初期化
	const int size = int(info.size());
	playMoveInfo.resize(size);
	clearMoveInfo.resize(size);
	for (int i = 0; i < size; i++) {
		//ゲームプレイ用
		playMoveInfo[i].move = {};
		playMoveInfo[i].timer = {};
		//クリア演出用
		XMFLOAT2 pos = { abs(info[i][int(transform::translation)].x),abs(info[i][int(transform::translation)].z) };
		float dist = pos.x * pos.x + pos.y * pos.y;
		clearMoveInfo[i].easingNum = int(sqrt(dist) / 5.0f);
		clearMoveInfo[i].step = moveStep::non;
	}
	setMoveTimer = 0;
}

void BackGround::Update()
{
	//クリア演出をやっているなら止める
	if (!isClear) {
		//移動オブジェクトセット
		setMoveTimer++;
		if (setMoveTimer % 80 == 1) {
			SetPlayMove();
			setMoveTimer = 1;
		}

		//オブジェクト移動
		PlayMove();
	}

	//ステージclear演出
	if (isClear) {
		SetStageClear();
		StageClear();
	}

	const DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };

	//使用するInstance番号
	int useBlock = 0;
	for (int i = 0; i< int(info.size()); i++) {

		XMFLOAT3 pos = info[i][int(transform::translation)];
		pos.y += clearMoveInfo[i].move + playMoveInfo[i].move;

		block[useBlock]->DrawInstance(pos, info[i][int(transform::scaling)], info[i][int(transform::rotation)], color);
		if (!block[useBlock]->GetInstanceDrawCheck()) { useBlock++; }
	}
}

void BackGround::Draw()
{
	for (int i = 0; i < instanceNum; ++i)
	{
		block[i]->Draw();
	}
}

void BackGround::DrawLightCameraView()
{
	for (int i = 0; i < instanceNum; ++i)
	{
		block[i]->DrawLightCameraView();
	}
}

void BackGround::FrameReset()
{
	for (int i = 0; i < instanceNum; ++i)
	{
		block[i]->FrameReset();
	}
}

void BackGround::SetStageClearFlag()
{
	isClear = true;
	isClearMove = false;
	objectMoveNum = 0;
	for (auto& itr : clearMoveInfo) {
		itr.step = moveStep::non;
	}
}

void BackGround::SetPlayMove()
{
	const int maxcount = 50;//最大個数
	int count = 0;//現在個数
	const int size = int(info.size()) - 1;
	//個数分セット
	while (count < maxcount)
	{
		int setObject = {};
		//同じオブジェクトにならないようにwhile分を回す
		do {
			setObject = Randomint(size, 0);
		} while (playMoveInfo[setObject].afterMove != 0.0f);

		//移動方向を決める
		//移動を一回もしていない場合
		if (playMoveInfo[setObject].move == 0.0f) {
			playMoveInfo[setObject].afterMove = (Randomint(300, 0) - 150.0f) / 10.0f;
			if (playMoveInfo[setObject].afterMove >= 0) {
				playMoveInfo[setObject].afterMove += 10.0f;
			} else {
				playMoveInfo[setObject].afterMove -= 10.0f;
			}
		}
		//一回以上移動していた場合
		else {
			if (playMoveInfo[setObject].move >= 0) {
				playMoveInfo[setObject].afterMove = (Randomint(150, 0) / 10.0f) + 10.0f;
			} else {
				playMoveInfo[setObject].afterMove = ((Randomint(150, 0) - 150.0f) / 10.0f) - 10.0f;
			}
		}

		playMoveInfo[setObject].beforeMove = playMoveInfo[setObject].move;
		playMoveInfo[setObject].timer = 0.0f;
		count++;
	}
}

void BackGround::PlayMove()
{
	const float maxtime = 200;
	for (auto& itr : playMoveInfo) {
		if (itr.afterMove == 0.0f) { continue; }
		itr.timer++;
		itr.move = Easing::InQuad(itr.beforeMove, itr.afterMove, itr.timer / maxtime);

		if (itr.timer < maxtime) { continue; }
		itr.afterMove = 0.0f;
	}
}

void BackGround::SetStageClear()
{
	for (int i = 0; i<int(info.size()); i++) {
		if (clearMoveInfo[i].step != moveStep::non || clearMoveInfo[i].easingNum > objectMoveNum) { continue; }
		clearMoveInfo[i].step = moveStep::up;
	}

	objectMoveNum++;
}

void BackGround::StageClear()
{
	const float maxtime = 50.0f;
	const float maxeaspos = 50.0f;
	int nowMove = 0;
	for (auto& itr : clearMoveInfo) {
		if (itr.step == moveStep::non || itr.step == moveStep::end) { continue; }
		isClearMove = true;
		itr.timer++;
		nowMove++;
		//上昇
		if (itr.step == moveStep::up) {
			itr.move = Easing::InQuad(0.0f, maxeaspos, itr.timer / maxtime);
			//イージング終了次第次に移行
			if (itr.timer < maxtime) { continue; }
			itr.move = maxeaspos;
			itr.timer = 0.0f;
			itr.step = moveStep::down;
		}
		//下降
		else if (itr.step == moveStep::down) {
			itr.move = Easing::OutQuad(maxeaspos, 0.0f, itr.timer / maxtime);
			//イージング終了次第次に移行
			if (itr.timer < maxtime) { continue; }
			itr.move = 0.0f;
			itr.timer = 0.0f;
			itr.step = moveStep::end;
		}
	}

	//クリア演出終了
	if (nowMove == 0 && isClearMove) {
		isClear = false;
		isClearMove = false;
	}
}
