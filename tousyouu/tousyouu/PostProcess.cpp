#include "Effects.h"
#include "XUtil.h"
#include "RenderStates.h"
#include "EffectHelper.h"
#include "DXTrace.h"
#include "Vertex.h"
#include "TextureManager.h"
#include "ModelManager.h"
using namespace DirectX;

class PostProcessEffect::Impl
{
public:
    Impl() {}
    ~Impl() = default;

public:
    template<class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    std::unique_ptr<EffectHelper> m_pEffectHelper;

};

namespace
{
    // PostProcessEffect����
    static PostProcessEffect* g_pInstance = nullptr;
}

PostProcessEffect::PostProcessEffect()
{
    if (g_pInstance)
        throw std::exception("PostProcessEffect is a singleton!");
    g_pInstance = this;
    pImpl = std::make_unique<PostProcessEffect::Impl>();
}

PostProcessEffect::~PostProcessEffect()
{
}

PostProcessEffect::PostProcessEffect(PostProcessEffect&& moveFrom) noexcept
{
    pImpl.swap(moveFrom.pImpl);
}

PostProcessEffect& PostProcessEffect::operator=(PostProcessEffect&& moveFrom) noexcept
{
    pImpl.swap(moveFrom.pImpl);
    return *this;
}

PostProcessEffect& PostProcessEffect::Get()
{
    if (!g_pInstance)
        throw std::exception("PostProcessEffect needs an instance!");
    return *g_pInstance;
}

bool PostProcessEffect::InitAll(ID3D11Device* device)
{
    if (!device)
        return false;

    if (!RenderStates::IsInit())
        throw std::exception("RenderStates need to be initialized first!");

    pImpl->m_pEffectHelper = std::make_unique<EffectHelper>();

    //CSO�ե��������ɤ������ָ������
    pImpl->m_pEffectHelper->SetBinaryCacheDirectory(L"HLSL\\Cache");

    Microsoft::WRL::ComPtr<ID3DBlob> blob;

    // ����������ɫ��
    HR(pImpl->m_pEffectHelper->CreateShaderFromFile("FadeVS", L"HLSL/PostProcess.hlsl", device,
        "FadeVS", "vs_5_0", nullptr, blob.GetAddressOf()));

    // ����������ɫ��
    HR(pImpl->m_pEffectHelper->CreateShaderFromFile("FadePS", L"HLSL\\PostProcess.hlsl",
        device, "FadePS", "ps_5_0"));


    // ����ͨ��
    EffectPassDesc passDesc;
    passDesc.nameVS = "FadeVS";
    passDesc.namePS = "FadePS";
    HR(pImpl->m_pEffectHelper->AddEffectPass("Fade", device, &passDesc));


    pImpl->m_pEffectHelper->SetSamplerStateByName("g_Sam", RenderStates::SSLinearWrap.Get());

    pImpl->m_pEffectHelper->SetDebugObjectName("PostProcessEffect");

    return true;
}

void PostProcessEffect::RenderScreenFade(
    ID3D11DeviceContext* deviceContext,
    ID3D11ShaderResourceView* input,
    ID3D11RenderTargetView* output,
    const D3D11_VIEWPORT& vp,
    float fadeAmount)
{
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->RSSetViewports(1, &vp);
    deviceContext->OMSetRenderTargets(1, &output, nullptr);
    pImpl->m_pEffectHelper->SetShaderResourceByName("g_Tex", input);
    auto pPass = pImpl->m_pEffectHelper->GetEffectPass("Fade");
    pPass->PSGetParamByName("fadeAmount")->SetFloat(fadeAmount);
    pPass->Apply(deviceContext);
    deviceContext->Draw(3, 0);

    // ���
    output = nullptr;
    input = nullptr;
    deviceContext->OMSetRenderTargets(0, &output, nullptr);
    deviceContext->PSSetShaderResources(pImpl->m_pEffectHelper->MapShaderResourceSlot("g_Tex"), 1, &input);
}



