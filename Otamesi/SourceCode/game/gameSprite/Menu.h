#pragma once
#include "Sprite.h"
#include "EaseData.h"
#include <memory>

class Menu : public Sprite
{
public: //�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <returns></returns>
	static std::unique_ptr<Menu> Create(Vector2 pos);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="position">���W</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���]���邩</param>
	/// <param name="isFlipY">�㉺���]���邩</param>
	/// <returns>����</returns>
	bool Initialize(const Texture& texture, const Vector2& position, const Vector2& anchorpoint, bool isFlipX, bool isFlipY) override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();

	const bool EaseEnd() {return menuEaseData_->GetEndFlag(); }

private: //�����o�ϐ�
	// �C�[�W���O
	std::unique_ptr<EaseData> menuEaseData_;
};

