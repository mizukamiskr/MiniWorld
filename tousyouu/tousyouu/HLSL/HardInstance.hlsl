#ifndef HARDINSTANCE_HLSL
#define HARDINSTANCE_HLSL


Texture2D g_DiffuseMap : register(t0);
Texture2DArray g_TexArray : register(t1);
SamplerState g_Sam : register(s0);


cbuffer CBChangesEveryInstanceDrawing : register(b0)
{
    matrix g_World;
    matrix g_WorldInvTranspose;
    float4 g_ConstantDiffuseColor;
}

cbuffer CBChangesEveryFrame : register(b2)
{
    matrix g_ViewProj;
    float3 g_EyePosW;
    float g_Pad;
}

struct VertexPosNormalTex
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 tex : TEXCOORD;
};

struct VertexPosColorTex
{
    float3 PosL : POSITIONT;
    float4 color : COLOR;
};

struct InstancePosNormalTex
{
    float3 posL : POSITION;
    float3 normalL : NORMAL;
    float2 tex : TEXCOORD;
    uint faceID : FACEID; // ���һ�����ʶ��
    matrix world : World;
    matrix worldInvTranspose : WorldInvTranspose;
    float4 color : COLOR;
    uint TexIndex : TEXINDEX;
};

struct VertexPosHWNormalColorTex
{
    float4 posH : SV_POSITION;
    float3 posW : POSITION; // �������е�λ��
    float3 normalW : NORMAL; // �������������еķ���
    float4 color : COLOR;
    float2 tex : TEXCOORD;
    uint TexIndex : TEXINDEX;
    uint faceID : FACEID; //�ɤ�����жϤ���ե饰
};

uint DetermineTexIndex(uint faceID, uint texIndex)
{
    //��ˤ�ä��`���ƥ������㤬��Ҫ���ɤ����жϤ���
    if (texIndex == 5) // �ݤΥ֥�å�
    {
        if (faceID == 2) //��
            return 5;
        else if (faceID == 3) //��
            return 7; //
        else
            return 6; //�܇�
    }
    else
    {
        //��������Υ֥�å���6��ȫ��ͬ���ƥ�������ˏ��ä���
        return texIndex;
    }
}


// VS
VertexPosHWNormalColorTex InstanceVS(InstancePosNormalTex vIn)
{
    VertexPosHWNormalColorTex vOut;
    
    vector posW = mul(float4(vIn.posL, 1.0f), vIn.world);

    vOut.posW = posW.xyz;
    vOut.posH = mul(posW, g_ViewProj);
    vOut.normalW = mul(vIn.normalL, (float3x3) vIn.worldInvTranspose);
    vOut.color = vIn.color;
    vOut.tex = vIn.tex;
    vOut.TexIndex = vIn.TexIndex;
    vOut.faceID = vIn.faceID;
    return vOut;
}

VertexPosHWNormalColorTex InstanceObjectVS(VertexPosNormalTex vIn)
{
    VertexPosHWNormalColorTex vOut;
    
    vector posW = mul(float4(vIn.posL, 1.0f), g_World);

    vOut.posW = posW.xyz;
    vOut.posH = mul(posW, g_ViewProj);
    vOut.normalW = mul(vIn.normalL, (float3x3) g_WorldInvTranspose);
    vOut.color = g_ConstantDiffuseColor;
    vOut.tex = vIn.tex;
    return vOut;
}

// PS
float4 InstancePS(VertexPosHWNormalColorTex pIn) : SV_Target
{
    uint texIndex = DetermineTexIndex(pIn.faceID, pIn.TexIndex);
    
    float4 texColor = g_TexArray.Sample(g_Sam, float3(pIn.tex, texIndex));

    clip(texColor.a - 0.1f);

    // normalize
    float3 normalW = normalize(pIn.normalW);

    float3 lightDir[4] =
    {
        float3(-1.0f, 1.0f, -1.0f),
        float3(1.0f, 1.0f, -1.0f),
        float3(0.0f, -1.0f, 0.0f),
        float3(1.0f, 1.0f, 1.0f)
    };


    float lighting = saturate(dot(lightDir[0], normalW)) * 0.05f +
                     saturate(dot(lightDir[1], normalW)) * 0.05f +
                     saturate(dot(lightDir[2], normalW)) * 0.05f +
                     saturate(dot(lightDir[3], normalW)) * 0.05f;


    float4 litColor = texColor * lighting;
    return litColor;
}

float4 InstanceObjectPS(VertexPosHWNormalColorTex pIn) : SV_Target
{
    
    float4 texColor = g_DiffuseMap.Sample(g_Sam, pIn.tex);

    clip(texColor.a - 0.1f);

    // normalize
    float3 normalW = normalize(pIn.normalW);

    float3 lightDir[4] =
    {
        float3(-1.0f, 1.0f, -1.0f),
        float3(1.0f, 1.0f, -1.0f),
        float3(0.0f, -1.0f, 0.0f),
        float3(1.0f, 1.0f, 1.0f)
    };


    float lighting = saturate(dot(lightDir[0], normalW)) * 0.05f +
                     saturate(dot(lightDir[1], normalW)) * 0.05f +
                     saturate(dot(lightDir[2], normalW)) * 0.05f +
                     saturate(dot(lightDir[3], normalW)) * 0.05f;


    float4 litColor = texColor * lighting;
    return litColor;
}




#endif //HARDINSTANCE_HLSL