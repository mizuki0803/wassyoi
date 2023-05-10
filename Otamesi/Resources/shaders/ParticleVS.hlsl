#include "Particle.hlsli"

VSOutput main(float4 pos : POSITION, float scale : TEXCOORD, float rot : ROTATION, float4 color : COLOR)
{
	VSOutput output;	//ピクセルシェーダに渡す値
	output.pos = pos;
	output.scale = scale;
	output.rot = rot;
	output.color = color;
	return output;
}