#include "Particle.hlsli"

//四角形の頂点数
static const uint vnum = 4;

//センターからのオフセット
static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0), //左下
	float4(-0.5f, +0.5f, 0, 0), //左上
	float4(+0.5f, -0.5f, 0, 0), //右下
	float4(+0.5f, +0.5f, 0, 0), //右上
};

//左上が0,0　右下が1,1
static const float2 uv_array[vnum] =
{
	float2(0,1), //左下
	float2(0,0), //左上
	float2(1,1), //右下
	float2(1,0), //右上
};

//点の入力から、四角形を出力
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : SV_POSITION,
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element;
	//4点分まわす
	for (uint i = 0; i < vnum; i++)
	{
		float4 offset = offset_array[i] * input[0].scale;
		//中心からのオフセットをビルボード回転(モデル座標)
		offset = mul(matBillboard, offset);

		//オフセット分ずらす(ワールド座標)
		element.svpos = input[0].pos + offset;
		//ビュープロジェクション行列
		element.svpos = mul(mat, element.svpos);

		//回転角からラジアンを算出
		const float PI = 3.14159265359f;
		float radian = (input[0].rot * PI / 180);
		float s = sin(radian);
		float c = cos(radian);
		float2x2 rotMatrix = float2x2(c, -s, s, c);
		//uv値をずらして回転させる
		float2 uv = uv_array[i] - float2(0.5f, 0.5f);
		uv = mul(rotMatrix, uv);
		uv += float2(0.5f, 0.5f);
		element.uv = uv;

		element.color = input[0].color;
		output.Append(element);
	}
}