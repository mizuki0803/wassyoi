#include "ParticleEmitter.h"
#include "Easing.h"
#include "TextureManager.h"
#include <functional>
#include <d3dx12.h>

ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//�e�N�X�`���S�ǂݍ���
	LoadTexture();

	//�p�[�e�B�N���}�l�[�W���[����
	circleParticle.reset(ParticleManager::Create(textures[Circle]));
	explosionParticle.reset(ParticleManager::Create(textures[Fluffy]));
	blackSmokeParticle.reset(ParticleManager::Create(textures[Fluffy]));
	shineParticle.reset(ParticleManager::Create(textures[Shine]));
	curveParticle.reset(ParticleManager::Create(textures[Curve]));
}

void ParticleEmitter::Update()
{
	//�p�[�e�B�N���}�l�[�W���[�X�V
	circleParticle->Update();
	explosionParticle->Update();
	blackSmokeParticle->Update();
	shineParticle->Update();
	curveParticle->Update();
}

void ParticleEmitter::DrawAll()
{
	//���Z�����`��O����
	ParticleManager::DrawPrevAddBlend();

	//�p�[�e�B�N���}�l�[�W���[�`��
	circleParticle->Draw();
	explosionParticle->Draw();
	shineParticle->Draw();
	curveParticle->Draw();

	//���Z�����`��O����
	ParticleManager::DrawPrevSubBlend();

	//�p�[�e�B�N���}�l�[�W���[�`��
	blackSmokeParticle->Draw();
}

void ParticleEmitter::DemoEffect()
{
	//�F
	const XMFLOAT4 purple = { 1.0f, 0.1f, 1.0f, 1.0f }; //��
	const XMFLOAT4 lightBlue = { 0.1f, 1.0f, 1.0f, 1.0f }; //���F

	for (int i = 0; i < 100; i++) {
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float mdVel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * mdAcc;

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, purple, lightBlue);
	}
}

void ParticleEmitter::DemoEffect2()
{
	//�F
	const XMFLOAT4 red = { 1.0f, 0.1f, 0.1f, 1.0f }; //��
	const XMFLOAT4 green = { 0.1f, 1.0f, 0.1f, 1.0f }; //���F

	for (int i = 0; i < 100; i++) {
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float mdVel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * mdAcc;

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		explosionParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, red, green);
	}
}

void ParticleEmitter::PlayerJet(std::function<Vector3()> getTargetPos, const XMMATRIX& playerMatWorld, const int playerSpeedPhase)
{
	//�p�[�e�B�N��(��)����
	{
		//�F
		const XMFLOAT4 startColor = { 0.6f, 0.6f, 0.6f, 1.0f }; //�Z����
		const XMFLOAT4 endColor = { 0.4f, 0.4f, 0.4f, 1.0f }; //������
		Vector3 pos{};
		float startScale = 0;
		float endScale = 0;
		float randVel = 0;
		float randAcc = 0;

		//�ړ����x�t�F�[�Y���Ƃɒ���
		//�ʏ�ړ����
		if (playerSpeedPhase == 0) {
			randVel = (float)rand() / RAND_MAX * -0.1f - 0.1f;
			const float mdAcc = 0.005f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 0.5f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 1) {
			randVel = (float)rand() / RAND_MAX * -0.2f - 0.2f;
			const float mdAcc = 0.01f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 1.0f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 2) {
			randVel = (float)rand() / RAND_MAX * -0.001f - 0.001f;
			const float mdAcc = 0.001f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
		}
		//�ʏ�ړ��ɖ߂����
		else if (playerSpeedPhase == 3) {
			randVel = (float)rand() / RAND_MAX * -0.002f - 0.002f;
			const float mdAcc = 0.0025f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.05f + 0.3f;
		}
		//���x���Z�b�g
		Vector3 vel = { 0, 0, randVel };
		vel = MatrixTransformDirection(vel, playerMatWorld);
		//�����x���Z�b�g
		Vector3 acc = { 0, 0, randAcc };
		acc = MatrixTransformDirection(acc, playerMatWorld);

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//�ǉ�
		circleParticle->AddTargetFollow(5, getTargetPos, pos, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}

	//�p�[�e�B�N��(��)����
	{
		//�F
		const XMFLOAT4 startColor = { 1.0f, 0.21f, 0.21f, 1.0f }; //������
		const XMFLOAT4 endColor = { 0.3f, 0.01f, 0.01f, 1.0f }; //�Z����
		Vector3 pos{};
		float startScale = 0;
		float endScale = 0;
		float randVel = 0;
		float randAcc = 0;

		//�ړ����x�t�F�[�Y���Ƃɒ���
		//�ʏ�ړ����
		if (playerSpeedPhase == 0) {
			randVel = (float)rand() / RAND_MAX * -0.1f - 0.1f;
			const float mdAcc = 0.008f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.2f + 2.0f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 1) {
			randVel = (float)rand() / RAND_MAX * -0.2f - 0.2f;
			const float mdAcc = 0.0016f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.2f + 3.4f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 2) {
			randVel = (float)rand() / RAND_MAX * -0.04f - 0.04f;
			const float mdAcc = 0.004f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 1.0f;
		}
		//�ʏ�ړ��ɖ߂����
		else if (playerSpeedPhase == 3) {
			randVel = (float)rand() / RAND_MAX * -0.06f - 0.06f;
			const float mdAcc = 0.005f;
			randAcc = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.15f + 1.2f;
		}
		//���x���Z�b�g
		Vector3 vel = { 0, 0, randVel };
		vel = MatrixTransformDirection(vel, playerMatWorld);
		//�����x���Z�b�g
		Vector3 acc = { 0, 0, randAcc };
		acc = MatrixTransformDirection(acc, playerMatWorld);

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//�ǉ�
		circleParticle->AddTargetFollow(5, getTargetPos, pos, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::PlayerBlackSmokeJet(const Vector3& position)
{
	//���W�����ɍ����G�t�F�N�g���쐬
	for (int i = 0; i < 2; i++) {
		//��������
		int life = (rand() % 30) + 180;

		//X,Y,Z�����_���ɕ��z
		const Vector3 mdVel = { 0.01f, 0.05f, 0.01f };
		Vector3 vel{};
		vel.x = ((float)rand() / RAND_MAX * mdVel.x - mdVel.x / 2.0f);
		vel.y = ((float)rand() / RAND_MAX * mdVel.y);
		vel.z = ((float)rand() / RAND_MAX * mdVel.z - mdVel.z / 2.0f);
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = (float)rand() / RAND_MAX * mdAcc;

		//�傫�����Z�b�g
		const float mdScale = 2.5f;
		const float randScale = ((float)rand() / RAND_MAX * mdScale);
		const float startScale = randScale;
		const float endScale = randScale * 5.0f;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�F
		const XMFLOAT4 startColor = { 0.15f, 0.15f, 0.15f, 1 }; //������
		const XMFLOAT4 endColor = { 0.01f, 0.01f, 0.01f, 1 }; //����������

		//�ǉ�
		blackSmokeParticle->Add(life, position, vel, acc, startScale, endScale, lerpFloat, startColor, endColor);
	}
}

void ParticleEmitter::PlayerVapor(const Vector3& position, const float size, const Vector3& velocity)
{
	//��������
	const int life = (rand() % 10) + 30;
	//�����x��0
	const Vector3 acc{};
	//�傫���ύX�̃C�[�W���O
	std::function<float(const float, const float, const float) > lerp =
		std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//�F
	const XMFLOAT4 startColor = { 0.05f, 0.05f, 0.05f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0.002f, 0.002f, 0.002f, 1.0f }; //������

	//�ǉ�
	circleParticle->Add(life, position, velocity, acc, size, size, lerp, startColor, endColor);
}

void ParticleEmitter::PlayerRolling(const int lifeTime, std::function<Vector3()> getTargetPos, const Vector3& localPos, const float rotation, const float size, const XMFLOAT4& color)
{
	//�����x��0
	const Vector3 vel{};
	const Vector3 acc{};
	//�傫���ύX�̃C�[�W���O
	std::function<float(const float, const float, const float) > lerp =
		std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//�F
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

	//�ǉ�
	curveParticle->AddTargetFollow(lifeTime, getTargetPos, localPos, vel, acc, size, size, lerp, color, endColor, rotation);
}

void ParticleEmitter::Shot(const Vector3& position)
{
	//��������
	const int life = 3;
	//���x�A�����x��0
	const Vector3 vel{};
	const Vector3 acc{};
	//�傫���ύX�̃C�[�W���O
	std::function<float(const float, const float, const float) > lerp =
		std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//�F
	const XMFLOAT4 green = { 0.1f, 0.3f, 0.1f, 1.0f }; //������
	for (int i = 0; i < 3; i++) {
		//�傫��
		const float scale = 2.5f;

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, scale, scale, lerp, green, green);
	}

	//�F
	const XMFLOAT4 white = { 0.1f, 0.1f, 0.1f, 1.0f }; //������
	for (int i = 0; i < 3; i++) {
		//�傫��
		const float scale = 1.25f;

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, scale, scale, lerp, white, white);
	}
}

void ParticleEmitter::ShotDead(const Vector3& position, const float size)
{
	//�F
	const XMFLOAT4 startColor = { 0.2f, 0.3f, 0.4f, 1.0f }; //������
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 3; i++) {
		//��������
		const int life = (rand() % 10) + 15;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 0.5f;
		Vector3 pos = position;
		pos.x += (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//�傫���������_���ɕ��z
		const float mdScale = 2.0f;
		const float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		const float endScale = startScale;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, pos, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShot(const Vector3& position, const float size)
{
	//��������
	const int life = 2;
	//�F
	const XMFLOAT4 startColor = { 0.1f, 1.0f, 0.05f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0.1f, 0.4f, 0.05f, 1.0f }; //������
	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 10; i++) {
		//�傫���������_���ɕ��z
		const float mdScale = 2.8f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 7) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShotDead(const Vector3& position, const float size)
{
	//�F
	const XMFLOAT4 startColor = { 0.04f, 0.1f, 0.04f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//��������
		int life = (rand() % 20) + 30;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}

	for (int i = 0; i < 10; i++) {
		//��������
		int life = (rand() % 20) + 60;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::FireBall(const Vector3& position, const float size, const int time)
{
	for (int i = 0; i < 5; i++) {
		//��������
		int life = (rand() % 30) + time;

		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		Vector3 pos = position;
		const float mdPos2 = 0.8f;
		pos.x += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
		pos.y += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
		pos.z += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;

		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float mdVel = 0.05f;
		Vector3 vel{};
		vel.x = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
		vel.y = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
		vel.z = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
		Vector3 acc{};

		//�傫��
		float startScale = size;
		float endScale = size * 2;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//�F
		const float mdGreen = 0.2f;
		const float green = 0.2f + (float)rand() / RAND_MAX * mdGreen;
		const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //�Z����
		const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

		//�ǉ�
		explosionParticle->Add(life, pos, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::BossCharge(const Vector3& position, const float size)
{
	//��������
	const int life = 2;
	//�F
	const XMFLOAT4 startColor = { 1.0f, 0.1f, 0.1f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0.4f, 0.01f, 0.01f, 1.0f }; //������
	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//�傫���������_���ɕ��z
		const float mdScale = 1.5f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 7) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::BossChargeShot(const Vector3& position, const float size)
{
	//��������
	const int life = 2;
	//�F
	const XMFLOAT4 startColor = { 1.0f, 0.1f, 0.1f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0.4f, 0.01f, 0.01f, 1.0f }; //������
	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//�傫���������_���ɕ��z
		const float mdScale = 1.0f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 4) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::BossChargeShotDead(const Vector3& position, const float size)
{
	//�F
	const XMFLOAT4 startColor = { 0.1f, 0.04f, 0.04f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//��������
		int life = (rand() % 10) + 15;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}

	for (int i = 0; i < 10; i++) {
		//��������
		int life = (rand() % 20) + 60;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::Explosion(const Vector3& position, const float size, const int time)
{
	for (int j = 0; j < 5; j++) {
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 2.0f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;

		for (int i = 0; i < 25; i++) {
			//��������
			int life = (rand() % 30) + time;

			//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
			const float mdPos2 = 0.8f;
			pos.x += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
			pos.y += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
			pos.z += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;

			//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
			const float mdVel = 0.05f;
			Vector3 vel{};
			vel.x = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			vel.y = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			vel.z = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			Vector3 acc{};

			const float mdScale = 3.0f;
			float startScale = 0.3f * size;
			float endScale = (4.0f + (float)rand() / RAND_MAX * mdScale - mdScale / 2.0f) * size;
			//�傫���ύX�̃C�[�W���O
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//�F
			const float mdGreen = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * mdGreen;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //�Z����
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

			//�ǉ�
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BossDeadExplosion(const Vector3& position, const float size, const bool isGround)
{
	for (int j = 0; j < 15; j++) {
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 3.0f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;
		if (isGround) { pos.y += ((float)rand() / RAND_MAX * mdPos) * size; }
		else { pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size; }
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;

		for (int i = 0; i < 25; i++) {
			//��������
			int life = (rand() % 30) + 75;

			//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
			const float mdPos2 = 2.5f;
			pos.x += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
			if(!isGround) { pos.y += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size; }
			pos.z += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;

			//X,Y,Z�S��[-0.25f, +0.25f]�Ń����_���ɕ��z
			const float mdVel = 0.75f;
			Vector3 vel{};
			vel.x = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			if (isGround) { vel.y = ((float)rand() / RAND_MAX * mdVel) * size; }
			else { vel.y = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size; }
			vel.z = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			Vector3 acc{};
			if (isGround) {
				const float mdAcc = 0.01f;
				acc.y = -(float)rand() / RAND_MAX * mdAcc;
			}

			const float mdScale = 5.0f;
			float startScale = 3.0f * size;
			float endScale = (20.0f + (float)rand() / RAND_MAX * mdScale - mdScale / 2.0f) * size;
			//�傫���ύX�̃C�[�W���O
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//�F
			const float mdGreen = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * mdGreen;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //�Z����
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

			//�ǉ�
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BlackSmoke(const Vector3& position, const float size, const int time)
{
	for (int i = 0; i < 2; i++) {
		//��������
		int life = (rand() % 30) + time;

		//X,Y,Z���ƂɂŃ����_���ɕ��z
		const float mdPos = 0.1f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) + 1.0f;
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		//X,Y,Z�S��[-0.25f, +0.25f]�Ń����_���ɕ��z
		const Vector3 mdVel = { 0.01f, 0.05f, 0.01f };
		Vector3 vel{};
		vel.x = ((float)rand() / RAND_MAX * mdVel.x - mdVel.x / 2.0f);
		vel.y = ((float)rand() / RAND_MAX * mdVel.y);
		vel.z = ((float)rand() / RAND_MAX * mdVel.z - mdVel.z / 2.0f);
		Vector3 acc{};
		const float mdAcc = 0.003f;
		acc.y = (float)rand() / RAND_MAX * mdAcc;

		const float mdScale = 1.3f;
		const float randScale = ((float)rand() / RAND_MAX * mdScale);
		const float startScale = randScale * size;
		const float endScale = randScale * size * 2.0f;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�F
		const XMFLOAT4 startColor = { 0.15f, 0.15f, 0.15f, 1 }; //������
		const XMFLOAT4 endColor = { 0.01f, 0.01f, 0.01f, 1 }; //����������

		//�ǉ�
		blackSmokeParticle->Add(life, pos, vel, acc, startScale, endScale, lerpFloat, startColor, endColor);
	}
}

void ParticleEmitter::ItemShine(const Vector3& position, const float size, const int num)
{
	//��������
	for (int i = 0; i < num; i++) {
		int life = (rand() % 30) + 30;

		//X,Y,Z�ɂŃ����_���ɕ��z
		const float mdPos = 4.0f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		//�ړ����Ȃ��̂ő��x�͕ύX�Ȃ�
		Vector3 vel{};
		Vector3 acc{};
		const float mdScale = 2.0f * size;
		const float randScale = ((float)rand() / RAND_MAX * mdScale) + (1.0f * size);
		const float scale = randScale;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�F
		const XMFLOAT4 startColor = { 0.8f, 0.8f, 0.6f, 1 }; //���F
		const XMFLOAT4 endColor = { 0.4f, 0.4f, 0.3f, 1 }; //�������F

		//������]�p
		const float rot = ((float)rand() / RAND_MAX * 360);

		//��]���x
		const float mdRotSpeed = 2.5f;
		const float baseSpeed = 5.0f * ((float)num / 2);
		const float rotSpeed = ((float)rand() / RAND_MAX * mdRotSpeed) + baseSpeed;

		//�ǉ�
		shineParticle->Add(life, pos, vel, acc, scale, scale, lerpFloat, startColor, endColor, rot, rotSpeed);
	}
}

void ParticleEmitter::AllDelete()
{
	//�S�p�[�e�B�N���̍폜
	circleParticle->AllDelete();
	explosionParticle->AllDelete();
	blackSmokeParticle->AllDelete();
	shineParticle->AllDelete();
}

void ParticleEmitter::LoadTexture()
{
	//�p�[�e�B�N���e�N�X�`���p�f�B���N�g���p�X
	const std::string directoryPath = "Resources/particleTexture/";

	//�p�[�e�B�N���p�e�N�X�`���ǂݍ���
	TextureManager::LoadTexture(textures[Circle], directoryPath, "effect1.png");
	TextureManager::LoadTexture(textures[Fluffy], directoryPath, "effect2.png");
	TextureManager::LoadTexture(textures[Shine], directoryPath, "effect3.png");
	TextureManager::LoadTexture(textures[Curve], directoryPath, "effect4.png");
}
