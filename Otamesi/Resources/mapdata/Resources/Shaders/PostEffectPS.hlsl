#include "PostEffect.hlsli"

Texture2D<float4> tex:register(t0);//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> bloom:register(t1);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> outline:register(t2);//2�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float> depthTex:register(t3);//1�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp:register(s0);//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

/// <summary>
/// �K�E�X����
/// </summary>
float Gaussian(float2 drawUV, float2 pickUV, float sigma);

/// <summary>
/// �u���[������
/// </summary>
float4 SetBloom(float2 uv);

/// <summary>
/// �A�E�g���C������
/// </summary>
float4 SetOutline(float2 uv, float outlineWidth, float4 outlineColor);

/// <summary>
/// �t�H�O����
/// </summary>
float4 SetFog(float2 uv);

float4 main(VSOutput input) : SV_TARGET
{
	//���C���J���[
	float4 mainColor = tex.Sample(smp, input.uv);

	//bloom����
	float4 bloom = SetBloom(input.uv);

	//�A�E�g���C������
	float4 outline = SetOutline(input.uv, outlineWidth, outlineColor);

	//�t�H�O����

	float4 fog = float4(0, 0, 0, 0);
	if (isFog)
	{
		fog = SetFog(input.uv);
	}

	//�|�X�g�G�t�F�N�g�̍���
	return mainColor;
}

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-d * d) / (2 * sigma * sigma);
}

float4 SetBloom(float2 uv)
{
	float totalWeight = 0;
	float sigma = 0.01;
	float stepWidth = 0.001;
	float4 color = { 0, 0, 0, 0 };

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
	{
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
		{
			float2 pickUV = uv + float2(px, py);
			float weight = Gaussian(uv, pickUV, sigma);
			color += bloom.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}

	color.rgb = color.rgb / totalWeight;

	//0.3�ȉ��؂�̂�
	//color = color * step(0.8,color.r + color.g);
	return color;
}

float4 SetOutline(float2 uv, float outlineWidth, float4 outlineColor)
{
	float4 outlineTex = float4(0, 0, 0, 0);

	float xPoutline = uv.x + outlineWidth;
	float xMoutline = uv.x - outlineWidth;
	float yPoutline = uv.y + outlineWidth;
	float yMoutline = uv.y - outlineWidth;

	float add1 = xPoutline < 1;
	float add2 = xMoutline > 0;
	float add3 = yPoutline < 1;
	float add4 = yMoutline > 0;

	outlineTex = outlineTex + outline.Sample(smp, float2(xPoutline, yPoutline)) * (add1 + add3);
	outlineTex = outlineTex + outline.Sample(smp, float2(xPoutline, yMoutline)) * (add1 + add4);
	outlineTex = outlineTex + outline.Sample(smp, float2(xMoutline, yPoutline)) * (add2 + add3);
	outlineTex = outlineTex + outline.Sample(smp, float2(xMoutline, yMoutline)) * (add2 + add4);
	outlineTex = outlineTex + outline.Sample(smp, float2(xPoutline, uv.y)) * add1;
	outlineTex = outlineTex + outline.Sample(smp, float2(xMoutline, uv.y)) * add2;
	outlineTex = outlineTex + outline.Sample(smp, float2(uv.x, yPoutline)) * add3;
	outlineTex = outlineTex + outline.Sample(smp, float2(uv.x, yMoutline)) * add4;

	float4 normalColor = outline.Sample(smp, uv);
	outlineTex.rgb = outlineTex.rgb - normalColor.rgb * 8.0;

	//�J���[�̍��v�l
	float addColor = 0;
	addColor = outlineTex.r + outlineTex.g + outlineTex.b;
	//0.5���傫����΃A�E�g���C������
	addColor = step(0.1, addColor);//0.5���傫����΃A�E�g���C������

	outlineTex.r = outlineColor.r * addColor;
	outlineTex.g = outlineColor.g * addColor;
	outlineTex.b = outlineColor.b * addColor;

	return outlineTex;
}

float4 SetFog(float2 uv)
{
	float depth = pow(depthTex.Sample(smp, uv), 10000);
	depth = depth * depth;
	float4 depthColor = float4(0, depth/2, depth, 1.0);

	return depthColor;
}