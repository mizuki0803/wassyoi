#pragma once
#include "SceneChangeSprite.h"
#include <list>
#include <memory>

/// <summary>
/// �V�[���ύX���o
/// </summary>
class SceneChangeEffect
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	static void Update();

	/// <summary>
	/// ��ĕ`��
	/// </summary>
	static void Draw();

	/// <summary>
	/// �V�[���ύX���o���쐬����
	/// </summary>
	/// <param name="color">��ʂ𕢂��F</param>
	/// <param name="deepenTime">�F��Z�����鎞��</param>
	/// <param name="waitTime">�F��Z�������܂ܑҋ@���鎞��</param>
	/// <param name="returnTime">�F��߂�����</param>
	static void CreateSceneChangeEffect(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime);

private: //�ÓI�����o�ϐ�
	//�V�[���ύX���o�p�X�v���C�g
	static std::list<std::unique_ptr<SceneChangeSprite>> sceneChangeSprites;
};
