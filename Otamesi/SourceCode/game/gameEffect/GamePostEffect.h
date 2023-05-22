#pragma once
#include "PostEffect.h"
#include <memory>

/// <summary>
/// �Q�[���Ŏg�p����I�u�W�F�N�g���ŕύX�\�ȃ|�X�g�G�t�F�N�g�N���X
/// </summary>
class GamePostEffect
{
public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// �`��
	/// </summary>
	static void Draw();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	static void DrawScenePrev();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	static void DrawSceneRear();

	//getter
	static PostEffect* GetPostEffect() { return GamePostEffect::postEffect.get(); }

private: //�ÓI�����o�ϐ�
	//�|�X�g�G�t�F�N�g
	static std::unique_ptr<PostEffect> postEffect;
};