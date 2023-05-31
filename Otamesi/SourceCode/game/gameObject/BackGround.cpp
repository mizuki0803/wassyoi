#include "BackGround.h"
#include <JsonLoader.h>

BackGround* BackGround::Create()
{
	//�V����ObjModel�^�̃C���X�^���X�̃��������m��
	BackGround* model = new BackGround();

	//obj�t�@�C������f�[�^�ǂݍ���
	model->Initialize();

	return model;
}

void BackGround::Initialize()
{
	model.reset(ObjModel::LoadFromOBJ("block"));
	block.reset(InstanceObject::Create(model.get()));

	JsonLoader::LoadNlohmannJson("back", &info);
}

void BackGround::Update()
{
	const DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
	for (auto& itr : info) {
		block->DrawInstance(itr[int(transform::translation)], itr[int(transform::scaling)], itr[int(transform::rotation)], color);
	}
}

void BackGround::Draw()
{
	block->Draw();
}
