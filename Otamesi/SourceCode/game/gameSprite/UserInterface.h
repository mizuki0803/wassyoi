#pragma once
#include "Menu.h"
#include "DrawerSprite.h"
#include "SoundVolumePointer.h"
#include "HintSprite.h"
#include <array>
#include <vector>
#include <functional>
#include "ImageUIRenderer.h"

class UserInterface
{
public: //enum
	/// <summary>
	/// 引き出しスプライトの役割名
	/// </summary>
	enum DrawerSpriteName
	{
		HowToPlayMenu,		//メニュー
		HowToPlayPlayer,	//プレイヤー操作
		HowToPlayCamera,	//カメラ操作
		Hint1,				//ヒント1
		Hint2,				//ヒント2

		DrawerSpriteNum,	//引き出しスプライト数
	};

	/// <summary>
	/// メニューの全体
	/// </summary>
	enum class GamePhase
	{
		Title,		//タイトル
		Selection,	//選択
		Game,		//ゲーム中
	};

	/// <summary>
	/// メニューの全体
	/// </summary>
	enum class MenuPhase
	{
		Start,		//開始
		Selection,	//選択
	};

	/// <summary>
	/// タイトルシーンの項目
	/// </summary>
	enum class TitleSceneItem
	{
		SoundVolumeChange,	//音量変更
		ExeEnd,				//.exeの終了
	};

	/// <summary>
	/// ステージ選択シーンの項目
	/// </summary>
	enum class StageSelectSceneItem
	{
		SoundVolumeChange,	//音量変更
		SceneChangeTitle,	//タイトルシーンに移行
	};

	/// <summary>
	/// ゲームシーンの項目
	/// </summary>
	enum class GameSceneItem
	{
		SoundVolumeChange,	//音量変更
		SceneChangeStageSelect,	//ステージ選択シーンに移行
		SceneChangeTitle,	//タイトルシーンに移行
	};

	struct SpaceIndo {
		std::unique_ptr<Sprite> inst;
		float isSizeUp;
	};

public: //メンバ関数
	/// <summary>
	/// 生成
	/// </summary>
	static std::unique_ptr<UserInterface> Create(GamePhase gamePhase);

	static void Common(ID3D12Device *dev, ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GamePhase gamePhase);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 加算合成描画
	/// </summary>
	void AddDraw();

	/// <summary>
	/// 奥の描画
	/// </summary>
	void Draw();

	void DrawMenuImage();

	// メニュー関係の処理
	void MenuUpdate();
	// 
	void MenuReset();
	//
	void MenuOpen();
	//
	void MenuSelection();

	/// <summary>
	/// 次元変更可能かをチェックし、スプライトの色を変更する
	/// </summary>
	/// <param name="isChangeDimension">次元変更可能か</param>
	void IsChangeDimensionCheck(bool isChangeDimension);

	/// <summary>
	/// ステージ変更に伴う処理
	/// </summary>
	void StageChangeUpdate();

	//フラグの設定、取得
	void SetMenuFlag(bool flag) { menuFlag_ = flag; drawerSprites[HowToPlayMenu]->MoveStart(); }
	void SetNotMove(bool flag) { notMove_ = flag; }

	bool GetMenuFlag() { return menuFlag_; }
	const int GetSelectionNumber() { return selectionNumber_; }
	bool GetIsHintViewMode();

private: //メンバ関数
	/// <summary>
	/// 引き出しスプライト生成
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="drawerKey">引き出しの開閉に使用するキー</param>
	/// <param name="hidePlace">引き出しで隠れる場所</param>
	/// <param name="posY">Y座標</param>
	/// <param name="stickoutNum">閉じている場合に画面内にはみ出す量</param>
	/// <param name="isOpenDrawer">引き出しを開いている状態か</param>
	void CreateDrawerSprite(const Texture& texture, BYTE drawerKey, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer);

	/// <summary>
	/// 子供スプライト生成
	/// </summary>
	/// <param name="texture">テクスチャ</param>
	/// <param name="parent">親スプライト</param>
	/// <param name="position">親スプライト原点での座標</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	void CreateChildSprite(const Texture& texture, Sprite* parent, const Vector2& position, const Vector2& anchorpoint = { 0.5f, 0.5f });

	/// <summary>
	/// キー入力による引き出しスプライト移動開始
	/// </summary>
	void DrawerSpriteMoveStartKey();

	/// <summary>
	/// ヒントスプライトの大きさ変更
	/// </summary>
	void HintSpriteSizeChange();

	/// <summary>
	/// スペースのエフェクト
	/// </summary>
	void SpaceEffect();

private: //静的メンバ変数
	//音の大きさ
	static float soundVolume_;
	//最大音量
	static const float soundMaxVolume_;

private: //メンバ変数
	//次元変更可能か可能のときのみ光るスペースキースプライト
	std::unique_ptr<Sprite> isChangeDimenisonSprite;
	// UndoRedo説明用
	std::unique_ptr<Sprite> unDoReDo;
	//押せるときの反応
	std::array<SpaceIndo, 4> ChangeDimenisonSpriteEffect;
	//反応を入れるカウント
	int SpriteEffectCount;
	//次元変更可能か
	bool isChangeDimenison;
	//説明用引き出しスプライト
	std::vector<std::unique_ptr<DrawerSprite>> drawerSprites;
	//引き出しスプライトの子供
	std::vector<std::unique_ptr<Sprite>> childSprites;
	//ヒントスプライト
	std::array<std::unique_ptr<HintSprite>, 2> hintSprites;
	// イージング進行
	float easeTimer_ = 0.0f;
	// メニュー用の背景
	std::unique_ptr<Sprite> menuBackScreen_;
	// メニュー用の枠組み
	std::vector<std::unique_ptr<Menu>> menuframe_;
	// 音量設定用バースプライト
	std::unique_ptr<Sprite> soundVolumeBar;
	// 音量設定用ポインタースプライト
	std::unique_ptr<SoundVolumePointer> soundVolumePointer;
	// 行動フラグ
	bool menuFlag_ = false;
	// 行動をさせない
	bool notMove_ = false;
	// メニュー選択
	int selectionNumber_ = 0;
	// 選択可能か
	bool selectionFlag_ = false;
	// 関数の管理
	std::vector<std::function<void()>> menuFunc_;
	// 関数の番号
	size_t menuPhase_ = static_cast<int>(MenuPhase::Start);
	// プレイシーンの保存用
	GamePhase gamePhase_;

	// メニュー用の画像生成
	std::unique_ptr<ImageUIRenderer> imageUiRendere_;
};
