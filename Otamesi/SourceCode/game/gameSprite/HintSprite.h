#pragma once
#include "DrawerSprite.h"

/// <summary>
/// �q���g�X�v���C�g
/// </summary>
class HintSprite : public Sprite
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="parent">�����o���e�X�v���C�g</param>
	/// <param name="position">���W</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <returns>�q���g�X�v���C�g</returns>
	static HintSprite* Create(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="parent">�����o���e�X�v���C�g</param>
	/// <param name="position">���W</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, DrawerSprite* parent, const Vector2 position, const Vector2& anchorpoint);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �傫���ύX�J�n
	/// </summary>
	void SizeChangeStart();

	//getter
	DrawerSprite* GetParentStorage() { return parentStorage; }
	bool GetIsSizeLarger() { return isSizeLarger; }
	bool GetIsSizeChange() { return isSizeChange; }

public: //�����o�֐�
	/// <summary>
	/// �傫���ύX
	/// </summary>
	void SizeChange();

private: //�����o�ϐ�
	//�e�q�\�������������肷��̂ŁA�ۑ��p�����o���e�I�u�W�F�N�g
	DrawerSprite* parentStorage;
	//�ۑ��p���W
	Vector2 positionStorage;
	//�ۑ��p�A���J�[�|�C���g
	Vector2 anchorpointStorage;
	//�g��\������
	bool isSizeLarger = false;
	//�傫���ύX����
	bool isSizeChange = false;
	//�A�N�V�����^�C�}�[
	int32_t actionTimer = 0;
	//�傫���ύX�O���W
	Vector2 sizeChangeBeforePos;
	//�傫���ύX����W
	Vector2 sizeChangeAfterPos;
};
