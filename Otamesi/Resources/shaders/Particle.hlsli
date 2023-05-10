cbuffer cbuff : register(b0)
{	
	matrix mat;	//ビュープロジェクション行列
	matrix matBillboard;	//ビルボード行列
};

//頂点シェーダからピクセルシェーダへのやり取りに使用する構造体
struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
	float scale : TEXCOORD;	//スケール
	float rot : ROTATION; //回転角
	float4 color : COLOR;	//色
};

//ジオメトリシェーダからピクセルシェーダへの出力
struct GSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float2 uv : TEXCOORD;		//uv値
	float4 color : COLOR;	 //色
};