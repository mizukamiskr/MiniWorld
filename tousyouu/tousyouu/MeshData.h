
#pragma once

#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <wrl/client.h>
#include <vector>
#include <DirectXCollision.h>

struct ID3D11Buffer;

struct MeshData
{

    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    ComPtr<ID3D11Buffer> m_pVertices; //頂点
    ComPtr<ID3D11Buffer> m_pNormals; //法線
    std::vector<ComPtr<ID3D11Buffer>> m_pTexcoordArrays; //UV座標
    ComPtr<ID3D11Buffer> m_pFaceID;  //面
    ComPtr<ID3D11Buffer> m_pTangents; //接線
    ComPtr<ID3D11Buffer> m_pBitangents; //
    ComPtr<ID3D11Buffer> m_pColors; //色

    ComPtr<ID3D11Buffer> m_pIndices; //Index(索引)
    uint32_t m_VertexCount = 0;
    uint32_t m_IndexCount = 0;
    uint32_t m_MaterialIndex = 0;
    uint32_t m_FaceID = 0;

    DirectX::BoundingBox m_BoundingBox;
    bool m_InFrustum = true;
};





#endif
