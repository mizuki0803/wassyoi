cbuffer cbuff0 : register(b0)
{
	matrix viewproj;	//ビュープロジェクション行列
	float3 cameraPos;	//カメラ座標(ワールド座標)
	matrix lightViewproj;	//ライトビュープロジェクション行列
	uint isShadowMap;	//影を付けるか
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient : packoffset(c0);	//アンビエント係数
	float3 m_diffuse : packoffset(c1);	//ディフューズ係数
	float3 m_specular : packoffset(c2);	//スペキュラー係数
	float m_alpha : packoffset(c2.w);	//アルファ
};

static const int INSTANCE_NUM = 512;
cbuffer cbuff2 : register(b3)
{
	float4 color[INSTANCE_NUM];//色
	matrix world[INSTANCE_NUM]; // ワールド行列
};

cbuffer cbuff3 : register(b4)
{
    float4 idColor;
};


//平行光源の数
static const int DIRLIGHT_NUM = 3;
struct DirLight
{
	float3 lightv;		//ライトへの方向の単位ベクトル
	float3 lightcolor;	//ライトの色(RGB)
	uint active;		//有効フラグ
};

//点光源の数
static const int POINTLIGHT_NUM = 3;
struct PointLight
{
	float3 lightpos;	//ライト座標
	float3 lightcolor;	//ライトの色(RGB)
	float3 lightatten;	//ライト距離減衰係数
	uint active;		//有効フラグ
};

//スポットライトの数
static const int SPOTLIGHT_NUM = 3;
struct SpotLight
{
	float3 lightv;				//ライトの光線方向の逆ベクトル
	float3 lightpos;			//ライト座標
	float3 lightcolor;			//ライトの色(RGB)
	float3 lightatten;			//ライト距離減衰係数
	float2 lightfactoranglecos;	//ライト減衰角度のコサイン
	uint active;				//有効フラグ
};

//丸影数
static const int CIRCLESHADOW_NUM = 3;
struct CircleShadow
{
	float3 dir;					//投影方向の逆ベクトル(単位ベクトル)
	float3 casterPos;			//キャスター座標
	float distanceCasterLight;	//キャスターとライトの距離
	float3 atten;				//影距離減衰係数
	float2 factorAngleCos;		//減衰角度のコサイン
	uint active;				//有効フラグ
};

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
	SpotLight spotLights[SPOTLIGHT_NUM];
	CircleShadow circleShadows[CIRCLESHADOW_NUM];
};

//頂点シェーダからピクセルシェーダへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION;	//システム用頂点座標
	float4 worldpos : POSITION;	//ワールド座標
	float3 normal : NORMAL;	//法線
	float2 uv : TEXCOORD;	//uv値
	float4 shadowpos : POSITION1;
	uint instNo : SV_InstanceID;//インスタンシング描画用
};

struct PSOutput
{
    float4 color : SV_TARGET0;	// 色画像
    float4 player : SV_TARGET1; // プレイヤー画像
    float4 id : SV_TARGET2;		// 深度画像
    float4 goal : SV_Target3; // Goal画像
};