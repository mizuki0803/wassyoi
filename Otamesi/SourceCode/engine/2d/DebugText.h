#pragma once
#include "Sprite.h"
#include <string>


/// <summary>
/// �f�o�b�O�e�L�X�g
/// </summary>
class DebugText final
{
public: //�ÓI�����o�ϐ�
	static const int maxCharCount = 256;	//1�t���[���ł̍ő�\��������
	static const int fontWidth = 9;			//�t�H���g�摜��1�������̉���
	static const int fontHeight = 18;		//�t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;	//�t�H���g�摜��1�s���̕�����

private: //�V���O���g����
	//�R���X�g���N�^���B��
	DebugText() = default;
	//�f�X�g���N�^���B��
	~DebugText();
public:
	//�R�s�[�R���X�g���N�^�𖳌���
	DebugText(const DebugText& debugText) = delete;
	//������Z�q�𖳌���
	void operator = (const DebugText& debugText) = delete;

public: //�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�f�o�b�O�e�L�X�g</returns>
	static DebugText* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	void Initialize(const Texture& texture);

	/// <summary>
	/// �e�L�X�g��������
	/// </summary>
	/// <param name="text">�e�L�X�g</param>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="scale">�傫��</param>
	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	/// <summary>
	/// ��ĕ`��
	/// </summary>
	void DrawAll();

private: //�����o�ϐ�
	//�X�v���C�g�̔z��
	Sprite* sprites[maxCharCount];
	//�X�v���C�g�z��̓Y�����ԍ�
	int spriteIndex = 0;
};