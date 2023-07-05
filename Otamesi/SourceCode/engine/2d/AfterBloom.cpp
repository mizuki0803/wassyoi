#include "AfterBloom.h"

#include "WindowApp.h"
#include "DescHeapSRV.h"
#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

ID3D12Device *AfterBloom::dev = nullptr;
ID3D12GraphicsCommandList *AfterBloom::cmdList = nullptr;
PipelineSet AfterBloom::pipelineSet;

AfterBloom *AfterBloom::Create()
{
	//�C���X�^���X�𐶐�
	AfterBloom *instance = new AfterBloom();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool AfterBloom::Initialize()
{
	//�p�C�v���C������
	CreateGraphicsPipelineState();

	HRESULT result;

	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{ -1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},	//����
		{{ -1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},	//����
		{{ +1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},	//�E��
		{{ +1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},	//�E��
	};

	//���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	////�萔�o�b�t�@�̐���
	//result = dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) * 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuff));
	//assert(SUCCEEDED(result));

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WindowApp::window_width,
		(UINT)WindowApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���	
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
		IID_PPV_ARGS(&baseTexture.texBuff)
	);
	baseTexture.texBuff->SetName(L"AfterBloomDraw");

	assert(SUCCEEDED(result));

	//�e�N�X�`����ԃN���A
	{
		//��f��(1280x720 = 921600�s�N�Z��)
		const UINT pixelCount = WindowApp::window_width * WindowApp::window_height;
		//�摜1�s���̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * WindowApp::window_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * WindowApp::window_height;
		//�摜�C���[�W
		UINT *img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) {
			img[i] = 0xff0000ff;
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = baseTexture.texBuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;
	//�f�X�N���v�^�q�[�v��SRV�쐬
	DescHeapSRV::CreateShaderResourceView(srvDesc, baseTexture);

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��RTV����
	dev->CreateRenderTargetView(baseTexture.texBuff.Get(),
		nullptr,
		descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WindowApp::window_width,
			WindowApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//�[�x�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));

	//DSV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	////DSV�p�f�X�N���v�^�q�[�v�𐶐�
	//result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	//assert(SUCCEEDED(result));

	////�f�X�N���v�^�q�[�v��DSV�쐬
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//dev->CreateDepthStencilView(depthBuff.Get(),
	//	&dsvDesc,
	//	descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	return true;
}

void AfterBloom::Draw()
{


	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	DescHeapSRV::SetGraphicsRootDescriptorTable(1, baseTexture.texNumber);

	//�|���S���̕`��(4���_�Ŏl�p�`)
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void AfterBloom::DrawScenePrev(ID3D12DescriptorHeap *descHeapDSV)
{
	//���\�[�X�o���A��ύX(�V�F�[�_���\�[�X���`��\)
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(baseTexture.texBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		WindowApp::window_width, WindowApp::window_height));
	//�V�U�����O��`�̐ݒ�
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WindowApp::window_width,
		WindowApp::window_height));

	//�S��ʃN���A
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	////�[�x�o�b�t�@�̃N���A
	//cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
	//	nullptr);
}

void AfterBloom::DrawSceneRear()
{
	//���\�[�X�o���A��ύX(�`��\���V�F�[�_���\�[�X)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(baseTexture.texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void AfterBloom::CreateGraphicsPipelineState()
{
	HRESULT result;

	ComPtr<ID3DBlob> vsBlob;		//���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob;		//�s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob;		//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/AfterBloomVS.hlsl",	//�V�F�[�_�t�@�C����
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
		assert(0);
	}

	//�s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/AfterBloomPS.hlsl",	//�V�F�[�_�t�@�C����
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
		assert(0);
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
	D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//���ݒ�

	//���ʐݒ�
	blenddesc.BlendEnable = true;	//�u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//�f�X�g�̒l��0%�g��
	//�F����

	//���Z����
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

	//���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//�f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//�\�[�X�̒l��100%�g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//�f�X�g�̒l��100%�g��

	//�F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;	//�g��Ȃ�

	////����������
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	//�\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-�\

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
	assert(SUCCEEDED(result));

	//���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//�O���t�B�b�N�X�p�C�v���C���X�e�[�g�̐���
	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();

	//�p�C�v���C���X�e�[�g�̐���
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));
}