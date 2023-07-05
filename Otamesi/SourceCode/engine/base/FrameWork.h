#pragma once
#include "WindowApp.h"
#include "DirectXBase.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "ShadowMap.h"
#include "SceneManager.h"
#include "AbstractSceneFactory.h"
#include "AfterBloom.h"
#include "OutLine.h"
#include "OutLineDraw.h"

/// <summary>
/// �t���[�����[�N
/// </summary>
class FrameWork
{
public: //�����o�֐�
	/// <summary>
	/// ���s
	/// </summary>
	void Run();

	/// <summary>
	/// ������
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// �I��
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// �Q�[�����[�v�I�����N�G�X�g��getter
	/// </summary>
	/// <returns>�Q�[�����[�v�I�����N�G�X�g</returns>
	virtual bool GetIsEndRequest() { return isEndRequest; }

protected: //�����o�ϐ�
	//�Q�[�����[�v�I�����N�G�X�g
	bool isEndRequest = false;
	//�E�B���h�E�A�v���P�[�V����
	std::unique_ptr<WindowApp> win;
	//DirectX���
	std::unique_ptr<DirectXBase> dxbase;
	//����
	Input* input = nullptr;
	//��
	Audio* audio = nullptr;
	//�X�v���C�g�̋��ʕ���
	SpriteCommon* spriteCommon = nullptr;
	//�f�o�b�O�e�L�X�g
	DebugText* debugText = nullptr;
	//�V���h�E�}�b�v(�e�������C�g���_�p)
	std::unique_ptr<ShadowMap> shadowMap;
	std::unique_ptr<AfterBloom> afterBloom;
	std::unique_ptr<OutLine> outLine;
	std::unique_ptr<OutLineDraw> outLineDraw;
	//�V�[���H��
	std::unique_ptr<AbstractSceneFactory> sceneFactory;
};