#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

/// <summary>
/// �u���[���̃Z�b�g
/// </summary>
float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color);

/// <summary>
/// �g�D�[���̃Z�b�g
/// </summary>
float4 SetToon(float4 shadecolor);

PSOutput main(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);

	float4 color = baseColor;

	// ����x
	const float shininess = 4.0f;

	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	if (isLight)
	{
		//���s����
		for (int i = 0; i < DIRLIGHT_NUM; i++)
		{
			if (dirLights[i].active)
			{
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
		for (i = 0; i < POINTLIGHT_NUM; i++)
		{
			if (pointLights[i].active)
			{
				//���C�g�ւ̃x�N�g��
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
				// �g�U���ˌ�
				float3 diffuse = dotlightnormal * m_diffuse;
				// ���ʔ��ˌ�
				float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

				// �S�ĉ��Z����
				shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
			}
		}

		// �X�|�b�g���C�g
		for (i = 0; i < SPOTLIGHT_NUM; i++)
		{
			if (spotLights[i].active)
			{
				// ���C�g�ւ̕����x�N�g��
				float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
				//�x�N�g���̒���
				float d = length(lightv);
				//���K�����A�P��
				lightv = normalize(lightv);
				// ���������W��
				float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
				// �p�x����
				float cos = dot(lightv, spotLights[i].lightv);
				// �����J�n�p�x����A�����I���p�x�ɂ����Č���
				// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
				float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
				// �p�x��������Z
				atten *= angleatten;
				// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
				float3 dotlightnormal = dot(lightv, input.normal);
				// ���ˌ��x�N�g��
				float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
				// �g�U���ˌ�
				float3 diffuse = dotlightnormal * m_diffuse;
				// ���ʔ��ˌ�
				float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

				// �S�ĉ��Z����
				shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
			}
		}

		// �ۉe
		for (i = 0; i < CIRCLESHADOW_NUM; i++)
		{
			if (circleShadows[i].active)
			{
				// �I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
				float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
				// ���������ł̋���
				float d = dot(casterv, circleShadows[i].dir);
				// ���������W��
				float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
				// �������}�C�i�X�Ȃ�0�ɂ���
				atten *= step(0, d);
				// ���C�g�̍��W
				float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
				//  �I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g���i�P�ʃx�N�g���j
				float3 lightv = normalize(lightpos - input.worldpos.xyz);
				// �p�x����
				float cos = dot(lightv, circleShadows[i].dir);
				// �����J�n�p�x����A�����I���p�x�ɂ����Č���
				// �����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
				float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
				// �p�x��������Z
				atten *= angleatten;

				// �S�Č��Z����
				shadecolor.rgb -= atten;
			}
		}
	}
	//�|�X�g�G�t�F�N�g����

	//�u���[������
	float4 bloom = float4(0, 0, 0, 0);
	if (isBloom)
	{
		bloom = SetBloom(shadecolor, texcolor, color);
		bloom = float4(bloom.rgb, 0.1);
	}

	//�g�D�[���V�F�[�h
	if (isToon)
	{
		shadecolor = SetToon(shadecolor);
	}


	// �V�F�[�f�B���O�ɂ��F�ŕ`��
	float4 mainColor = shadecolor * texcolor * color;
	PSOutput output;
	output.target0 = float4(mainColor.rgb, color.w);
	output.target1 = bloom;
	output.target2 = float4(0.5, 0.5, 0.5, 1.0) * isOutline;
	return output;
}

float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color)
{
	//���x�l�̒��o
	float LuminousIntensity = dot(shadecolor.rgb * texcolor.rgb, float3(0.2125, 0.7154, 0.0712));

	//�u���[����������ꏊ
	float4 bloomColor = step(1.0, LuminousIntensity) * texcolor * color;

	return bloomColor;
}

float4 SetToon(float4 shadecolor)
{
	//�g�D�[���̐F�͈�
	float toonLighrRange = 0.525;
	//���Â̒��ԕ�
	float contourWidth = 0.1;

	//���邢
	float3 bright;
	bright.r = step(toonLighrRange, shadecolor.r);
	bright.g = step(toonLighrRange, shadecolor.g);
	bright.b = step(toonLighrRange, shadecolor.b);
	//�Â�
	float3 dark;
	dark.r = (1 - step(toonLighrRange, shadecolor.r));
	dark.g = (1 - step(toonLighrRange, shadecolor.g));
	dark.b = (1 - step(toonLighrRange, shadecolor.b));

	//����
	float3 intermediate;
	intermediate.r = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.r);
	intermediate.g = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.g);
	intermediate.b = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.b);

	//���݂̐F
	return float4(bright + dark + intermediate, 1);
}