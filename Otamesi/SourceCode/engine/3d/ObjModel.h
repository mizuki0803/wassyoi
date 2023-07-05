#pragma once
#include "Texture.h"
#include <Windows.h>
#include <DirectXMath.h>
#include "DirectXBase.h"
#include <unordered_map>

/// <summary>
/// obj���f��
/// </summary>
class ObjModel
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;	//xyz���W
		XMFLOAT3 normal;//�@���x�N�g��
		XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����B1
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	//�A���r�G���g�W��
		float pad1;			//�p�f�B���O
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�W��
		float pad2;			//�p�f�B���O
		XMFLOAT3 specular;	//�X�y�L�����[�W��
		float alpha;		//�A���t�@
	};

	//�}�e���A��
	struct Material
	{
		std::string name;	//�}�e���A����
		XMFLOAT3 ambient;	//�A���r�G���g�e���x
		XMFLOAT3 diffuse;	//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;	//�X�y�L�����[�e���x
		float alpha;	//�A���t�@
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// obj�t�@�C������3D���f����ǂݍ���
	/// </summary>
	/// <param name="modelname">���f����</param>
	/// <param name="smoothing">�X���[�W���O�������邩</param>
	/// <returns>obj���f��</returns>
	static ObjModel* LoadFromOBJ(const std::string& modelname, const bool smoothing = false);

	//setter
	static void SetDevice(ID3D12Device* device) { ObjModel::dev = device; };
	static void SetShadowMapTexture(const Texture& shadowMapTexture) { ObjModel::shadowMapTexture = shadowMapTexture; };

private: //�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* dev;
	//�e�p�[�x�e�N�X�`��
	static Texture shadowMapTexture;

private: //����J�̃����o�֐�
	/// <summary>
	/// OBJ�t�@�C���ǂݍ���
	/// </summary>
	/// <param name="modelname">���f����</param>
	/// <param name="smoothing">�X���[�W���O�������邩</param>
	void LoadFromOBJInternal(const std::string& modelname, const bool smoothing);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	/// <param name="directoryPath">�p�X</param>
	/// <param name="filename">�t�@�C����</param>
	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// �e��o�b�t�@����
	/// </summary>
	void CreateBuffers();

	/// <summary>
	/// ���_�f�[�^�̐����擾
	/// </summary>
	/// <returns>���_�f�[�^�̐�</returns>
	inline size_t GetVertexCount() { return vertices.size(); }

	/// <summary>
	/// �G�b�W�������f�[�^�̒ǉ�
	/// </summary>
	/// <param name="indexPosition">���W�C���f�b�N�X</param>
	/// <param name="indexVertex">���_�C���f�b�N�X</param>
	void AddSmootgData(const unsigned short indexPosition, const unsigned short indexVertex);

	/// <summary>
	/// ���������ꂽ���_�@���̌v�Z
	/// </summary>
	void CalculateSmoothedVertexNormals();


public: //�����o�֐�
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootOaramIndexMaterial">���[�g�p�����[�^�̐���</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial, UINT _instanceDrawNum = 1);

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootOaramIndexMaterial">���[�g�p�����[�^�̐���</param>
	void DrawLightCameraView(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial, UINT _instanceDrawNum = 1);

	/// <summary>
	/// ���Z�����p
	/// </summary>
	/// <param name="cmdList"></param>
	/// <param name="rootParamIndexMaterial"></param>
	/// <param name="_instanceDrawNum"></param>
	void DrawAdd(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndexMaterial, UINT _instanceDrawNum = 1);

private: //�����o�ϐ�
	//�e�N�X�`��
	Texture texture;
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUv> vertices;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	//�萔�o�b�t�@(�}�e���A��)
	ComPtr<ID3D12Resource> constBuffB1;
	//�}�e���A��
	Material material;
	//���_�@���X���[�W���O�p�f�[�^
	std::unordered_map<unsigned short, std::vector<unsigned short>> smoothData;
};