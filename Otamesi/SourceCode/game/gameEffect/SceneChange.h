#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Sprite.h"

namespace
{
	//�t�F�[�h�^�C�v
	enum class TYPE
	{
		FadeOut = -1,
		Not = 0,
		FadeIn = 1,
	};

	//�t�F�[�h�^�C�v
	enum class SIZEMAX
	{
		Width = 32,
		Height = 18,
	};

	class BlackBox
	{
	public: // �����o�֐�
		/// <summary>
		/// ����
		/// </summary>
		/// <param name="type">�t�F�[�h�^�C�v</param>
		void Spin(TYPE type);
		/// <summary>
		/// �I�t�Z�b�g�^�C�}�[�̐ݒ�
		/// </summary>
		/// <param name="num"></param>
		void AddOffsetTimer(int num) { offsetTimer_ += num; }

	public: // �����o�֐�
		/// <summary>
		/// �I�t�Z�b�g�^�C�}�[�̎擾
		/// </summary>
		/// <returns>�I�t�Z�b�g�^�C�}�[</returns>
		int GetOffsetTimer() { return offsetTimer_; }

	public: // �����o�ϐ�
		std::unique_ptr<Sprite> sprite_;
		int offsetTimer_ = 0;
		// �C�[�W���O�̐i�s�x�p
		float easeTimer_ = 0.0f;
		bool inEnd_ = false;
		bool outEnd_ = false;
	};
}

class SceneChange final
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public: // �T�u�N���X


private:
	SceneChange() = default;
	~SceneChange() = default;

public:
	SceneChange(const SceneChange& fbxFactory) = delete;
	SceneChange& operator=(const SceneChange& fbxFactory) = delete;

public: // �����o�֐�
	/// <summary>
	/// �V���O���g���C���X�^���X�̎擾
	/// </summary>
	/// <returns>�V���O���g���C���X�^���X</returns>
	static SceneChange* GetInstance();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// ���t���[������
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �V�[���`�F���W�J�n
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	void SceneChangeStart(const std::string& sceneName);

public: // �����o�֐�
	/// <summary>
	/// �V�[���C���I���t���O�̎擾
	/// </summary>
	/// <returns>�V�[���C���I���t���O</returns>
	bool GetinEndFlag() { return inEndFlag_; }

private: // �����o�ϐ�
	bool inEndFlag_ = false;
	bool outEndFlag_ = false;

	std::vector<std::vector<std::unique_ptr<BlackBox>>> blackBox_;
	TYPE type_;
	std::string sceneName_;
};