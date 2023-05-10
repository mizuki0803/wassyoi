#pragma once
#include "BaseScene.h"
#include "AbstractSceneFactory.h"

/// <summary>
/// �V�[���Ǘ�
/// </summary>
class SceneManager final
{
private: //�V���O���g����
	//�R���X�g���N�^���B��
	SceneManager() = default;
	//�f�X�g���N�^���B��
	~SceneManager();
public:
	//�R�s�[�R���X�g���N�^�𖳌���
	SceneManager(const SceneManager& sceneManager) = delete;
	//������Z�q�𖳌���
	void operator = (const SceneManager& sceneManager) = delete;


public: //�����o�֐�
	/// <summary>
	/// �C���X�^���X���擾
	/// </summary>
	/// <returns>�V�[���}�l�[�W���[</returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite();

	/// <summary>
	/// 3D�V�[���`��
	/// </summary>
	void Draw3D();

	/// <summary>
	/// 3D�V�[���`��(���C�g���猩�����_)
	/// </summary>
	void Draw3DLightView();

	/// <summary>
	/// �O�i�X�v���C�g�`��
	/// </summary>
	void DrawFrontSprite();

	/// <summary>
	/// ���̃V�[�����Z�b�g
	/// </summary>
	/// <param name="sceneName">���̃V�[��</param>
	void ChangeScene(const std::string& sceneName);

	/// <summary>
	/// �V�[���H����Z�b�g
	/// </summary>
	/// <param name="sceneFactory">�V�[���H��</param>
	void SetSceneFactory(AbstractSceneFactory* sceneFactory) { this->sceneFactory = sceneFactory; }

private: //�����o�ϐ�
	//���̃V�[��
	BaseScene* scene = nullptr;
	//���̃V�[��
	BaseScene* nextScene = nullptr;
	//�V�[���H��
	AbstractSceneFactory* sceneFactory = nullptr;
};

