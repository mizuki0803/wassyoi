#include "BackGround.h"
#include <JsonLoader.h>

BackGround* BackGround::Create()
{
	//新たなObjModel型のインスタンスのメモリを確保
	BackGround* model = new BackGround();

	//objファイルからデータ読み込み
	model->Initialize();

	return model;
}

void BackGround::Initialize()
{
	model.reset(ObjModel::LoadFromOBJ("backBlock"));
	for (int i = 0; i < 3; ++i)
	{
		upperBlock[i].reset(InstanceObject::Create(model.get()));
		underBlock[i].reset(InstanceObject::Create(model.get()));
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
			upperBlock[0]->DrawInstance(pos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
			underBlock[0]->DrawInstance(invpos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}
		else if (index < 512*2)
		{
			upperBlock[1]->DrawInstance(pos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
			underBlock[1]->DrawInstance(invpos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}

		else if (index < 512 * 3)
		{
			upperBlock[2]->DrawInstance(pos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
			underBlock[2]->DrawInstance(invpos, itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}

		++index;
	}
}

void BackGround::Draw()
{
	for (int i = 0; i < 3; ++i)
	{
		upperBlock[i]->Draw();
		underBlock[i]->Draw();

	}

}
