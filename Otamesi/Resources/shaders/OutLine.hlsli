// カメラバッファ
cbuffer CONSTANT_BUFFER_CAMERA : register(b0)
{
    row_major float4x4 view_projection; // ビュープロジェクション
    row_major float4x4 inv_view_projection; // ビュープロジェクション逆行列
};


//頂点シェーダからピクセルシェーダへのやり取りに使用する構造体
struct VSOutput
{
    float4 viewprojection_position : SV_POSITION; // VP座標
    float4 world_position : W_POSITION; // ワールド座標座標
	float2 uv : TEXCOORD;	//uv値
};

struct PSOutput
{
	float4 color				: SV_TARGET0;	// 色画像
	float4 depth				: SV_TARGET1;	// 深度画像
};