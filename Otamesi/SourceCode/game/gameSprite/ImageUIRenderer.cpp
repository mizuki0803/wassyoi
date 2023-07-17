#include "ImageUIRenderer.h"
#include "DescHeapSRV.h"


ID3D12Device *ImageUIRenderer::dev = nullptr;
ID3D12GraphicsCommandList *ImageUIRenderer::cmdList = nullptr;

ImageUIRenderer *ImageUIRenderer::Create()
{
	//�C���X�^���X�𐶐�
	ImageUIRenderer *instance = new ImageUIRenderer();
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

bool ImageUIRenderer::Initialize()
{
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

	for (int i{ 0 }; i < static_cast<int>(TexName::Max); ++i)
	{
		//�e�N�X�`���o�b�t�@�̐���	
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
			IID_PPV_ARGS(&texture_[i].texBuff)
		);
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
			result = texture_[i].texBuff->WriteToSubresource(0, nullptr,
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
		DescHeapSRV::CreateShaderResourceView(srvDesc, texture_[i]);

		//RTV�p�f�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescHeapDesc.NumDescriptors = 2;
		//RTV�p�f�X�N���v�^�q�[�v�𐶐�
		result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
		assert(SUCCEEDED(result));

	}


	// RTV
	for (int i{ 0 }; i < static_cast<int>(TexName::Max); ++i)
	{
		//�f�X�N���v�^�q�[�v��RTV����
		dev->CreateRenderTargetView(
			texture_[i].texBuff.Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))

		);
	}
	texture_[static_cast<int>(TexName::Camera)].texBuff->SetName(L"Camera");
	texture_[static_cast<int>(TexName::Move)].texBuff->SetName(L"Move");


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
	//DSV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart());

	

	// ���f���̓ǂݍ���
	key_[static_cast<int>(KeyObjectName::W)]->Create(
		ObjModel::LoadFromOBJ("KeyBoradW", true)
	);
	key_[static_cast<int>(KeyObjectName::A)]->Create(
		ObjModel::LoadFromOBJ("KeyBoradA", true)
	);
	key_[static_cast<int>(KeyObjectName::S)]->Create(
		ObjModel::LoadFromOBJ("KeyBoradS", true)
	);
	key_[static_cast<int>(KeyObjectName::D)]->Create(
		ObjModel::LoadFromOBJ("KeyBoradD", true)
	);

	return true;
}

void ImageUIRenderer::Update(bool isMoveMenu, bool isCameraMenu)
{
	// �ړ��p�摜�X�V
	if (isMoveMenu)
	{

	}

	// �J�����p�摜�X�V
	if (isCameraMenu)
	{

	}
}

void ImageUIRenderer::DrawCameraDescription()
{
	//���\�[�X�o���A��ύX(�V�F�[�_���\�[�X���`��\)
	cmdList->ResourceBarrier(1,	&CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Camera)].texBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));


	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), static_cast<int>(TexName::Camera),
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
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
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);

	// ��������`��
	ObjObject3d::DrawPrev();



	//���\�[�X�o���A��ύX(�`��\���V�F�[�_���\�[�X)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Camera)].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void ImageUIRenderer::DrawMoveDescription()
{
	//���\�[�X�o���A��ύX(�V�F�[�_���\�[�X���`��\)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Move)].texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET));


	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), static_cast<int>(TexName::Move),
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
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
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);

	// ��������`��
	ObjObject3d::DrawPrev();



	//���\�[�X�o���A��ύX(�`��\���V�F�[�_���\�[�X)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Move)].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}
