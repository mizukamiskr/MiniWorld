
#ifndef POSTPROCESS_HLSL
#define POSTPROCESS_HLSL

Texture2D g_Tex : register(t0);
SamplerState g_Sam : register(s0);

cbuffer CB : register(b0)
{
    float g_VisibleRange; // 3D������ӷ�Χ
    float3 g_EyePosW; // �����λ��
    float4 g_RectW; // С��ͼxOzƽ���Ӧ3D�����������(Left, Front, Right, Back)
}

struct VertexPosTex
{
    float3 posL : POSITION;
    float2 tex : TEXCOORD;
};

struct VertexPosHTex
{
    float4 posH : SV_POSITION;
    float2 tex : TEXCOORD;
};


// һ�Ĥ������Τ�ȫ�����
// (-1, 1)________ (3, 1)
//        |   |  /
// (-1,-1)|___|/ (1, -1)   
//        |  /
// (-1,-3)|/      
VertexPosHTex FadeVS(uint vertexID : SV_VertexID)
{
    VertexPosHTex vOut;
    float2 grid = float2((vertexID << 1) & 2, vertexID & 2);
    float2 xy = grid * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f);
    vOut.tex = grid * float2(1.0f, 1.0f);
    vOut.posH = float4(xy, 1.0f, 1.0f);
    return vOut;
}

// ������ɫ��
float4 FadePS(VertexPosHTex pIn, uniform float fadeAmount) : SV_Target
{
    return g_Tex.Sample(g_Sam, pIn.tex) * float4(fadeAmount, fadeAmount, fadeAmount, 1.0f);
}




#endif //POSTPROCESS_HLSL