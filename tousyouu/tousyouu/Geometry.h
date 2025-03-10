﻿
#pragma once

#include <vector>
#include <string>
#include <map>
#include <DirectXMath.h>
#include <functional>

struct GeometryData
{
    std::vector<DirectX::XMFLOAT3> vertices;
    std::vector<DirectX::XMFLOAT3> normals;
    std::vector<DirectX::XMFLOAT2> texcoords;
    std::vector<DirectX::XMFLOAT4> tangents;
    std::vector<int> faceIDs;  // 面
    std::vector<uint32_t> indices32;
    std::vector<uint16_t> indices16;
};

namespace Geometry
{
    // 球体メッシュデータを生成します。levelsとslicesが大きいほど、精度が高くなります。
    GeometryData CreateSphere(float radius = 1.0f, uint32_t levels = 20, uint32_t slices = 20);

    // 立方体メッシュデータを生成します。
    GeometryData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f);

    // 円柱メッシュデータを生成します。slicesが大きいほど、精度が高くなります。
    GeometryData CreateCylinder(float radius = 1.0f, float height = 2.0f, uint32_t slices = 20, uint32_t stacks = 10, float texU = 1.0f, float texV = 1.0f);

    // 円錐メッシュデータを生成します。slicesが大きいほど、精度が高くなります。
    GeometryData CreateCone(float radius = 1.0f, float height = 2.0f, uint32_t slices = 20);

    // 平面メッシュデータを生成します。
    GeometryData CreatePlane(const DirectX::XMFLOAT2& planeSize, const DirectX::XMFLOAT2& maxTexCoord = { 1.0f, 1.0f });
    GeometryData CreatePlane(float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f);

    // グリッドメッシュデータを生成します。
    GeometryData CreateGrid(const DirectX::XMFLOAT2& gridSize, const DirectX::XMUINT2& slices, const DirectX::XMFLOAT2& maxTexCoord,
        const std::function<float(float, float)>& heightFunc = [](float x, float z) { return 0.0f; },
        const std::function<DirectX::XMFLOAT3(float, float)>& normalFunc = [](float x, float z) { return DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); },
        const std::function<DirectX::XMFLOAT4(float, float)>& colorFunc = [](float x, float z) { return DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); });

}






