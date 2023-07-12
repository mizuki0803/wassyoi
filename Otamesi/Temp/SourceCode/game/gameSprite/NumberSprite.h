#pragma once
#include "Sprite.h"

/// <summary>
/// �����X�v���C�g
/// </summary>
class NumberSprite : public Sprite
{
public:
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="position">���W</param>
	/// <param name="size">�傫��</param>
	/// <param name="texSize">�e�N�X�`���T�C�Y</param>
	/// <returns>�����X�v���C�g</returns>
	static NumberSprite* Create(const Texture& texture, const Vector2& position, const Vector2& size, const Vector2& texSize);

public: //�����o�֐�
	/// <summary>
	/// �\������ԍ����Z�b�g
	/// </summary>
	/// <param name="number">�\������ԍ�(0�`9, 10��.)</param>
	void SetNumber(const int number);
};