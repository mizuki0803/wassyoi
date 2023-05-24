#pragma once
#include "Sprite.h"

/// <summary>
/// �V�[���ύX���o�p�X�v���C�g
/// </summary>
class SceneChangeSprite : public Sprite
{
public:
	//�F�̔Z���ύX�t�F�[�Y
	enum class IntensityChangePhase {
		Deepen, //�Z������
		Wait,	//�F��Z�������܂ܑҋ@����
		Return, //��������(�߂�)
	};

public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="color">��ʂ𕢂��F</param>
	/// <param name="deepenTime">�F��Z�����Ă�������</param>
	/// <param name="waitTime">�F��Z�������܂ܑҋ@���鎞��</param>
	/// <param name="returnTime">�F�𔖂����Ă�������</param>
	/// <returns>�V�[���ύX���o�p�X�v���C�g</returns>
	static SceneChangeSprite* Create(const XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���]���邩</param>
	/// <param name="isFlipY">�㉺���]���邩</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	//getter
	bool GetIsDead() { return isDead; }


private: //�����o�֐�
	/// <summary>
	/// �F��Z������
	/// </summary>
	void ColorDeepen();

	/// �F��Z�������܂ܑҋ@����
	/// </summary>
	void ColorWait();

	/// <summary>
	/// �Z�������F��߂�
	/// </summary>
	void ColorReturn();


private: //�ÓI�����o�ϐ�
	//�F�̔Z���ύX�����J��
	static void (SceneChangeSprite::* intensityChangeActionFuncTable[])();

private: //�����o�ϐ�
	//�F�̔Z���ύX�t�F�[�Y
	IntensityChangePhase phase = IntensityChangePhase::Deepen;
	//�F��Z�����鎞��
	int32_t deepenTime = 0;
	//�F��Z�������܂ܑҋ@���鎞��
	int32_t waitTime = 0;
	//�Z�������F��߂�����
	int32_t returnTime = 0;
	//�F�̔Z����ύX����^�C�}�[
	int32_t intensityChangeTimer = 0;
	//���S�t���O
	bool isDead = false;
};