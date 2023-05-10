#include "Block.h"

const float Block::blockSize = 5.0f;

Block* Block::Create(ObjModel* model, const Vector3& position)
{
    //インスタンス生成
    Block* instance = new Block();

	//モデルをセット
	assert(model);
	instance->model = model;

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	//座標をセット
	instance->position = position;
	//大きさをセット
	instance->scale = { blockSize, blockSize, blockSize };

    return instance;
}
