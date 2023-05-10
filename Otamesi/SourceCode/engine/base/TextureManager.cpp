#include "TextureManager.h"
#include "DescHeapSRV.h"
#include "DirectXBase.h"
#include <cassert>
#include <DirectXTex.h>

using namespace DirectX;
using namespace Microsoft::WRL;

using namespace std;

ID3D12Device* TextureManager::dev = nullptr;

void TextureManager::LoadTexture(Texture& texture, const std::string& directoryPath, const std::string& filename)
{
	HRESULT result;

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//�t�@�C���p�X������
	string filepath = directoryPath + filename;

	//���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1,
		wfilepath, _countof(wfilepath));

	//�t�@�C�����Ɗg���q�𕪗�����
	std::string exceptExt;
	std::string fileExt;
	//��؂蕶�� '.'���o�Ă����ԍŌ�̕���������
	size_t extDotPos = filepath.rfind('.');
	//�������q�b�g������
	if (extDotPos != std::string::npos) {
		//��؂蕶���̌����t�@�C���g���q�Ƃ��ĕۑ�
		fileExt = filepath.substr(extDotPos + 1, filepath.size() - extDotPos - 1);
		//��؂蕶���̑O�܂ł𔲂��o��
		exceptExt = filepath.substr(0, extDotPos);
	}
	else {
		fileExt = "";
		exceptExt = filepath;
	}

	//dds�t�@�C�������̑��`���t�@�C�����Ő؂�ւ�
	if (fileExt == "dds") {
		result = LoadFromDDSFile(wfilepath, DDS_FLAGS_NONE, &metadata, scratchImg);
	}
	else {
		result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);
	}
	assert(SUCCEEDED(result));

	//���f�[�^���o
	const Image* img = scratchImg.GetImage(0, 0, 0);

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���p�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texture.texBuff));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = texture.texBuff->WriteToSubresource(
		0,
		nullptr,	//�S�̈�R�s�[
		img->pixels,	//���f�[�^�A�h���X
		(UINT)img->rowPitch,	//1���C���T�C�Y
		(UINT)img->slicePitch	//1���T�C�Y
	);

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = metadata.format;	//RGBA
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	DescHeapSRV::CreateShaderResourceView(srvDesc, texture);
}
