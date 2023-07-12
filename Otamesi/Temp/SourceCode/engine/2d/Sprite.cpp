#include "Sprite.h"
#include "DescHeapSRV.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <d3dx12.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;


Sprite* Sprite::Create(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	//�C���X�^���X�𐶐�
	Sprite* instance = new Sprite();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(texture, anchorpoint, isFlipX, isFlipY)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool Sprite::Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	this->texture = texture;
	this->anchorpoint = anchorpoint;
	this->isFlipX = isFlipX;
	this->isFlipY = isFlipY;

	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	HRESULT result;

	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{  0.0f, 100.0f, 0.0f}, {0.0f, 1.0f}},	//����
		{{  0.0f,   0.0f, 0.0f}, {0.0f, 0.0f}},	//����
		{{100.0f, 100.0f, 0.0f}, {1.0f, 1.0f}},	//�E��
		{{100.0f,   0.0f, 0.0f}, {1.0f, 0.0f}},	//�E��
	};

	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (texture.texBuff.Get()) {
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = texture.texBuff.Get()->GetDesc();
		//�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		size = { (float)resDesc.Width, (float)resDesc.Height };
		texSize = { (float)resDesc.Width, (float)resDesc.Height };
	}

	//���_�o�b�t�@����
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertexBuffer();

	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐���
	result = spriteCommon->GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuff));

	//�萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1);	//�F�w��(RGBA)
	//���s���e�s��
	constMap->mat = spriteCommon->GetMatProjection();
	constBuff->Unmap(0, nullptr);


	return true;
}

void Sprite::TransferVertexBuffer()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	HRESULT result;

	//���_�f�[�^
	VertexPosUv vertices[] = {
		//
		{{}, {0.0f, 1.0f}},	//����
		{{}, {0.0f, 0.0f}},	//����
		{{}, {1.0f, 1.0f}},	//�E��
		{{}, {1.0f, 0.0f}},	//�E��
	};

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;

	//���E����ւ�
	if (isFlipX) {
		left = -left;
		right = -right;
	}
	//�㉺���]
	if (isFlipY) {
		top = -top;
		bottom = -bottom;
	}

	//�����@����@�E���@�E��
	enum { LB, LT, RB, RT };

	vertices[LB].pos = { left, bottom,  0.0f };	//����
	vertices[LT].pos = { left, top,     0.0f };	//����
	vertices[RB].pos = { right, bottom, 0.0f };	//�E��
	vertices[RT].pos = { right, top,    0.0f };	//�E��

	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (texture.texBuff.Get()) {
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = texture.texBuff.Get()->GetDesc();

		float tex_left = texLeftTop.x / resDesc.Width;
		float tex_right = (texLeftTop.x + texSize.x) / resDesc.Width;
		float tex_top = texLeftTop.y / resDesc.Height;
		float tex_bottom = (texLeftTop.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left, tex_bottom };	//����
		vertices[LT].uv = { tex_left, tex_top };	//����
		vertices[RB].uv = { tex_right, tex_bottom };//�E��
		vertices[RT].uv = { tex_right, tex_top };	//�E��
	}

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	vertBuff->Unmap(0, nullptr);
}

void Sprite::Update()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//���[���h�s��̍X�V
	matWorld = XMMatrixIdentity();
	//Z����]
	matWorld *= XMMatrixRotationZ(XMConvertToRadians(rotation));
	//���s�ړ�
	matWorld *= XMMatrixTranslation(position.x, position.y, 0);

	//���_�o�b�t�@�ɔ��f
	TransferVertexBuffer();

	//�萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = matWorld * spriteCommon->GetMatProjection();
	constMap->color = color;
	constBuff->Unmap(0, nullptr);
}

void Sprite::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	ID3D12GraphicsCommandList* cmdList = spriteCommon->GetCmdList();

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	DescHeapSRV::SetGraphicsRootDescriptorTable(1, texture.texNumber);

	//�|���S���̕`��(4���_�Ŏl�p�`)
	cmdList->DrawInstanced(4, 1, 0, 0);
}
