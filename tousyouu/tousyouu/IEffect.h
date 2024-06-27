//***************************************************************************************
// IEffects.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// 特效基类定义
// Effect interface definitions.
//***************************************************************************************

#ifndef IEFFECT_H
#define IEFFECT_H

#include "WinMin.h"
#include <memory>
#include <vector>
#include <d3d11_1.h>
#include <DirectXMath.h>

class Material;
struct MeshData;

// 一つのMeshDataが入力アセンブリステージに設定されるべき内容
// レイアウト、strides、offsetsとプリミティブはeffect passを提供します
// ほかのデータはMeshDataから
struct MeshDataInput
{
    ID3D11InputLayout* pInputLayout = nullptr; // 入力レイアウト
    D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED; // プリミティブトポロジー
    std::vector<ID3D11Buffer*> pVertexBuffers; // 頂点バッファ
    ID3D11Buffer* pIndexBuffer = nullptr; // インデックスバッファ
    std::vector<uint32_t> strides; // ストライド
    std::vector<uint32_t> offsets; // オフセット
    uint32_t indexCount = 0; // インデックスの数
};

class IEffect
{
public:
    IEffect() = default; // コンストラクタ
    virtual ~IEffect() = default; // デストラクタ
    // コピー不可、ムーブ可
    IEffect(const IEffect&) = delete; // コピーコンストラクタ削除
    IEffect& operator=(const IEffect&) = delete; // 代入演算子削除
    IEffect(IEffect&&) = default; // ムーブコンストラクタ
    IEffect& operator=(IEffect&&) = default; // ムーブ代入演算子

    // 定数バッファを更新してバインドする
    virtual void Apply(ID3D11DeviceContext* deviceContext) = 0;
};

class IEffectTransform
{
public:
    virtual void XM_CALLCONV SetWorldMatrix(DirectX::FXMMATRIX W) = 0; // ワールド行列を設定
    virtual void XM_CALLCONV SetViewMatrix(DirectX::FXMMATRIX V) = 0; // ビュー行列を設定
    virtual void XM_CALLCONV SetProjMatrix(DirectX::FXMMATRIX P) = 0; // プロジェクション行列を設定
};

class IEffectMaterial
{
public:
    virtual void SetMaterial(const Material& material) = 0; // マテリアルを設定
};

class IEffectMeshData
{
public:
    virtual MeshDataInput GetInputData(const MeshData& meshData) = 0; // MeshDataから入力データを取得
};


#endif
