#pragma once

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Geometry.h"
#include "Material.h"
#include "MeshData.h"
#include "Transform.h"
#include "IEffect.h"

struct Model;

class GameObject
{
public:
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;


    GameObject() = default;
    ~GameObject() = default;

    GameObject(const GameObject&) = default;
    GameObject& operator=(const GameObject&) = default;

    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    // オブジェクト变换を取得する
    Transform& GetTransform();
    // オブジェクト変換を取得する
    const Transform& GetTransform() const;

    //
    // 衝突検出
    //
    void FrustumCulling(const DirectX::BoundingFrustum& frustumInWorld);
    void CubeCulling(const DirectX::BoundingOrientedBox& obbInWorld);
    void CubeCulling(const DirectX::BoundingBox& aabbInWorld);
    bool InFrustum() const { return m_InFrustum; }

    //
    // モデル
    //
    void SetModel(const Model* pModel);
    const Model* GetModel() const;

    //当たり判定
    DirectX::BoundingBox GetLocalBoundingBox() const;
    DirectX::BoundingBox GetLocalBoundingBox(size_t idx) const;
    DirectX::BoundingBox GetBoundingBox() const;
    DirectX::BoundingBox GetBoundingBox(size_t idx) const;
    DirectX::BoundingBox GetBoundingBoxAtPosition(const DirectX::XMFLOAT3& position) const;
    DirectX::BoundingOrientedBox GetBoundingOrientedBox() const;
    DirectX::BoundingOrientedBox GetBoundingOrientedBox(size_t idx) const;
    //
    // 描画
    //

    void SetVisible(bool visible) {
        m_InFrustum = visible;
        m_SubModelInFrustum.assign(m_SubModelInFrustum.size(), true);
    }

    // オブジェクト描画
    void Draw(ID3D11DeviceContext* deviceContext, IEffect& effect);
    virtual void Update(float dt);

protected:
    const Model* m_pModel = nullptr;
    std::vector<bool> m_SubModelInFrustum;
    Transform m_Transform = {};
    bool m_InFrustum = true;
};




#endif
