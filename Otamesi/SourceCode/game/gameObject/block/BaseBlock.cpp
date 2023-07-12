#include "BaseBlock.h"
#include "GamePostEffect.h"
void BaseBlock::StaticInitialize()
{
	model[int(BROCK_MODEL_TYPE::block)].reset(ObjModel::LoadFromOBJ("block"));
	model[int(BROCK_MODEL_TYPE::plane)].reset(ObjModel::LoadFromOBJ("plane"));
	model[int(BROCK_MODEL_TYPE::goal)].reset(ObjModel::LoadFromOBJ("goal"));
	model[int(BROCK_MODEL_TYPE::player)].reset(ObjModel::LoadFromOBJ("player"));

	//ステージブロック
	for (int i = 0; i<int(BROCK_TYPE::plane); i++) {
		block[i].reset(InstanceObject::Create(model[int(BROCK_MODEL_TYPE::block)].get()));
	}
	//ハリボテ
	block[int(BROCK_TYPE::plane)].reset(InstanceObject::Create(model[int(BROCK_MODEL_TYPE::plane)].get()));
	//ゴール
	block[int(BROCK_TYPE::goal)].reset(InstanceObject::Create(model[int(BROCK_MODEL_TYPE::goal)].get()));
	//プレイヤー
	block[int(BROCK_TYPE::player)].reset(InstanceObject::Create(model[int(BROCK_MODEL_TYPE::player)].get()));
}

void BaseBlock::Update(const int _blockType, const XMFLOAT3& _pos, const XMFLOAT3& _scale,
const XMFLOAT3& _rotation, const XMFLOAT4& _color, const XMMATRIX* _parentWorld)
{
	//ブロックだった場合描画数によって仕様配列を変更
	if (_blockType == int(BROCK_TYPE::block)) {
		for (int i = 0; i < int(BROCK_TYPE::plane); i++) {
			if (block[i]->GetInstanceDrawCheck()) {
				block[i]->DrawInstance(_pos, _scale, _rotation, _color, _parentWorld);
				return;
			}
		}
	}
	block[_blockType]->DrawInstance(_pos, _scale, _rotation, _color, _parentWorld);
}

void BaseBlock::Draw()
{
	//ブロック描画
	int type = static_cast<int>(BROCK_TYPE::block);

	GamePostEffect::SetIdColorBuffer(6, PostEffect::kGoal);
	block[static_cast<int>(BROCK_TYPE::goal)]->Draw();
	for (auto& i : block) {
		switch (static_cast<BROCK_TYPE>(type))
		{
		case BaseBlock::BROCK_TYPE::block:
			GamePostEffect::SetIdColorBuffer(6, PostEffect::kStage);
			i->Draw();
			break;
		case BaseBlock::BROCK_TYPE::plane:
			GamePostEffect::SetIdColorBuffer(6, PostEffect::kStage);
			i->Draw();
			break;
		case BaseBlock::BROCK_TYPE::goal:
			break;
		case BaseBlock::BROCK_TYPE::player:
			GamePostEffect::SetIdColorBuffer(6, PostEffect::kPlayer);
			i->Draw();
			break;
		default:
			break;
		}

		++type;
	}
}
