cbuffer cbuff0 : register(b0)
{
    float4 color; //色(RGBA)
    matrix mat; //3D変換行列
}
cbuffer CameraBuffer : register(b1)
{
    float4 camera_position; //色(RGBA)
    row_major float4x4	view_projection; //3D変換行列
    row_major float4x4	inv_view_projection; //3D変換行列

}

//頂点シェーダーからピクセルシェーダ―へのやり取りに使用する構造体
struct VSOutput {
	float4 svpos : SV_POSITION;	//システム用長点座標
	float2 uv : TEXCOORD;	//uv値
};