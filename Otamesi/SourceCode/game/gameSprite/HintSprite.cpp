#include "HintSprite.h"
#include "Easing.h"
#include "WindowApp.h"

const float HintSprite::normalTextureScale = 0.15f;
const float HintSprite::largeTextureScale = 0.7f;


HintSprite* HintSprite::Create(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint)
{
	//�C���X�^���X����
	HintSprite* instance = new HintSprite();
	if (instance == nullptr) {
		return nullptr;
	}

	//����������
	if (!instance->Initialize(texture, parent, position, anchorpoint)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool HintSprite::Initialize(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint)
{
	//�X�v���C�g����
	if (!Sprite::Initialize(texture, position, anchorpoint, false, false)) {
		return false;
	}
	this->parent = parent;

	//�ۑ��p�����Z�b�g
	parentStorage = parent;
	positionStorage = position;
	anchorpointStorage = anchorpoint;

	//�傫�����Z�b�g
	SetScale(normalTextureScale);

	return true;
}

void HintSprite::Update()
{
	//�傫���ύX
	SizeChange();

	//�X�v���C�g�X�V
	Sprite::Update();
}

void HintSprite::SizeChangeStart()
{
	//�傫����ύX���̏ꍇ�͔�����
	if (isSizeChange) { return; }

	//�傫�����ʏ�̏ꍇ�́A�傫������
	if (!isSizeLarger) {

		//�ړ��O���W���Z�b�g
		sizeChangeBeforePos = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1] };
		//�ړ�����W���Z�b�g
		sizeChangeAfterPos = { WindowApp::window_width / 2, WindowApp::window_height / 2 };
		//�e�Ƃ͕ʂ̓���������̂ŁA�e�q�\��������
		parent = nullptr;

		//�T�C�Y���傫����ԂɕύX
		isSizeLarger = true;
	}
	//�傫�����傫���ꍇ�́A���ɖ߂�
	else {
		//�e�q�\���𕜊�������
		parent = parentStorage;
		//�ړ��O���W���Z�b�g
		sizeChangeBeforePos = position - parent->GetPosition();
		//�ړ�����W���Z�b�g
		sizeChangeAfterPos = positionStorage;

		//�T�C�Y����������ԂɕύX
		isSizeLarger = false;
	}

	//�傫����ύX��Ԃɂ���
	isSizeChange = true;
	//�^�C�}�[��������
	actionTimer = 0;
}

void HintSprite::SizeChange()
{
	//�傫���ύX���łȂ���Δ�����
	if (!isSizeChange) { return; }

	//�^�C�}�[�X�V
	actionTimer++;
	const float moveTime = 30; //���W�ړ��ɂ����鎞��

	//�C�[�W���O�Ɏg�p����ϐ�(0�`1���Z�o)
	const float time = actionTimer / moveTime;
	//�ړ�������
	position.x = Easing::OutQuint(sizeChangeBeforePos.x, sizeChangeAfterPos.x, time);
	position.y = Easing::OutQuint(sizeChangeBeforePos.y, sizeChangeAfterPos.y, time);
	
	float size;	//�傫���ύX�p�ϐ�
	//�傫������ꍇ
	if (isSizeLarger) {
		//�傫����ύX
		size = Easing::OutQuint(normalTextureScale, largeTextureScale, time);

		//�A���J�[�|�C���g�ύX
		anchorpoint.x = Easing::OutQuint(anchorpointStorage.x, 0.5f, time);
		anchorpoint.y = Easing::OutQuint(anchorpointStorage.y, 0.5f, time);
	}
	//���ɖ߂��ꍇ
	else {
		//�傫����ύX
		size = Easing::OutQuint(largeTextureScale, normalTextureScale, time);

		//�A���J�[�|�C���g�ύX
		anchorpoint.x = Easing::OutQuint(0.5f, anchorpointStorage.x, time);
		anchorpoint.y = Easing::OutQuint(0.5f, anchorpointStorage.y, time);
	}
	SetScale(size);
	

	//�^�C�}�[���w�肵�����Ԃɖ����Ȃ���Δ�����
	if (actionTimer < moveTime) { return; }

	//�傫���ύX��Ԃ�����
	isSizeChange = false;
}
