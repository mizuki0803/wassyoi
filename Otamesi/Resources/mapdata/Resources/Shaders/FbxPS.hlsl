#include "Fbx.hlsli"

Texture2D<float4> tex : register(t0);  // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
TextureCube cubeTex:register(t1);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);      // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

//�΂̒�`
static const float PI = 3.141592654f;
//���˓_�̖@���x�N�g��
static float3 N;

//�֐���`
float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color);
float4 SetToon(float4 shadecolor);
float3 BRDF(float3 L, float3 V, float3 color);
float SchlickFresnel(float f0, float f90, float cosine);
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH);
float DistributionGGX(float alpha, float NdotH);
float3 SchlickFresnel3(float3 f0, float3 f90, float cosine);
float3 DisneyFresnel(float LdotH);
float GeometricSmith(float cosine);

float4 main(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
	float4 texcolor = tex.Sample(smp, input.uv);

	//�L���[�u�}�b�v
	float4 cubeColor = cubeTex.Sample(smp, input.normal);

	// ����x
	const float shininess = 4.0f;

	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	// �����ˌ�
	float3 ambient = m_ambient;

	// �V�F�[�f�B���O�ɂ��F
	float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	N = input.normal;
	//�o�͐F
	float3 finalRGB = float3(0, 0, 0);


	//���s����
	for (int i = 0; i < DIRLIGHT_NUM; i++)
	{
		if (dirLights[i].active)
		{
			//// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
			//float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);

			//// ���ˌ��x�N�g��
			//float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
			//// �g�U���ˌ�
			//float3 diffuse = dotlightnormal * m_diffuse;
			//// ���ʔ��ˌ�
			//float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			//// �S�ĉ��Z����
			//shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;

			finalRGB += BRDF(dirLights[i].lightv, eyedir, cubeColor.rgb) * dirLights[i].lightcolor;
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

	//�|�X�g�G�t�F�N�g����

	//�u���[������
	float4 bloom = float4(0, 0, 0, 0);
	if (isBloom)
	{
		bloom = SetBloom(shadecolor, texcolor, color);
	}

	//�g�D�[���V�F�[�h
	if (isToon)
	{
		shadecolor = SetToon(shadecolor);
	}

	// �V�F�[�f�B���O�ɂ��F�ŕ`��
	float4 output = float4(finalRGB.rgb, color.w);
	return output;
}

/// <summary>
/// �u���[���̃Z�b�g
/// </summary>
float4 SetBloom(float4 shadecolor, float4 texcolor, float4 color)
{
	//���x�l�̒��o
	float LuminousIntensity = dot(shadecolor.rgb * texcolor.rgb, float3(0.2125, 0.7154, 0.0712));

	//�u���[����������ꏊ
	float4 bloomColor = step(1.0, LuminousIntensity) * texcolor * color;

	return bloomColor;
}

/// <summary>
/// �g�D�[���̃Z�b�g
/// </summary>
float4 SetToon(float4 shadecolor)
{
	//�g�D�[���̐F�͈�
	float toonLighrRange = 0.525;
	//���Â̒��ԕ�
	float contourWidth = 0.1;

	//���邢
	float3 bright;
	bright.r = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.r);
	bright.g = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.g);
	bright.b = smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.b);
	//�Â�
	float3 dark;
	dark.r = (1 - smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.r));
	dark.g = (1 - smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.g));
	dark.b = (1 - smoothstep(toonLighrRange, toonLighrRange + contourWidth, shadecolor.b));

	//���݂̐F
	return float4(bright + dark, 1);
}

/// <summary>
/// �o�������˕��z�֐�
/// </summary>
float3 BRDF(float3 L, float3 V,float3 color)
{
	//�@���ƃ��C�g�����̓���
	float NdotL = dot(N, L);
	//�@���ƃJ���������̓���
	float NdotV = dot(N, V);
	//�ǂ��炩��90���ȏ�Ȃ獕��Ԃ�
	if (NdotL < 0 || NdotV < 0) { return float3(0, 0, 0); }

	//���C�g�����ƃJ���������̒���
	float3 H = normalize(L + V);
	//�@���ƃn�[�t�x�N�g���̓���
	float NdotH = dot(N, H);
	//���C�g�ƃn�[�t�x�N�g���̓���
	float LdotH = dot(L, H);

	//�g�U���˗�
	float diffuseReflectance = 1.0f / PI;

	//90�x�̏ꍇ�̃��t�l�X��������
	float energyBias = 0.5f * m_roughness;
	//���ˊp��90�x�̏ꍇ�̊g�U���˗�
	float Fd90 = energyBias + 2.0f * LdotH * LdotH * m_roughness;
	//�����Ă������̊g�U���˗�
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	//�o�Ă������̊g�U���˗�
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	//�ŏI�I�Ȕ��˗��Ƀ��t�l�X�ɂ�錸����ǉ�
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, m_roughness);
	//�����ďo�Ă����܂ł̊g�U���˗�
	float Fd = FL * FV * energyFactor;

	//�g�U���ˌ�
	float3 diffuseColor = diffuseReflectance * Fd * m_baseColor * (1 - m_metalness);

	//���ʔ��ˍ�
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH);

	return diffuseColor + specularColor * color;
}

/// <summary>
/// �t���l���̋ߎ��l(float)
/// </summary>
float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

/// <summary>
/// ���ʔ��˂̌v�Z
/// </summary>
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH)
{
	//D��
	float Ds = DistributionGGX(m_roughness * m_roughness, NdotH);
	//F��(���z : Distribution)
	float3 Fs = DisneyFresnel(LdotH);
	//G��(�􉽌��� : Geometry attenuation)
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);
	//m��(����)
	float m = 1.0f * NdotL * NdotV;

	//�������ċ��ʔ��˂̐F��Ԃ�
	return Ds * Fs * Gs / m;
}

/// <summary>
/// D��
/// </summary
float DistributionGGX(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

/// <summary>
/// �t���l���̋ߎ��l(float3)
/// </summary
float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

/// <summary>
/// �f�B�Y�j�[�̃t���l���v�Z
/// </summary
float3 DisneyFresnel(float LdotH)
{
	//F��(�t���l�� : Fresnel)
	//�P�x
	float liminance = 0.3f * m_baseColor.r + 0.6f * m_baseColor.g + 0.1f * m_baseColor.b;
	//�F����
	float3 tintColor = m_baseColor / liminance;
	//������̋��ʔ��ːF�̌v�Z
	float3 nonMetarlColor = m_specular * 0.08f * tintColor;
	//metalness�ɂ��F��� �����̏ꍇ�̓x�[�X�J���[
	float3 specularColor = lerp(nonMetarlColor, m_baseColor, m_metalness);

	//NdptH�̊�����SchlickFresnel���
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

/// <summary>
/// UE4��Smith���f��
/// </summary
float GeometricSmith(float cosine)
{
	float k = (m_roughness + 1.0f);
	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}