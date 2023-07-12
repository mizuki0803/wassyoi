#include "OutLine.h"

#include "WindowApp.h"
#include "DescHeapSRV.h"
#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

ID3D12Device *OutLine::dev = nullptr;
ID3D12GraphicsCommandList *OutLine::cmdList = nullptr;
PipelineSet OutLine::pipelineSet;
Camera *OutLine::camera_ = nullptr;

OutLine *OutLine::Create()
{
	//�C���X�^���X�𐶐�
	OutLine *instance = new OutLine();
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

bool OutLine::Initialize()
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

	for (int i{ 0 }; i < kTexMax; ++i)
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
			IID_PPV_ARGS(&texture[i].texBuff)
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
			result = texture[i].texBuff->WriteToSubresource(0, nullptr,
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
		DescHeapSRV::CreateShaderResourceView(srvDesc, texture[i]);

		//RTV�p�f�X�N���v�^�q�[�v�ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescHeapDesc.NumDescriptors = 2;
		//RTV�p�f�X�N���v�^�q�[�v�𐶐�
		result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
		assert(SUCCEEDED(result));

	}
	

	// RTV
	for (int i{ 0 }; i < kTexMax; ++i)
	{
		//�f�X�N���v�^�q�[�v��RTV����
		dev->CreateRenderTargetView(
			texture[i].texBuff.Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))

		);
	}
	texture[kColor].texBuff->SetName(L"OutLineBase");
	texture[kDepth].texBuff->SetName(L"DepthTex");


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

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(CameraBuffer) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&camera_buffer_));


	return true;
}

void OutLine::Draw()
{

	//�萔�o�b�t�@�ւ̃f�[�^�]��(�����J�������_)
	CameraBuffer *camera_map = nullptr;
	if (SUCCEEDED(camera_buffer_->Map(0, nullptr, (void **)&camera_map))) {
		camera_map->view_projection = camera_->GetMatView() * camera_->GetMatProjection();
		camera_map->inv_view_projection = XMMatrixInverse(nullptr, camera_map->view_projection);
		camera_buffer_->Unmap(0, nullptr);
	}
	cmdList->SetGraphicsRootConstantBufferView(0, camera_buffer_->GetGPUVirtualAddress());

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	DescHeapSRV::SetGraphicsRootDescriptorTable(1, texture[kColor].texNumber);
	DescHeapSRV::SetGraphicsRootDescriptorTable(2, texture[kDepth].texNumber);



	//�|���S���̕`��(4���_�Ŏl�p�`)
	cmdList->DrawInstanced(4, 1, 0, 0);


}

void OutLine::DrawScenePrev(ID3D12DescriptorHeap *descHeapDSV)
{
	for (int i{ 0 }; i < kTexMax; ++i)
	{
		//���\�[�X�o���A��ύX(�V�F�[�_���\�[�X���`��\)
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texture[i].texBuff.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}
	//�����_�[�^�[�Q�b�g�r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[kTexMax];
	for (int i{ 0 }; i < kTexMax; ++i)
	{
		rtvH[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();



	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(kTexMax, rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewports[kTexMax];
	CD3DX12_RECT scissor_rects[kTexMax];
	for (int i = 0; i < kTexMax; ++i)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f,
			WindowApp::window_width, WindowApp::window_height); 
		scissor_rects[i] = CD3DX12_RECT(0, 0, 
			WindowApp::window_width, WindowApp::window_height);
	}


	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(2, viewports);
	//�V�U�����O��`�̐ݒ�
	cmdList->RSSetScissorRects(2, scissor_rects);

	//�S��ʃN���A
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < kTexMax; ++i)
	{
		cmdList->ClearRenderTargetView(rtvH[i], clearColor, 0, nullptr);
	}

}

void OutLine::DrawSceneRear()
{
	//���\�[�X�o���A��ύX(�`��\���V�F�[�_���\�[�X)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[kColor].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[kDepth].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}
