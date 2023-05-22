#pragma once
#include "Block.h"

/// <summary>
/// �}�b�v�p�n���{�e(�u���b�N)
/// </summary>
class PlaneBlock : public Block
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>	
	/// <param name="model">���f��</param>
	/// <param name="mapChipNum">�}�b�v�`�b�v�̔ԍ�</param>
	/// <param name="rotation">�X����p�x</param>
	/// <returns>�u���b�N</returns>
	static PlaneBlock* Create(ObjModel* model, const XMINT3& mapChipNum, const Vector3& rotation);

public:
	/// <summary>
	/// �����֐�
	/// </summary>
	void aaaa();

	//getter
	static float GetBlockSize() { return Block::blockSize; }

};