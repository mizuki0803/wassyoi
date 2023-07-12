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
	model.reset(ObjModel::LoadFromOBJ("backBlock"));
	for (int i = 0; i < 3; ++i)
	{
		block[i].reset(InstanceObject::Create(model.get()));
		block[i]->SetIsShadowMap(true);
	}

	JsonLoader::LoadNlohmannJson("back", &info);

}

void BackGround::Update()
{
	const DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
	int index{ 0 };
	for (auto& itr : info) {
		XMFLOAT3 pos = itr[int(transform::translation)];
		XMFLOAT3 invpos = itr[int(transform::translation)];
		invpos.y *= -1;
		if (index < 512)
		{
			block[0]->DrawInstance(pos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}
		else if (index < 512 * 2)
		{
			block[1]->DrawInstance(pos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}

		else if (index < 512 * 3)
		{
			block[2]->DrawInstance(pos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}

		++index;
	}
}

void BackGround::Draw()
{
	for (int i = 0; i < 3; ++i)
	{
		block[i]->Draw();
	}

}

void BackGround::DrawLightCameraView()
{
	for (int i = 0; i < 3; ++i)
	{
		block[i]->DrawLightCameraView();

	}
}

void BackGround::FrameReset()
{
	for (int i = 0; i < 3; ++i)
	{
		block[i]->FrameReset();
	}
}
