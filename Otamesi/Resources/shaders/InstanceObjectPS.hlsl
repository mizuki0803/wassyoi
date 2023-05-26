#include "InstanceObject.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> shadowMap : register(t1);	//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
	//�e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv) * color[input.instNo];

	// ����x
	const float shininess = 4.0f;
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	//���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
			// ���ˌ��x�N�g��
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			// �g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			// ���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			// �S�ĉ��Z����
			shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
		}
	}

	//�_����
	for (i = 0; i < POINTLIGHT_NUM; i++) {
		if (pointLights[i].active) {
			// ���C�g�ւ̃x�N�g��
			float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
			//�x�N�g���̒���
			float d = length(lightv);
			//���K�����A�P�ʃx�N�g���ɂ���
			lightv = normalize(lightv);
			//���������W��
			float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//�S�ĉ��Z����
			shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
		}
	}

	//�X�|�b�g���C�g
	for (i = 0; i < SPOTLIGHT_NUM; i++) {
		if (spotLights[i].active) {
			//���C�g�ւ̕����x�N�g��
			float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			//���������W��
			float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
			//�p�x����
			float cos = dot(lightv, spotLights[i].lightv);
			//�����J�n�p�x����A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
			//�p�x��������Z
			atten *= angleatten;
			//���C�g�Ɍ������x�N�g���Ɩ@���̓���
			float3 dotlightnormal = dot(lightv, input.normal);
			//���ˌ��x�N�g��
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			//�g�U���ˌ�
			float3 diffuse = dotlightnormal * m_diffuse;
			//���ʔ��ˌ�
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
			//�S�ĉ��Z����
			shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
		}
	}

	//�ۉe
	for (i = 0; i < CIRCLESHADOW_NUM; i++) {
		if (circleShadows[i].active) {
			//�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
			float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
			//���e�����ł̋���
			float d = dot(casterv, circleShadows[i].dir);
			//���������W��
			float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
			//�������}�C�i�X�Ȃ�0�ɂ���
			atten *= step(0, d);
			//���z���C�g�̍��W
			float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
			//�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g��(�P�ʃx�N�g��)
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			//�p�x����
			float cos = dot(lightv, circleShadows[i].dir);
			//�����J�n�p�x����A�����I���p�x�ɂ����Č���
			//�����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
			float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
			//�p�x��������Z
			atten *= angleatten;

			//�S�ĉ��Z����
			shadecolor.rgb -= atten;
		}
	}

	//�e(�V���h�E�}�b�v)
	float shadow = 1.0f;
	if (isShadowMap) {
		//�V���h�E�}�b�v��Z�l���Q��
		float w = 1.0f / input.shadowpos.w;
		float2 shadowTexUV;
		shadowTexUV.x = (1.0f + input.shadowpos.x * w) * 0.5f;
		shadowTexUV.y = (1.0f - input.shadowpos.y * w) * 0.5f;
		//uv���W��0�`1�Ȃ�e���������
		if (shadowTexUV.x >= 0 && shadowTexUV.x <= 1.0f && shadowTexUV.y >= 0 && shadowTexUV.y <= 1.0f) {
			if (shadowMap.Sample(smp, shadowTexUV).x + 0.0005f < input.shadowpos.z * w) {
				shadow *= 0.5f;
			}
		}
	}

	// �V�F�[�f�B���O�ɂ��F�ŕ`��;
	return shadecolor * texcolor * float4(shadow, shadow, shadow, 1);
}