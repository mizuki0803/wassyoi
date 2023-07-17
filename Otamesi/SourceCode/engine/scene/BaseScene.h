#pragma once
#include <DirectXBase.h>

class SceneManager;

/// <summary>
/// ���V�[��
/// </summary>
class BaseScene
{
public: //�����o�֐�
	/// <summary>
	/// ���z�f�X�g���N�^
	/// </summary>
	virtual ~BaseScene() = default;

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// �I������
	/// </summary>
	virtual void Finalize() = 0;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	virtual void DrawBackSprite() = 0;

	/// <summary>
	/// 3D�V�[���`��
	/// </summary>
	virtual void Draw3D() = 0;

	/// <summary>
	/// �ォ��u���[���`��
	/// </summary>
	virtual void AfterBloomDraw() {};

	virtual void OutLineDraw() {};

	/// <summary>
	/// 3D�V�[���`��(���C�g���猩�����_)
	/// </summary>
	virtual void Draw3DLightView() = 0;

	/// <summary>
	/// �O�i�X�v���C�g�`��
	/// </summary>
	virtual void DrawFrontSprite() = 0;

	///
	virtual void DrawImageForUI() {};
	/// <summary>
	/// frame���Ƃ̏�����
	/// </summary>
	virtual void FrameReset() = 0;

	//getter
	bool GetIsEndRequest() { return isEndRequest; }

	//setter
	static void SetDirectXbase(DirectXBase* dxbase) { BaseScene::dxbase = dxbase; }

protected: //�ÓI�����o�ϐ�
	static DirectXBase* dxbase;

protected: //�����o�ϐ�
	//�Q�[�����[�v�I�����N�G�X�g
	bool isEndRequest = false;
};

