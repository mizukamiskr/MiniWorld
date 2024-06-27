//***************************************************************************************
// Effects.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// 简易特效管理框架
// Simple effect management framework.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include "GameObject.h"

#include "IEffect.h"
#include "Material.h"
#include "MeshData.h"
#include "Buffer.h"

class ForwardEffect : public IEffect, public IEffectTransform,
    public IEffectMaterial, public IEffectMeshData
{
public:
    ForwardEffect();
    virtual ~ForwardEffect() override;

    ForwardEffect(ForwardEffect&& moveFrom) noexcept;
    ForwardEffect& operator=(ForwardEffect&& moveFrom) noexcept;

    // シングルトンを取得する
    static ForwardEffect& Get();

    // 初期化
    bool InitAll(ID3D11Device* device);

    //
    // IEffectTransform
    //

    void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) override;
    void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) override;
    void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) override;

    //
    // IEffectMaterial
    //

    void SetMaterial(const Material& material) override;

    //
    // IEffectMeshData
    //

    MeshDataInput GetInputData(const MeshData& meshData) override;


    //
    // ForwardEffect
    //

    void SetCascadeLevels(int cascadeLevels);
    void SetPCFDerivativesOffsetEnabled(bool enable);
    void SetCascadeBlendEnabled(bool enable);
    void SetCascadeIntervalSelectionEnabled(bool enable);

    void SetCascadeVisulization(bool enable);
    void SetCascadeOffsets(const DirectX::XMFLOAT4 offsets[8]);
    void SetCascadeScales(const DirectX::XMFLOAT4 scales[8]);
    void SetCascadeFrustumsEyeSpaceDepths(const float depths[8]);
    void SetCascadeBlendArea(float blendArea);

    void SetPCFKernelSize(int size);
    void SetPCFDepthOffset(float bias);

    void SetShadowSize(int size);
    void XM_CALLCONV SetShadowViewMatrix(DirectX::FXMMATRIX ShadowView);
    void SetShadowTextureArray(ID3D11ShaderResourceView* shadow);

    void SetLightDir(const DirectX::XMFLOAT3& dir);


    // デフォルト
    void SetRenderDefault(bool reversedZ = false);
    // Pre-Zパスで描画
    void SetRenderPreZPass(bool reversedZ = false);


    // 定数バッファとテクスチャリソースの変更を適用
    void Apply(ID3D11DeviceContext* deviceContext) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

class ShadowEffect : public IEffect, public IEffectTransform,
    public IEffectMaterial, public IEffectMeshData
{
public:
    ShadowEffect();
    virtual ~ShadowEffect() override;

    ShadowEffect(ShadowEffect&& moveFrom) noexcept;
    ShadowEffect& operator=(ShadowEffect&& moveFrom) noexcept;

    // シングルトンを取得する
    static ShadowEffect& Get();

    // 初期化
    bool InitAll(ID3D11Device* device);

    //
    // IEffectTransform
    //

    void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) override;
    void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) override;
    void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) override;

    //
    // IEffectMaterial
    //

    void SetMaterial(const Material& material) override;

    //
    // IEffectMeshData
    //

    MeshDataInput GetInputData(const MeshData& meshData) override;

    //
    // ShadowEffect
    //

    // デフォルト
    void SetRenderDefault();

    // アルファクリッピングで描画（透明度のあるオブジェクトを扱う）
    void SetRenderAlphaClip(float alphaClipValue);

    // テクスチャに深度マップを描画
    void RenderDepthToTexture(ID3D11DeviceContext* deviceContext,
        ID3D11ShaderResourceView* input,
        ID3D11RenderTargetView* output,
        const D3D11_VIEWPORT& vp);

    // 定数バッファとテクスチャリソースの変更を適用
    void Apply(ID3D11DeviceContext* deviceContext) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

class SkyboxEffect : public IEffect, public IEffectTransform,
    public IEffectMeshData, public IEffectMaterial
{
public:
    SkyboxEffect();
    virtual ~SkyboxEffect() override;

    SkyboxEffect(SkyboxEffect&& moveFrom) noexcept;
    SkyboxEffect& operator=(SkyboxEffect&& moveFrom) noexcept;

    // シングルトンを取得する
    static SkyboxEffect& Get();

    // 初期化
    bool InitAll(ID3D11Device* device);


    //
    // IEffectTransform
    //

    // 用がない
    void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) override;

    void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) override;
    void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) override;

    //
    // IEffectMaterial
    //

    void SetMaterial(const Material& material) override;

    //
    // IEffectMeshData
    //

    MeshDataInput GetInputData(const MeshData& meshData) override;

    // 
    // SkyboxEffect
    //

    // デフォルト
    void SetRenderDefault();

    // 深度テクスチャを設置
    void SetDepthTexture(ID3D11ShaderResourceView* depthTexture);
    // シーンレンダリングテクスチャを設定する
    void SetLitTexture(ID3D11ShaderResourceView* litTexture);

    // MSAAレベルを設置
    void SetMsaaSamples(UINT msaaSamples);

    //
    // IEffect
    //

    // 定数バッファとテクスチャリソースの変更を適用
    void Apply(ID3D11DeviceContext* deviceContext) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};



//インスタンス生成
class InstanceEffect : public IEffect, public IEffectTransform,
    public IEffectMaterial, public IEffectMeshData
{
public:
    struct InstancedData
    {
        DirectX::XMFLOAT4X4 world;
        DirectX::XMFLOAT4X4 worldInvTranspose;
        DirectX::XMFLOAT4 color;
        uint32_t index;               //テクスチャのインデックス
    };

public:
    InstanceEffect();
    virtual ~InstanceEffect() override;

    InstanceEffect(InstanceEffect&& moveFrom) noexcept;
    InstanceEffect& operator=(InstanceEffect&& moveFrom) noexcept;

    // シングルトンを取得する
    static InstanceEffect& Get();

    // 初期化
    bool InitAll(ID3D11Device* device);

    //
    // IEffectTransform
    //

    void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) override;
    void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) override;
    void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) override;

    //
    // IEffectMaterial
    //

    void SetMaterial(const Material& material) override;
    void SetTextureArray(ID3D11ShaderResourceView* textures);

    //
    // IEffectMeshData
    //

    MeshDataInput GetInputData(const MeshData& meshData) override;


    //
    // InstanceEffect
    //

    void SetLightDir(const DirectX::XMFLOAT3& dir);

    void SetEyePos(const DirectX::XMFLOAT3& eyePos);
    void SetDiffuseColor(const DirectX::XMFLOAT4& color);

    // レンダリングモードはデフォルトモードに設置
    void SetRenderDefault();

    //collisionを描画する
    void SetRenderCollision();

    // インスタンス描画
    void DrawInstanced(ID3D11DeviceContext* deviceContext, Buffer& buffer, const GameObject& object, uint32_t numObjects);


    // 定数バッファとテクスチャリソースの変更を適用
    void Apply(ID3D11DeviceContext* deviceContext) override;

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

class PostProcessEffect
{
public:
    PostProcessEffect();
    ~PostProcessEffect();

    PostProcessEffect(PostProcessEffect&& moveFrom) noexcept;
    PostProcessEffect& operator=(PostProcessEffect&& moveFrom) noexcept;

    // シングルトンを取得する
    static PostProcessEffect& Get();

    bool InitAll(ID3D11Device* device);

    // fade-in fade-out
    void RenderScreenFade(
        ID3D11DeviceContext* deviceContext,
        ID3D11ShaderResourceView* input,
        ID3D11RenderTargetView* output,
        const D3D11_VIEWPORT& vp,
        float fadeAmount);

    void SetEyePos(const DirectX::XMFLOAT3& eyePos);

private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};


#endif

