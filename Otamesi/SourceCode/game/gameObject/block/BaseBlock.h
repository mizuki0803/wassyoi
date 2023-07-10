#pragma once
#include "InstanceObject.h"
#include <array>

class BaseBlock
{
private:

	enum class BROCK_MODEL_TYPE {
		block,
		plane,
		goal,
		player,
		size,
	};

	enum class BROCK_TYPE {
		block,
		plane = 10,
		goal,
		player,
		size,
	};

	//obj���f��
	std::array<std::unique_ptr<ObjModel>, int(BROCK_MODEL_TYPE::size)> model;

	//�`��p�N���X
	std::array<std::unique_ptr<InstanceObject>, int(BROCK_TYPE::size)> block;

public:

	/// <summary>
	/// ������
	/// </summary>
	void StaticInitialize();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="_blockType">�u���b�N���</param>
	/// <param name="_pos">���W</param>
	/// <param name="_scale">�傫��</param>
	/// <param name="_rotation">��]�p</param>
	/// <param name="_color">�F</param>
	void Update(const int _blockType, const XMFLOAT3& _pos, const XMFLOAT3& _scale,
		const XMFLOAT3& _rotation, const XMFLOAT4& _color = { 1.0f,1.0f,1.0f,1.0f }, const XMMATRIX* _parentWorld = nullptr);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

};

