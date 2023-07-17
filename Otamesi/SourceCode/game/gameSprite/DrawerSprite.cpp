#include "DrawerSprite.h"
#include "Input.h"
#include "Easing.h"
#include "WindowApp.h"

DrawerSprite* DrawerSprite::Create(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//�C���X�^���X����
	DrawerSprite* instance = new DrawerSprite();

	//����������
	instance->Initialize(texture, drawerKey, hidePlace, posY, stickoutNum, isOpenDrawer);

	return instance;
}

bool DrawerSprite::Initialize(const Texture& texture, BYTE drawerKey, HidePlace hidePlace, float posY, float stickoutNum, bool isOpenDrawer)
{
	//�����o���̊J�Ɏg�p����L�[���Z�b�g
	this->drawerKey = drawerKey;

	//�B���ꏊ�ɉ����ăA���J�[�|�C���g���Z�b�g
	Vector2 anchorpoint;
	if (hidePlace == HidePlace::Left) { anchorpoint = { 1.0f, 0.0f }; }
	else if (hidePlace == HidePlace::Right) { anchorpoint = { 0.0f, 0.0f }; }

	//�X�v���C�g����
	if (!Sprite::Initialize(texture, position, anchorpoint, false, false)) {
		return false;
	}

	//���Ă���ꍇ�̍��W���Z�b�g
	if (hidePlace == HidePlace::Left) { closedStatePos = { stickoutNum, posY }; }
	else if (hidePlace == HidePlace::Right) { closedStatePos = { WindowApp::window_width - stickoutNum, posY }; }
	//�J���Ă���ꍇ�̍��W���Z�b�g
	openStatePos = closedStatePos;
	if (hidePlace == HidePlace::Left) { openStatePos.x += size.x - stickoutNum-10; }
	else if (hidePlace == HidePlace::Right) { openStatePos.x -= size.x - stickoutNum-10; }

	//�J�n���Ɉ����o�����J���Ă����Ԃ��Z�b�g
	isOpenDrawerSceneStart = isOpenDrawer;
	this->isOpenDrawer = isOpenDrawer;

	//�J�n���̊J��Ԃɍ��킹�č��W���Z�b�g
	if (isOpenDrawer) { position = openStatePos; }
	else { position = closedStatePos; }

	return true;
}

void DrawerSprite::Update()
{
	//�J�ړ�
	Move();

	//�X�v���C�g�X�V
	Sprite::Update();
}

void DrawerSprite::MoveStart()
{
	//�ړ��J�n�����W���Z�b�g
	moveStartPos = position;

	//�^�C�}�[��������
	moveTimer = 0;
	//�J�J�n����Ԃɂ���
	isMoveDrawer = true;

	//�J�̏�Ԃ𔽓]������
	if (isOpenDrawer) { isOpenDrawer = false; }
	else { isOpenDrawer = true; }
}

void DrawerSprite::Reset()
{
	//���W�ƊJ��Ԃ��V�[���J�n��Ԃɖ߂�
	isOpenDrawer = isOpenDrawerSceneStart;
	if (isOpenDrawer) { position = openStatePos; }
	else { position = closedStatePos; }
	isMoveDrawer = false;
}

void DrawerSprite::Move()
{
	//�J�ړ����łȂ���Δ�����
	if (!isMoveDrawer) { return; }

	//�^�C�}�[�X�V
	moveTimer++;
	//���W�ړ��C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = moveTimer / (float)moveTime;

	//�ړ�������
	//�J��
	if (isOpenDrawer) {
		position.x = Easing::OutQuint(moveStartPos.x, openStatePos.x, time);
		position.y = Easing::OutQuint(moveStartPos.y, openStatePos.y, time);
	}
	//����
	else {
		position.x = Easing::OutQuint(moveStartPos.x, closedStatePos.x, time);
		position.y = Easing::OutQuint(moveStartPos.y, closedStatePos.y, time);
	}

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (moveTimer < moveTime) { return; }

	//���������̂ňړ���Ԃ���������
	isMoveDrawer = false;
}
