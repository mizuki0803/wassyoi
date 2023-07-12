#include "PostEffect.hlsli"

Texture2D<float4> baseTex : register(t0);	//0番スロットに設定されたテクスチャ
Texture2D<float4> idTex : register(t1);	    //1番スロットに設定されたテクスチャ
Texture2D<float4> playerTex : register(t2);	//2番スロットに設定されたテクスチャ
Texture2D<float4> goalTex : register(t3); //3番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	// 最終的な出力
    float4 outputColor;
	
    float4 baseColor = baseTex.Sample(smp, input.uv);
    float4 idColor = idTex.Sample(smp, input.uv);
    float4 playerColor = playerTex.Sample(smp, input.uv);
    float4 goalColor = goalTex.Sample(smp, input.uv);
	
    
    // ID抽出
    float isPlayerID = step(0.5f, playerColor.b);
    float isGoalID = step(0.5f, goalColor.r);
    float isStageID = step(0.5f, idColor.g);
    
    float isOutline = step(1.0f, (isPlayerID + isStageID) / 2.0f);
    float isGoal = step(1.0f, (isGoalID + isStageID) / 2.0f);
    
    float4 outline =
    lerp(
        float4(0.0f,0.0f,0.0f,0.0f),
        float4(0.0f,0.0f,1.0f,0.5f),
        isOutline
    );
    float4 goalPosition =
    lerp(
        float4(0.0f,0.0f,0.0f,0.0f),
        float4(1.0f,0.0f,0.0f,0.5f),
        isGoal
    );

    outputColor = baseColor + outline + goalPosition;
    return float4(outputColor.rgb, baseColor.w);
}