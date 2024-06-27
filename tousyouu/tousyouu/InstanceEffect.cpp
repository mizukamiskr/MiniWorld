#include "Effects.h"
#include "XUtil.h"
#include "RenderStates.h"
#include "EffectHelper.h"
#include "DXTrace.h"
#include "Vertex.h"
#include "TextureManager.h"
#include "ModelManager.h"

using namespace DirectX;

# pragma warning(disable: 26812)


//
// InstanceEffect::Impl 需要先于BasicEffect的定义
//

class InstanceEffect::Impl
{
public:
    // 必须显式指定
    Impl() {}
    ~Impl() = default;

public:
    template<class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    std::unique_ptr<EffectHelper> m_pEffectHelper;

    std::shared_ptr<IEffectPass> m_pCurrEffectPass;
    ComPtr<ID3D11InputLayout> m_pCurrInputLayout;
    D3D11_PRIMITIVE_TOPOLOGY m_CurrTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

    ComPtr<ID3D11InputLayout> m_pInstancePosNormalTexLayout;
    ComPtr<ID3D11InputLayout> m_pVertexPosNormalTexLayout;
    ComPtr<ID3D11InputLayout> m_pVertexPosColorLayout;
    ComPtr<ID3D11ShaderResourceView> m_pTextures;				// テクスチャ配列

    XMFLOAT4X4 m_World{}, m_View{}, m_Proj{};
};

/*InstanceEffect*/

namespace
{
    // BasicEffect单例
    static InstanceEffect* g_pInstance = nullptr;
}

InstanceEffect::InstanceEffect()
{
    if (g_pInstance) 
    {
        throw std::exception("InstanceEffect is a singleton!");
    }
    g_pInstance = this;
    pImpl = std::make_unique<InstanceEffect::Impl>();
}

InstanceEffect::~InstanceEffect()
{

}

InstanceEffect::InstanceEffect(InstanceEffect&& moveFrom) noexcept
{
    pImpl.swap(moveFrom.pImpl);
}

InstanceEffect& InstanceEffect::operator=(InstanceEffect&& moveFrom) noexcept
{
    pImpl.swap(moveFrom.pImpl);
    return *this;
}

InstanceEffect& InstanceEffect::Get()
{
    if (!g_pInstance)
        throw std::exception("InstanceEffect needs an instance!");
    return *g_pInstance;
}

bool InstanceEffect::InitAll(ID3D11Device* device)
{
    if (!device)
        return false;

    if (!RenderStates::IsInit())
        throw std::exception("RenderStates need to be initialized first!");

    pImpl->m_pEffectHelper = std::make_unique<EffectHelper>();

    //CSOファイル生成する場所指定する
    pImpl->m_pEffectHelper->SetBinaryCacheDirectory(L"HLSL\\Cache");

    // 実体InputLayoutを指定する
    D3D11_INPUT_ELEMENT_DESC basicInstLayout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "FACEID", 0, DXGI_FORMAT_R32_UINT, 3, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "World", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "World", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "World", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "World", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "WorldInvTranspose", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "WorldInvTranspose", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "WorldInvTranspose", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "WorldInvTranspose", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 4, 128, D3D11_INPUT_PER_INSTANCE_DATA, 1},
        { "TEXINDEX", 0, DXGI_FORMAT_R32_UINT, 4, 144, D3D11_INPUT_PER_INSTANCE_DATA, 1} 
    };

    Microsoft::WRL::ComPtr<ID3DBlob> blob;


    // 创建顶点着色器
    HR(pImpl->m_pEffectHelper->CreateShaderFromFile("InstanceVS", L"HLSL/HardInstance.hlsl", device,
        "InstanceVS", "vs_5_0", nullptr, blob.GetAddressOf()));
    // 创建顶点布局
    HR(device->CreateInputLayout(basicInstLayout, ARRAYSIZE(basicInstLayout),
        blob->GetBufferPointer(), blob->GetBufferSize(), pImpl->m_pInstancePosNormalTexLayout.GetAddressOf()));

    // 创建顶点着色器
    HR(pImpl->m_pEffectHelper->CreateShaderFromFile("InstanceObjectVS", L"HLSL/HardInstance.hlsl", device,
        "InstanceObjectVS", "vs_5_0", nullptr, blob.GetAddressOf()));
    // 创建顶点布局
    HR(device->CreateInputLayout(VertexPosNormalTex::GetInputLayout(), ARRAYSIZE(VertexPosNormalTex::GetInputLayout()),
        blob->GetBufferPointer(), blob->GetBufferSize(), pImpl->m_pVertexPosNormalTexLayout.GetAddressOf()));

    // 创建像素着色器
    HR(pImpl->m_pEffectHelper->CreateShaderFromFile("InstancePS", L"HLSL\\HardInstance.hlsl",
        device, "InstancePS", "ps_5_0"));

    HR(pImpl->m_pEffectHelper->CreateShaderFromFile("InstanceObjectPS", L"HLSL\\HardInstance.hlsl",
        device, "InstanceObjectPS", "ps_5_0"));

    // 创建通道
    EffectPassDesc passDesc;
    passDesc.nameVS = "InstanceVS";
    passDesc.namePS = "InstancePS";
    HR(pImpl->m_pEffectHelper->AddEffectPass("BasicInstance", device, &passDesc));

    passDesc.nameVS = "InstanceObjectVS";
    passDesc.namePS = "InstanceObjectPS";
    HR(pImpl->m_pEffectHelper->AddEffectPass("InstanceObject", device, &passDesc));


    pImpl->m_pEffectHelper->SetSamplerStateByName("g_Sam", RenderStates::SSLinearWrap.Get());

    // 设置调试对象名
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
    SetDebugObjectName(pImpl->m_pVertexPosNormalTexLayout.Get(), "InstanceEffect.VertexPosNormalTexLayout");
    SetDebugObjectName(pImpl->m_pInstancePosNormalTexLayout.Get(), "InstanceEffect.InstancePosNormalTexLayout");
#endif
    pImpl->m_pEffectHelper->SetDebugObjectName("InstanceEffect");

    return true;
}

void XM_CALLCONV InstanceEffect::SetWorldMatrix(DirectX::FXMMATRIX W)
{
    XMStoreFloat4x4(&pImpl->m_World, W);
}

void XM_CALLCONV InstanceEffect::SetViewMatrix(DirectX::FXMMATRIX V)
{
    XMStoreFloat4x4(&pImpl->m_View, V);
}

void XM_CALLCONV InstanceEffect::SetProjMatrix(DirectX::FXMMATRIX P)
{
    XMStoreFloat4x4(&pImpl->m_Proj, P);
}

void InstanceEffect::SetMaterial(const Material& material)
{
    TextureManager& tm = TextureManager::Get();

    auto pStr = material.TryGet<std::string>("$Diffuse");
    pImpl->m_pEffectHelper->SetShaderResourceByName("g_DiffuseMap", pStr ? tm.GetTexture(*pStr) : tm.GetNullTexture());
}

void InstanceEffect::SetTextureArray(ID3D11ShaderResourceView* textures)
{
    pImpl->m_pTextures = textures;
    pImpl->m_pEffectHelper->SetShaderResourceBySlot(1, pImpl->m_pTextures.Get());
}

MeshDataInput InstanceEffect::GetInputData(const MeshData& meshData)
{
    MeshDataInput input;
    input.pInputLayout = pImpl->m_pCurrInputLayout.Get();
    input.topology = pImpl->m_CurrTopology;
    
        input.pVertexBuffers = {
            meshData.m_pVertices.Get(),
            meshData.m_pNormals.Get(),
            meshData.m_pTexcoordArrays.empty() ? nullptr : meshData.m_pTexcoordArrays[0].Get(),
            meshData.m_pFaceID.Get(),
            nullptr
        };

        input.strides = { 12, 12, 8 ,4,148 };
        input.offsets = { 0, 0, 0, 0 ,0};

        input.pIndexBuffer = meshData.m_pIndices.Get();
        input.indexCount = meshData.m_IndexCount;

    return input;
}

void InstanceEffect::SetLightDir(const DirectX::XMFLOAT3& dir)
{
    pImpl->m_pEffectHelper->GetConstantBufferVariable("g_LightDir")->SetFloatVector(3, (const float*)&dir);
}

void InstanceEffect::SetEyePos(const DirectX::XMFLOAT3& eyePos)
{
    pImpl->m_pEffectHelper->GetConstantBufferVariable("g_EyePosW")->SetFloatVector(3, reinterpret_cast<const float*>(&eyePos));
}

void InstanceEffect::SetDiffuseColor(const DirectX::XMFLOAT4& color)
{
    pImpl->m_pEffectHelper->GetConstantBufferVariable("g_ConstantDiffuseColor")->SetFloatVector(4, reinterpret_cast<const float*>(&color));
}

void InstanceEffect::SetRenderDefault()
{
    pImpl->m_pCurrEffectPass = pImpl->m_pEffectHelper->GetEffectPass("InstanceObject");
    pImpl->m_pCurrInputLayout = pImpl->m_pVertexPosNormalTexLayout;
    pImpl->m_CurrTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

void InstanceEffect::SetRenderCollision()
{
    pImpl->m_pCurrEffectPass = pImpl->m_pEffectHelper->GetEffectPass("CollisionDraw");
    pImpl->m_pCurrInputLayout = pImpl->m_pVertexPosColorLayout;
    pImpl->m_CurrTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

void InstanceEffect::DrawInstanced(ID3D11DeviceContext* deviceContext, Buffer& buffer, const GameObject& object, uint32_t numObjects)
{
    deviceContext->IASetInputLayout(pImpl->m_pInstancePosNormalTexLayout.Get());
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    auto pPass = pImpl->m_pEffectHelper->GetEffectPass("BasicInstance");

    XMMATRIX V = XMLoadFloat4x4(&pImpl->m_View);
    XMMATRIX P = XMLoadFloat4x4(&pImpl->m_Proj);

    XMMATRIX VP = V * P;
    VP = XMMatrixTranspose(VP);
    pImpl->m_pEffectHelper->GetConstantBufferVariable("g_ViewProj")->SetFloatMatrix(4, 4, (FLOAT*)&VP);

    const Model* pModel = object.GetModel();
    size_t sz = pModel->meshdatas.size();
    for (size_t i = 0; i < sz; ++i)
    {
        SetMaterial(pModel->materials[pModel->meshdatas[i].m_MaterialIndex]);
        pPass->Apply(deviceContext);

        MeshDataInput input = GetInputData(pModel->meshdatas[i]);
        input.pVertexBuffers.back() = buffer.GetBuffer();
        deviceContext->IASetVertexBuffers(0, (uint32_t)input.pVertexBuffers.size(),
            input.pVertexBuffers.data(), input.strides.data(), input.offsets.data());
        deviceContext->IASetIndexBuffer(input.pIndexBuffer, input.indexCount > 65535 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT, 0);

        deviceContext->DrawIndexedInstanced(input.indexCount, numObjects, 0, 0, 0);
    }
}

void InstanceEffect::Apply(ID3D11DeviceContext* deviceContext)
{
    XMMATRIX W = XMLoadFloat4x4(&pImpl->m_World);
    XMMATRIX V = XMLoadFloat4x4(&pImpl->m_View);
    XMMATRIX P = XMLoadFloat4x4(&pImpl->m_Proj);

    XMMATRIX VP = V * P;
    XMMATRIX WInvT = XMath::InverseTranspose(W);

    W = XMMatrixTranspose(W);
    VP = XMMatrixTranspose(VP);
    WInvT = XMMatrixTranspose(WInvT);


    pImpl->m_pEffectHelper->GetConstantBufferVariable("g_WorldInvTranspose")->SetFloatMatrix(4, 4, (FLOAT*)&WInvT);
    pImpl->m_pEffectHelper->GetConstantBufferVariable("g_ViewProj")->SetFloatMatrix(4, 4, (FLOAT*)&VP);
    pImpl->m_pEffectHelper->GetConstantBufferVariable("g_World")->SetFloatMatrix(4, 4, (FLOAT*)&W);

    if (pImpl->m_pCurrEffectPass)
        pImpl->m_pCurrEffectPass->Apply(deviceContext);
}


