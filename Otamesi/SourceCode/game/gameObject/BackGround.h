#pragma once
#include "InstanceObject.h"

class BackGround
{
private:

	enum class transform {
		translation,
		rotation,
		scaling,
		size,
	};

public:

	/// <summary>
	/// ����
	/// </summary>
	/// <returns></returns>
	static BackGround* Create();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	//obj���f��
	std::unique_ptr<ObjModel> model;

	//�`��p�N���X
	std::unique_ptr<InstanceObject> block;

	//�I�u�W�F�N�g�̏��
	std::vector<std::array<DirectX::XMFLOAT3, int(transform::size)>> info;
};
