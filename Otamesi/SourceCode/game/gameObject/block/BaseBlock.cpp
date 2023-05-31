#include "BaseBlock.h"

void BaseBlock::StaticInitialize()
{
	model[int(BROCK_TYPE::block)].reset(ObjModel::LoadFromOBJ("block"));
	model[int(BROCK_TYPE::plane)].reset(ObjModel::LoadFromOBJ("plane"));
	model[int(BROCK_TYPE::goal)].reset(ObjModel::LoadFromOBJ("goal"));
	model[int(BROCK_TYPE::player)].reset(ObjModel::LoadFromOBJ("player"));

	block[int(BROCK_TYPE::block)].reset(InstanceObject::Create(model[int(BROCK_TYPE::block)].get()));
	block[int(BROCK_TYPE::plane)].reset(InstanceObject::Create(model[int(BROCK_TYPE::plane)].get()));
	block[int(BROCK_TYPE::goal)].reset(InstanceObject::Create(model[int(BROCK_TYPE::goal)].get()));
	block[int(BROCK_TYPE::player)].reset(InstanceObject::Create(model[int(BROCK_TYPE::player)].get()));
}

void BaseBlock::Update(const int _blockType, const XMFLOAT3& _pos, const XMFLOAT3& _scale,
const XMFLOAT3& _rotation, const XMFLOAT4& _color, const XMMATRIX* _parentWorld)
{
	block[_blockType]->DrawInstance(_pos, _scale, _rotation, _color, _parentWorld);
}

void BaseBlock::Draw()
{
	//ƒuƒƒbƒN•`‰æ
	for (auto& i : block) {
		i->Draw();
	}
}
