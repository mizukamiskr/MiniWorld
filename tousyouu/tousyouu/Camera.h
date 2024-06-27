//***************************************************************************************
// Camera.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// 提供第一人称(自由视角)和第三人称摄像机
// Provide 1st person(free view) and 3rd person cameras.
//***************************************************************************************

#pragma once

#ifndef CAMERA_H
#define CAMERA_H

#include "WinMin.h"
#include <d3d11_1.h>
#include <DirectXMath.h>
#include "Transform.h"
#include "Player.h"

enum CameraMode {
    FIRST,
    THIRD,
    FREE,
};

class Camera
{
public:
    Camera() = default;
    virtual ~Camera() = 0;

    //
    // カメラの座標を取る
    //

    DirectX::XMVECTOR GetPositionXM() const;
    DirectX::XMFLOAT3 GetPosition() const;

    //
    // 回転を取る
    //

    // X軸のラジアンを取る
    float GetRotationX() const;
    // Y軸のラジアンを取る
    float GetRotationY() const;

    //
    // カメラの座標軸ベクトルを取る
    //

    DirectX::XMVECTOR GetRightAxisXM() const;
    DirectX::XMFLOAT3 GetRightAxis() const;
    DirectX::XMVECTOR GetUpAxisXM() const;
    DirectX::XMFLOAT3 GetUpAxis() const;
    DirectX::XMVECTOR GetLookAxisXM() const;
    DirectX::XMFLOAT3 GetLookAxis() const;

    //
    // マトリックスを取る
    //

    DirectX::XMMATRIX GetLocalToWorldMatrixXM() const;
    DirectX::XMMATRIX GetViewMatrixXM() const;
    DirectX::XMMATRIX GetProjMatrixXM(bool reversedZ = false) const;
    DirectX::XMMATRIX GetViewProjMatrixXM(bool reversedZ = false) const;

    // ビューポートを取る
    D3D11_VIEWPORT GetViewPort() const;

    float GetNearZ() const;
    float GetFarZ() const;
    float GetFovY() const;
    float GetAspectRatio() const;

    // ビューフラスタム設置
    void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

    // ビューポート設置
    void SetViewPort(const D3D11_VIEWPORT& viewPort);
    void SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);

protected:

    // カメラ変換
    Transform m_Transform = {};

    // ビューフラスタムのステータス
    float m_NearZ = 0.0f;
    float m_FarZ = 0.0f;
    float m_Aspect = 0.0f;
    float m_FovY = 0.0f;

    // 今のビューポート
    D3D11_VIEWPORT m_ViewPort = {};

};

class FirstPersonCamera : public Camera
{
public:
    FirstPersonCamera() = default;
    ~FirstPersonCamera() override;

    // カメラ座標を設置
    void SetPosition(float x, float y, float z);
    void SetPosition(const DirectX::XMFLOAT3& pos);
    // カメラの向こうを設置
    void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up);
    void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);
    // 平行移動
    void Strafe(float d);
    // 前に進む（平面）
    void Walk(float d);
    // 前に進む
    void MoveForward(float d);
    // 移動
    void Translate(const DirectX::XMFLOAT3& dir, float magnitude);
    // 上下
    // radは+の場合上に観測
    // radは-の場合下に観測
    void Pitch(float rad);
    // 左右
    // radは+の場合右に観測
    // radは-の場合左に観測
    void RotateY(float rad);

    void SetCharacter(Player player);
};

class ThirdPersonCamera : public Camera
{
public:
    ThirdPersonCamera() = default;
    ~ThirdPersonCamera() override;

    // 追跡オブジェクトの座標を取る
    DirectX::XMFLOAT3 GetTargetPosition() const;
    // 追跡オブジェクト間の距離を取る
    float GetDistance() const;
    // 追跡オブジェクトを中心として回転
    void RotateX(float rad);
    // 水平方向回転
    void RotateY(float rad);
    // 近くにする
    void Approach(float dist);
    // 初期のX軸ラジアン設置(ラジアン範囲[0, pi/3])
    void SetRotationX(float rad);
    // 初期のY軸ラジアン設置
    void SetRotationY(float rad);
    // 追跡オブジェクトを設置
    void SetTarget(const DirectX::XMFLOAT3& target);
    // 初期の距離を設置
    void SetDistance(float dist);
    // オブジェクトとの最大最小距離を設置
    void SetDistanceMinMax(float minDist, float maxDist);

private:
    DirectX::XMFLOAT3 m_Target = {};
    float m_Distance = 0.0f;
    // 最小距離、最大距離
    float m_MinDist = 0.0f, m_MaxDist = 0.0f;
};


#endif
