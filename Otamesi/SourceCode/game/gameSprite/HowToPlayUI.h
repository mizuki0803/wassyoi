#pragma once
#include "Sprite.h"
#include <memory>
#include <vector>

/// <summary>
/// ������@UI
/// </summary>
class HowToPlayUI
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="isDrawPause">������@�u�|�[�Y�v�̕`������邩</param>
	/// <returns>������@UI</returns>
	static HowToPlayUI* Create(bool isDrawPause);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="isDrawPause">������@�u�|�[�Y�v�̕`������邩</param>
	void Initialize(bool isDrawPause);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private: //�����o�֐�
	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="texture">�g�p����e�N�X�`��</param>
	/// <param name="position">���W</param>
	/// <param name="texSize">�e�N�X�`���T�C�Y</param>
	/// <param name="sizeRatio">�e�N�X�`���T�C�Y�ɑ΂���T�C�Y�̔䗦</param>
	void CreateNewSprite(const Texture& texture, Vector2& position, const Vector2& texSize, const float sizeRatio);

private: //�����o�ϐ�
	//������@�X�v���C�g
	std::vector<std::unique_ptr<Sprite>> sprites;
};