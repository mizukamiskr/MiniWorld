
#pragma once

#ifndef COLLISION_H
#define COLLISION_H

#include <DirectXCollision.h>
#include <vector>
#include "Vertex.h"
#include "Camera.h"


struct Ray
{
	Ray();
	Ray(const DirectX::XMFLOAT3& origin, const DirectX::XMFLOAT3& direction);

	static Ray ScreenToRay(const Camera& camera, float screenX, float screenY);

	bool Hit(const DirectX::BoundingBox& box, float* pOutDist = nullptr, float maxDist = FLT_MAX);
	bool Hit(const DirectX::BoundingOrientedBox& box, float* pOutDist = nullptr, float maxDist = FLT_MAX);
	bool Hit(const DirectX::BoundingSphere& sphere, float* pOutDist = nullptr, float maxDist = FLT_MAX);
	bool XM_CALLCONV Hit(DirectX::FXMVECTOR V0, DirectX::FXMVECTOR V1, DirectX::FXMVECTOR V2, float* pOutDist = nullptr, float maxDist = FLT_MAX);

	DirectX::XMFLOAT3 origin;		// rayのスタートポイント
	DirectX::XMFLOAT3 direction;	// 単位方向ベクトル
};


class Collision
{
public:

	// 线框顶点/索引数组
	struct WireFrameData
	{
		std::vector<VertexPosColor> vertexVec;		// 顶点数组
		std::vector<uint32_t> indexVec;				// 索引数组
	};

	//
	// 包围盒线框的创建
	//

	// 创建AABB盒线框
	static WireFrameData CreateBoundingBox(const DirectX::BoundingBox& box, const DirectX::XMFLOAT4& color);
	// 创建OBB盒线框
	static WireFrameData CreateBoundingOrientedBox(const DirectX::BoundingOrientedBox& box, const DirectX::XMFLOAT4& color);
	// 创建包围球线框
	static WireFrameData CreateBoundingSphere(const DirectX::BoundingSphere& sphere, const DirectX::XMFLOAT4& color, int slices = 20);
	// 创建视锥体线框
	static WireFrameData CreateBoundingFrustum(const DirectX::BoundingFrustum& frustum, const DirectX::XMFLOAT4& color);

	// 视锥体裁剪
	static std::vector<Transform> XM_CALLCONV FrustumCulling(
		const std::vector<Transform>& transforms, const DirectX::BoundingBox& localBox, DirectX::FXMMATRIX View, DirectX::CXMMATRIX Proj);
	
    // 视锥体裁剪
    static void XM_CALLCONV FrustumCulling(
        std::vector<Transform>& dest, const std::vector<Transform>& src, 
        const DirectX::BoundingBox& localBox, DirectX::FXMMATRIX View, DirectX::CXMMATRIX Proj);

	void Draw(ID3D11DeviceContext* devicecontext);

private:
	static WireFrameData CreateFromCorners(const DirectX::XMFLOAT3(&corners)[8], const DirectX::XMFLOAT4& color);
};


//2D AABBボックス
struct BoundingBox2D {
	float left;
	float top;
	float right;
	float bottom;

	// ポイントと判定
	bool Contains(float x, float y) {
		return (x >= left && x <= right && y >= bottom && y <= top);
	}

};


#endif
