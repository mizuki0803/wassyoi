#pragma once
#include "Menu.h"
#include "DrawerSprite.h"
#include "SoundVolumePointer.h"
#include "HintSprite.h"
#include <array>
#include <vector>
#include <functional>

class UserInterface
{
public: //enum
	/// <summary>
	/// �����o���X�v���C�g�̖�����
	/// </summary>
	enum DrawerSpriteName
	{
		HowToPlayMenu,		//���j���[
		HowToPlayPlayer,	//�v���C���[����
		HowToPlayCamera,	//�J��������
		Hint1,				//�q���g1
		Hint2,				//�q���g2

		DrawerSpriteNum,	//�����o���X�v���C�g��
	};

	/// <summary>
	/// ���j���[�̑S��
	/// </summary>
	enum class GamePhase
	{
		Title,		//�^�C�g��
		Selection,	//�I��
		Game,		//�Q�[����
	};

	/// <summary>
	/// ���j���[�̑S��
	/// </summary>
	enum class MenuPhase
	{
		Start,		//�J�n
		Selection,	//�I��
	};

	/// <summary>
	/// �^�C�g���V�[���̍���
	/// </summary>
	enum class TitleSceneItem
	{
		SoundVolumeChange,	//���ʕύX
		ExeEnd,				//.exe�̏I��
	};

	/// <summary>
	/// �X�e�[�W�I���V�[���̍���
	/// </summary>
	enum class StageSelectSceneItem
	{
		SoundVolumeChange,	//���ʕύX
		SceneChangeTitle,	//�^�C�g���V�[���Ɉڍs
	};

	/// <summary>
	/// �Q�[���V�[���̍���
	/// </summary>
	enum class GameSceneItem
	{
		SoundVolumeChange,	//���ʕύX
		SceneChangeStageSelect,	//�X�e�[�W�I���V�[���Ɉڍs
		SceneChangeTitle,	//�^�C�g���V�[���Ɉڍs
	};

public: //�����o�֐�
	/// <summary>
	/// ����
	/// </summary>
	static std::unique_ptr<UserInterface> Create(GamePhase gamePhase);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize(GamePhase gamePhase);

	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();

	/// <summary>
	/// ���̕`��
	/// </summary>
	void Draw();

	// ���j���[�֌W�̏���
	void MenuUpdate();
	// 
	void MenuReset();
	//
	void MenuOpen();
	//
	void MenuSelection();

	/// <summary>
	/// �����ύX�\�����`�F�b�N���A�X�v���C�g�̐F��ύX����
	/// </summary>
	/// <param name="isChangeDimension">�����ύX�\��</param>
	void IsChangeDimensionCheck(bool isChangeDimension);

	/// <summary>
	/// �����o���X�v���C�g�̊J��Ԃ����Z�b�g
	/// </summary>
	void DrawerSpriteReset();

	//�t���O�̐ݒ�A�擾
	void SetMenuFlag(bool flag) { menuFlag_ = flag; drawerSprites[HowToPlayMenu]->MoveStart(); }
	void SetNotMove(bool flag) { notMove_ = flag; }

	bool GetMenuFlag() { return menuFlag_; }
	const int GetSelectionNumber() { return selectionNumber_; }
	bool GetIsHintViewMode();

private: //�����o�֐�
	/// <summary>
	/// �����o���X�v���C�g����
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="drawerKey">�����o���̊J�Ɏg�p����L�[</param>
	/// <param name="hidePlace">�����o���ŉB���ꏊ</param>
	/// <param name="posY">Y���W</param>
	/// <param name="stickoutNum">���Ă���ꍇ�ɉ�ʓ��ɂ͂ݏo����</param>
	/// <param name="isOpenDrawer">�����o�����J���Ă����Ԃ�</param>
	void CreateDrawerSprite(const Texture& texture, BYTE drawerKey, DrawerSprite::HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer);

	/// <summary>
	/// �q���X�v���C�g����
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="parent">�e�X�v���C�g</param>
	/// <param name="position">���W</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	void CreateChildSprite(const Texture& texture, Sprite* parent, const Vector2& position, const Vector2& anchorpoint = { 0.5f, 0.5f });

	/// <summary>
	/// �L�[���͂ɂ������o���X�v���C�g�ړ��J�n
	/// </summary>
	void DrawerSpriteMoveStartKey();

	/// <summary>
	/// �q���g�X�v���C�g�̑傫���ύX
	/// </summary>
	void HintSpriteSizeChange();

private: //�ÓI�����o�ϐ�
	//���̑傫��
	static float soundVolume_;
	//�ő剹��
	static const float soundMaxVolume_;

private: //�����o�ϐ�
	//�����ύX�\���\�̂Ƃ��̂݌���X�y�[�X�L�[�X�v���C�g
	std::unique_ptr<Sprite> isChangeDimenisonSprite;
	//�����p�����o���X�v���C�g
	std::vector<std::unique_ptr<DrawerSprite>> drawerSprites;
	//�����o���X�v���C�g�̎q��
	std::vector<std::unique_ptr<Sprite>> childSprites;
	//�q���g�X�v���C�g
	std::array<std::unique_ptr<HintSprite>, 2> hintSprites;
	// �C�[�W���O�i�s
	float easeTimer_ = 0.0f;
	// ���j���[�p�̔w�i
	std::unique_ptr<Sprite> menuBackScreen_;
	// ���j���[�p�̘g�g��
	std::vector<std::unique_ptr<Menu>> menuframe_;
	// ���ʐݒ�p�o�[�X�v���C�g
	std::unique_ptr<Sprite> soundVolumeBar;
	// ���ʐݒ�p�|�C���^�[�X�v���C�g
	std::unique_ptr<SoundVolumePointer> soundVolumePointer;
	// �s���t���O
	bool menuFlag_ = false;
	// �s���������Ȃ�
	bool notMove_ = false;
	// ���j���[�I��
	int selectionNumber_ = 0;
	// �I���\��
	bool selectionFlag_ = false;
	// �֐��̊Ǘ�
	std::vector<std::function<void()>> menuFunc_;
	// �֐��̔ԍ�
	size_t menuPhase_ = static_cast<int>(MenuPhase::Start);
	// �v���C�V�[���̕ۑ��p
	GamePhase gamePhase_;
};
