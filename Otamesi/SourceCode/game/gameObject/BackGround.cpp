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
		block[i].reset(InstanceObject::Create(model.get()));
	}

	JsonLoader::LoadNlohmannJson("back", &info);

}

void BackGround::Update()
{
	const DirectX::XMFLOAT4 color = { 1.0f,1.0f ,1.0f ,1.0f };
	int index{ 0 };
	for (auto& itr : info) {
		if (index < 512)
		{
			block[0]->DrawInstance(itr[int(transform::translation)], itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}
		else if (index < 512*2)
		{
			block[1]->DrawInstance(itr[int(transform::translation)], itr[int(transform::scaling)], itr[int(transform::rotation)], color);
		}

		else if (index < 512 * 3)
		{
			block[2]->DrawInstance(itr[int(transform::translation)], itr[int(transform::scaling)], itr[int(transform::rotation)], color);
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
