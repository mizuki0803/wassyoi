#include "SceneChangeSprite.h"
#include "SpriteTextureLoader.h"
#include "WindowApp.h"
#include "Easing.h"

void (SceneChangeSprite::* SceneChangeSprite::intensityChangeActionFuncTable[])() = {
	&SceneChangeSprite::ColorDeepen,
	&SceneChangeSprite::ColorWait,
	&SceneChangeSprite::ColorReturn,
};

SceneChangeSprite* SceneChangeSprite::Create(const XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime)
{
	//�V�[���ύX���o�p�X�v���C�g�̃C���X�^���X�𐶐�
	SceneChangeSprite* sceneChangeSprite = new SceneChangeSprite();
	if (sceneChangeSprite == nullptr) {
		return nullptr;
	}

	// ������
	if (!sceneChangeSprite->Initialize(SpriteTextureLoader::GetTexture(SpriteTextureLoader::White), {0, 0}, false, false)) {
		delete sceneChangeSprite;
		assert(0);
		return nullptr;
	}

	//�F���Z�b�g
	sceneChangeSprite->color = { color.x, color.y, color.z, 0 };

	//���Ԃ��Z�b�g
	sceneChangeSprite->deepenTime = deepenTime;
	sceneChangeSprite->waitTime = waitTime;
	sceneChangeSprite->returnTime = returnTime;

	return sceneChangeSprite;
}

bool SceneChangeSprite::Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	//�X�v���C�g������
	if (!Sprite::Initialize(texture, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	//��ʑS�̂𕢂��̂ɕK�v�ȏ����Z�b�g
	position = { 0, 0 };
	size = { WindowApp::window_width, WindowApp::window_height };
	texSize = { 1, 1 };

	return true;
}

void SceneChangeSprite::Update()
{
	//�F�̔Z���ύX����
	(this->*intensityChangeActionFuncTable[static_cast<size_t>(phase)])();

	//�X�v���C�g�X�V
	Sprite::Update();
}

void SceneChangeSprite::ColorDeepen()
{
	//�^�C�}�[�X�V
	intensityChangeTimer++;
	//�C�[�W���O�p�^�C��
	const float time = (float)intensityChangeTimer / deepenTime;

	//���񂾂�F��Z�����Ă���
	color.w = Easing::LerpFloat(0, 1, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (intensityChangeTimer >= deepenTime) {
		//���̃t�F�[�Y��
		phase = IntensityChangePhase::Wait;

		//�^�C�}�[������
		intensityChangeTimer = 0;
	}
}

void SceneChangeSprite::ColorWait()
{
	//�^�C�}�[�X�V
	intensityChangeTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (intensityChangeTimer >= waitTime) {
		//���̃t�F�[�Y��
		phase = IntensityChangePhase::Return;

		//�^�C�}�[������
		intensityChangeTimer = 0;
	}
}

void SceneChangeSprite::ColorReturn()
{
	//�^�C�}�[�X�V
	intensityChangeTimer++;
	//�C�[�W���O�p�^�C��
	const float time = (float)intensityChangeTimer / returnTime;

	//���񂾂�F�𔖂����Ă���
	color.w = Easing::LerpFloat(1, 0, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (intensityChangeTimer >= returnTime) {
		//���S�t���O�𗧂Ă�
		isDead = true;
	}
}
