#include "StageNumberUI.h"
#include "SpriteTextureLoader.h"

StageNumberUI* StageNumberUI::Create(const Vector2& position, const float size, const int stageNum)
{
	//�C���X�^���X�𐶐�
	StageNumberUI* instance = new StageNumberUI();

	// ������
	if (!instance->Initialize(position, size, stageNum)) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool StageNumberUI::Initialize(const Vector2& position, const float size, const int stageNum)
{
	//�\���p�T�C�Y���Z�b�g
	const Vector2 texSize = { 32, 48 };
	const Vector2 showsize = texSize * size;
	
	//�����̕��A�����X�v���C�g����
	for (int i = 0; i < digitNum; i++) {
		const Vector2 pos = { position.x + texSize.x + ((digitNum - i) * showsize.x), position.y };
		numbers[i].reset(NumberSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Number), pos, showsize, texSize));
	}

	//���ꂼ��̌����ɍ��������������蓖�Ă�
	ChengeStageNum(stageNum);

	return true;
}

void StageNumberUI::Update()
{
	//�X�v���C�g�X�V
	for (int i = 0; i < digitNum; i++) {
		numbers[i]->Update();
	}
}

void StageNumberUI::Draw()
{
	//�X�v���C�g�`��
	for (int i = 0; i < digitNum; i++) {
		numbers[i]->Draw();
	}
}

void StageNumberUI::ChengeStageNum(const int stageNum)
{
	//���ꂼ��̌����ɍ��������������蓖�Ă�
	for (int i = 0; i < digitNum; i++) {
		//���鐔
		int divisionNum = 1;
		//������10����Z����(1,10,100,...)
		for (int j = 0; j < i; j++) {
			divisionNum *= 10;
		}
		//�w�肵�����̐������o��
		int numberDigit = (stageNum / divisionNum) % 10;
		//�Z�o�������̐����𐔎��X�v���C�g�Ɋ��蓖�Ă�
		numbers[i]->SetNumber(numberDigit);
	}
}
