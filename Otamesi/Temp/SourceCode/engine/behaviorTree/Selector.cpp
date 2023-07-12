#include "Selector.h"
#include <assert.h>

Selector* Selector::Create()
{
	//�Z���N�^�[�̃C���X�^���X�𐶐�
	Selector* selector = new Selector();
	if (selector == nullptr) {
		return nullptr;
	}

	return selector;
}

bool Selector::Select()
{
	//true���A���Ă����瑦�I��
	for (int i = 0; i < (signed)nodes.size(); i++) {
		if ((nodes[i])()) {
			return true;
		}
	}

	return false;
}

void Selector::AddNode(std::function<bool()> func)
{
	//�q�m�[�h�z��ɓo�^
	nodes.push_back(func);
}
