#include "FrameWork.h"
#include "FpsCheck.h"
#include "DescHeapSRV.h"
#include "TextureManager.h"
#include "SpriteTextureLoader.h"
#include "ObjObject3d.h"
#include "FbxLoader.h"
#include "FbxObject3d.h"
#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "LightGroup.h"
#include "GamePostEffect.h"
#include "InstanceObject.h"
#include "math/Timer.h"

void FrameWork::Run()
{
	//ゲーム初期化
	Initialize();

	//ゲームループ
	while (true) {
		//毎フレーム更新
		Update();

		//終了リクエストが来たらループを抜ける
		if (GetIsEndRequest()) { break; }

		//描画
		Draw();
	}

	//ゲーム終了
	Finalize();
}

void FrameWork::Initialize()
{
	//ウインドウ作成
	win.reset(new WindowApp());
	win->WindowCreate(L"チェンジゲン");

	//DirectX初期化
	dxbase.reset(new DirectXBase());
	dxbase->Initialize(win.get());

	//入力の初期化
	input = Input::GetInstance();
	input->Initialize(win.get());

	//音声初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	//SRV用デスクリプタヒープの初期化
	DescHeapSRV::Initialize(dxbase->GetDevice(), dxbase->GetCmdList());
	//テクスチャ管理の初期化
	TextureManager::SetDevice(dxbase->GetDevice());

	//スプライト共通部分初期化
	spriteCommon = SpriteCommon::GetInstance();
	spriteCommon->Initialize(dxbase->GetDevice(), dxbase->GetCmdList(), win->window_width, win->window_height);
	//スプライトテクスチャ全読み込み
	SpriteTextureLoader::TextureLoad();

	//デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(SpriteTextureLoader::GetTexture(SpriteTextureLoader::DebugFont));

	//ポストエフェクト共通初期化処理
	PostEffect::PostEffectCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//ゲーム共通ポストエフェクトの初期化
	GamePostEffect::Initialize();


	afterBloom->AfterBloomCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	afterBloom.reset(AfterBloom::Create());

	outLine->OutLineCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	outLine.reset(OutLine::Create());
	outLineDraw->OutLineCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	outLineDraw.reset(OutLineDraw::Create());
	//シャドウマップ共通初期化処理
	ShadowMap::ShadowMapCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//シャドウマップの初期化
	shadowMap.reset(ShadowMap::Create());

	//objオブジェクト3d共通初期化処理
	ObjObject3d::Object3dCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	ObjModel::SetShadowMapTexture(shadowMap->GetTexture());

	//objオブジェクト3d共通初期化処理
	InstanceObject::InstanceObjectCommon(dxbase->GetDevice(), dxbase->GetCmdList());

	//FBXLoader初期化
	FbxLoader::GetInstance()->Initialize(dxbase->GetDevice());

	//FBXオブジェクト3d共通
	FbxObject3d::SetDevice(dxbase->GetDevice());
	FbxObject3d::SetCommandList(dxbase->GetCmdList());
	//グラフィックスパイプライン生成
	FbxObject3d::CreateGraphicsPipeline();

	//ライト共通初期化処理
	LightGroup::StaticInitialize(dxbase->GetDevice());

	//パーティクル共通初期化処理
	ParticleManager::ParticleManagerCommon(dxbase->GetDevice(), dxbase->GetCmdList());
	//パーティクルエミッター初期化
	ParticleEmitter::GetInstance()->Initialize();

	//全シーンで使用するテクスチャの枚数を確定させる
	DescHeapSRV::SetAllSceneTextureNum();

	// タイマーsingleton生成
	Timer::CreateSingleton();
;
}

void FrameWork::Finalize()
{
	SceneManager::GetInstance()->Finalize();
	//FBXLoader解放
	FbxLoader::GetInstance()->Finalize();

	// singleton解放
	SingletonFinalizer::Finalize();

	//audio解放
	audio->Finalize();

	//ウインドウ解放
	win->WindowRelease();
}

void FrameWork::Update()
{
	// タイマー計測開始
	Timer::GetInstance().InstrumentationStart();

	//入力の更新
	input->Update();

	//シーン更新
	SceneManager::GetInstance()->Update();

	//メッセージ
	//×ボタンで終了メッセージが来たらゲームループを抜ける
	if (win->MessageProc()) {
		isEndRequest = true;
		return;
	}
	//各シーンから終了リクエストがあればゲームループ終了
	if (SceneManager::GetInstance()->GetIsEndRequest()) {
		isEndRequest = true;
		return;
	}

	//フレームレート確認
	//FpsCheck::Display();
}

void FrameWork::Draw()
{
	//SRV用共通デスクリプタヒープSetDescriptorHeaps
	DescHeapSRV::SetDescriptorHeaps();

	//シャドウマップのレンダーテクスチャへの描画
	shadowMap->DrawScenePrev();
	SceneManager::GetInstance()->Draw3DLightView();
	shadowMap->DrawSceneRear();

	//ゲームポストエフェクトへの描画
	GamePostEffect::DrawScenePrev();
	SceneManager::GetInstance()->Draw3D();
	GamePostEffect::DrawSceneRear();

	// 単体で発光するもの用
	afterBloom->DrawScenePrev(GamePostEffect::GetPostEffect()->GetDsv());
	SceneManager::GetInstance()->AfterBloomDraw();
	afterBloom->DrawSceneRear();
	
	// アウトライン描画用
	outLine->DrawScenePrev(GamePostEffect::GetPostEffect()->GetDsv());
	SceneManager::GetInstance()->OutLineDraw();
	outLine->DrawSceneRear();

	outLineDraw->DrawScenePrev(GamePostEffect::GetPostEffect()->GetDsv());
	outLine->Draw();
	outLineDraw->DrawSceneRear();


	//グラフィックスコマンド(前)
	dxbase->GraphicsCommandPrev();

	//シーンの背景スプライト描画
	SceneManager::GetInstance()->DrawBackSprite();

	//ゲームポストエフェクトの描画
	GamePostEffect::Draw();
	afterBloom->Draw();				// アフターブルームを描画
	outLineDraw->Draw();
	//シーンの前景スプライト描画
	SceneManager::GetInstance()->DrawFrontSprite();

	//スプライト共通コマンド
	spriteCommon->DrawPrev();
	//デバックテキスト描画
	debugText->DrawAll();

	//グラフィックスコマンド(後)
	dxbase->GraphicsCommandRear();

	Timer::GetInstance().InstrumentationEnd();

}
