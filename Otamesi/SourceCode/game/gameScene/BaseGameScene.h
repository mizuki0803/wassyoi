#pragma once
#include "BaseScene.h"
#include <DirectXMath.h>

class GameCamera;
class Player;

/// <summary>
/// �Q�[���ŗL�̊��V�[��
/// </summary>
class BaseGameScene : public BaseScene
{
protected: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

protected: //�����o�֐�
	/// <summary>
	/// �V�[���ύX�����Z�b�g���A�V�[���ύX���J�n����
	/// </summary>
	/// <param name="color">��ʂ𕢂��F</param>
	/// <param name="deepenTime">�F��Z�����鎞��</param>
	/// <param name="waitTime">�F��Z�������܂ܑҋ@���鎞��</param>
	/// <param name="returnTime">�F��߂�����</param>
	/// <param name="nextSceneName">�V�[���ύX��̃V�[����</param>
	void SceneChangeStart(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t waitTime, int32_t returnTime, const std::string& nextSceneName);

	/// <summary>
	/// �V�[���ύX���
	/// </summary>
	void SceneChangeMode();

	/// <summary>
	/// binary�폜
	/// </summary>
	void DeleteBinary();

	/// <summary>
	/// ��Ԃ̕ۑ�
	/// </summary>
	/// <param name="_camera">�J�����N���X</param>
	/// <param name="_player">�v���C���[�N���X</param>
	void KeepBinary(GameCamera& _camera, Player& _player);

	/// <summary>
	/// undo
	/// </summary>
	/// <param name="_camera">�J�����N���X</param>
	/// <param name="_player">�v���C���[�N���X</param>
	void Undo(GameCamera* _camera, Player* _player);

	/// <summary>
	/// redo
	/// </summary>
	/// <param name="_camera">�J�����N���X</param>
	/// <param name="_player">�v���C���[�N���X</param>
	void Redo(GameCamera* _camera, Player* _player);

protected: //�����o�ϐ�
	//�V�[���ύX�ɂ����鎞��
	int32_t sceneChangeTime = 0;
	//�V�[���ύX�J�n����̃^�C�}�[
	int32_t sceneChangeTimer = 0;
	//�V�[���ύX��Ԃ�
	bool isSceneChange = false;
	//���̃V�[��
	std::string nextSceneName;
#pragma region
	//���݂̎菇
	int orderNum;
	//���݂̍ő�菇
	int orderMaxNum;
	//�������̍ő�ԍ�
	int deleteOrderMaxNum;
#pragma endregion undo redo�p

};
