cbuffer cbuff0 : register(b0)
{
	uint isRadialBlur; //ラジアルブラーをかけるか
	uint radialBlurSampleNum; //ラジアルブラーのサンプル回数
	float radialBlurStrength; //ラジアルブラーの広がる強さ
};

//頂点シェーダからピクセルシェーダへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float2 uv : TEXCOORD;	//uv値
};
