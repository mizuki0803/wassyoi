#pragma once
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <string>

/// <summary>
/// �}�b�v�f�[�^
/// </summary>
class MapData
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMINT3 = DirectX::XMINT3;

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	//getter
	const XMINT3& GetMapSize() { return mapSize; }
	const std::vector<std::vector<std::vector<int>>>& GetMapChipNum() { return mapChipNum; }

protected: //�����o�֐�
	/// <summary>
	/// �}�b�v����
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	bool CreateMap(const int selectStageNum);

	/// <summary>
	/// �}�b�v�f�[�^�ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	bool LoadMapData(const std::string& fileName);

	/// <summary>
	/// �}�b�v�u���b�N����
	/// </summary>
	virtual void CreateMapBlock() = 0;

protected: //�����o�ϐ�
	//�}�b�v�̃T�C�Y
	XMINT3 mapSize;
	//3�����}�b�v�p��3�����z��
	std::vector<std::vector<std::vector<int>>> mapChipNum;
};