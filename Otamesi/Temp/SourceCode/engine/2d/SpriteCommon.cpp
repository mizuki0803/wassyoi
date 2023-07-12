#include "SpriteCommon.h"
#include "DescHeapSRV.h"
#include <cassert>
#include <d3dcompiler.h>
#include <string>
#include <d3dx12.h>
#include <DirectXTex.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

SpriteCommon* SpriteCommon::GetInstance()
{
	static SpriteCommon instance;

	return &instance;
}

void SpriteCommon::Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, int window_width, int window_height)
{
	//nullptr�`�F�b�N
	assert(dev);

	//�����o�ϐ��ɋL�^
	this->dev = dev;
	this->cmdList = cmdList;

	//�X�v���C�g�p�p�C�v���C������
	CreatePipeline();
	CreateSkyDormPipeline();

	//���s���e�̎ˉe�s�񐶐�
	matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);
}

void SpriteCommon::DrawPrev(const std::string &name)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineSet[name].pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipelineSet[name].rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

void SpriteCommon::CreatePipeline()
{
	HRESULT result;

	std::string name = "Default";

	ComPtr<ID3DBlob> vsBlob;		//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;		//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;		//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl",	//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�C���N���[�h�\�ɂ���
		"main", "vs_5_0",	//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�N�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl",	//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�C���N���[�h�\�ɂ���
		"main", "ps_5_0",	//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//���_���C�A�E�g�z��̐錾�Ɛݒ�
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//uv���W
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	//�p�C�v���C���X�e�[�g�̐ݒ�ϐ��̐錾�ƁA�e�퍀�ڂ̐ݒ�
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//�T���v���}�X�N�ƃ��X�^���C�U�X�e�[�g�̐ݒ�
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//�W���ݒ�
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);	//��U�W���l���Z�b�g
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;			//�w�ʃJ�����O�����Ȃ�

	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//��U�W���l���Z�b�g
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//��ɏ㏑�����[��

	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//���ݒ�

	//���ʐݒ�
	blenddesc.BlendEnable = true;	//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��
	//�F����

	//���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

	//���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

	//�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;	//�g��Ȃ�

	//����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	//�\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-�\

	//�[�x�l�t�H�[�}�b�g
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;	//�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	//0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1;	//1�s�N�Z���ɂ�1��T���v�����O

	//�f�X�N���v�^�e�[�u���̐ݒ�
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 ���W�X�^

	//���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV);

	//�e�N�X�`���T���v���[�̐ݒ�
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	//���[�g�V�O�l�`���̐���
	ComPtr<ID3DBlob> rootSigBlob;

	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,
		&errorBlob);

	//���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet[name].rootsignature));

	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet[name].rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet[name].pipelinestate));
}

void SpriteCommon::CreateSkyDormPipeline()
{
	HRESULT result;

	std::string name = "SkyDorm";

	DXGI_FORMAT rtv_format_sky[1]
	{
		DXGI_FORMAT_R8G8B8A8_UNORM
	};

	// �f�X�N���v�^�����W
	CD3DX12_DESCRIPTOR_RANGE desc_range_srv_sky = {};
	desc_range_srv_sky.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t���W�X�^


	// ���[�g�p�����[�^
	CD3DX12_ROOT_PARAMETER rootparams_sky[3] = {};
	rootparams_sky[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams_sky[1].InitAsDescriptorTable(1, &desc_range_srv_sky, D3D12_SHADER_VISIBILITY_ALL);
	rootparams_sky[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL); // �J����

	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC sampler_desc_sky = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT);

	// �f�v�X�X�e���V���X�e�[�g
	D3D12_DEPTH_STENCIL_DESC depth_stencil_state_sky = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	depth_stencil_state_sky.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
	depth_stencil_state_sky.DepthEnable = false;



	// �p�C�v���C���X�e�[�g�����J�n
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphics_pipeline_state{};

	// �T���v���}�X�N
	graphics_pipeline_state.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	// ���X�^���C�U�X�e�[�g
	graphics_pipeline_state.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	graphics_pipeline_state.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	// �f�v�X�X�e���V���X�e�[�g
	D3D12_DEPTH_STENCIL_DESC depth_stencil_state = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	depth_stencil_state.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	depth_stencil_state.DepthEnable = false;
	//�f�v�X�X�e���V���X�e�[�g
	graphics_pipeline_state.DepthStencilState = depth_stencil_state;

	// �u�����h�X�e�[�g�Z�b�g
	D3D12_BLEND_DESC blend_desc{};
	D3D12_RENDER_TARGET_BLEND_DESC render_target_blend_desc{};
	render_target_blend_desc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA�S�Ẵ`�����l����`��
	render_target_blend_desc.BlendEnable = true;
	render_target_blend_desc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	render_target_blend_desc.SrcBlendAlpha = D3D12_BLEND_ONE;
	render_target_blend_desc.DestBlendAlpha = D3D12_BLEND_ZERO;
	render_target_blend_desc.BlendOp = D3D12_BLEND_OP_ADD;
	render_target_blend_desc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	render_target_blend_desc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blend_desc.RenderTarget[0] = render_target_blend_desc;
	graphics_pipeline_state.BlendState = blend_desc;

	// ���_���C�A�E�g�̐ݒ�
	// �[�x�o�b�t�@�̃t�H�[�}�b�g
	graphics_pipeline_state.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// ���_���C�A�E�g�̐ݒ�
	//���_���C�A�E�g�z��̐錾�Ɛݒ�
	D3D12_INPUT_ELEMENT_DESC input_layout_sky[] = {
		{	//xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//uv���W
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	graphics_pipeline_state.InputLayout.pInputElementDescs = input_layout_sky;
	graphics_pipeline_state.InputLayout.NumElements = sizeof(input_layout_sky) / sizeof(input_layout_sky[0]);

	// �v���~�e�B�u�ݒ�
	graphics_pipeline_state.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;



	// �`��ݒ�
	graphics_pipeline_state.NumRenderTargets = sizeof(rtv_format_sky) / sizeof(rtv_format_sky[0]);
	for (int i = 0; i < sizeof(rtv_format_sky) / sizeof(rtv_format_sky[0]); ++i)
	{
		graphics_pipeline_state.RTVFormats[i] = rtv_format_sky[i];
	}
	graphics_pipeline_state.SampleDesc.Count = 1;	// 1�s�N�Z���ɂ�1�T���v�����O


	// ���[�g�V�O�l�`���̐ݒ�
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC root_signature_desc{};

	// �X�^�e�B�b�N�T���v���[
	root_signature_desc.Init_1_0(
		sizeof(rootparams_sky) / sizeof(rootparams_sky[0]),
		rootparams_sky, 
		1, &sampler_desc_sky, 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);



	ComPtr<ID3DBlob> vsBlob;		//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;		//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;		//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParanoiaSkyDomeVS.hlsl",	//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�C���N���[�h�\�ɂ���
		"main", "vs_5_0",	//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�N�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParanoiaSkyDomePS.hlsl",	//�V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�C���N���[�h�\�ɂ���
		"main", "ps_5_0",	//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//�f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlob����G���[���e��string�^�ɃR�s�[
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//�G���[���e���o�̓E�C���h�E�ɕ\��
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}


	//�p�C�v���C���X�e�[�g�̐ݒ�ϐ��̐錾�ƁA�e�퍀�ڂ̐ݒ�
	//�O���t�B�b�N�X�p�C�v���C���ݒ�
	graphics_pipeline_state.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	graphics_pipeline_state.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;

	// �o�[�W������������̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(
		&root_signature_desc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// ���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(
		0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(pipelineSet[name].rootsignature.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	graphics_pipeline_state.pRootSignature = pipelineSet[name].rootsignature.Get();

	// �O���t�B�b�N�X�p�C�v���C���̐���
	result = dev->CreateGraphicsPipelineState(
		&graphics_pipeline_state,
		IID_PPV_ARGS(pipelineSet[name].pipelinestate.ReleaseAndGetAddressOf()));
	assert(SUCCEEDED(result));

}
