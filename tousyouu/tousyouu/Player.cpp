#include "Player.h"
#include <imgui.h>
#include "Collision.h"
#include "PerlinNoise.h"

void Player::Update(float dt)
{
    if (m_isGodMode) 
    {
        SetMoveSpeed(150);
        Move(dt);
        return;
    }

    Move(dt);

    m_isGround = CheckGrounded();

    if (m_isGround && isJumpKeyPressed()) {
        Jump();
    }

    ApplyGravity(dt);


}

void Player::Move(float dt)
{
    

    ImGuiIO& io = ImGui::GetIO();


    if (m_isGodMode) {
        // 移動方向計算
        DirectX::XMVECTOR forward = m_Transform.GetForwardAxisXM();
        DirectX::XMVECTOR right = m_Transform.GetRightAxisXM();

        // 前後
        int moveForward = (ImGui::IsKeyDown(ImGuiKey_S) ? 1 : 0) - (ImGui::IsKeyDown(ImGuiKey_W) ? 1 : 0);
        // 左右
        int moveRight = (ImGui::IsKeyDown(ImGuiKey_A) ? 1 : 0) - (ImGui::IsKeyDown(ImGuiKey_D) ? 1 : 0);
        //上下
        int moveUp = (ImGui::IsKeyDown(ImGuiKey_Q) ? 1 : 0) - (ImGui::IsKeyDown(ImGuiKey_E) ? 1 : 0);

        // 方向
        DirectX::XMVECTOR moveDir = DirectX::XMVector3Normalize(DirectX::XMVectorAdd(DirectX::XMVectorScale(forward, (float)moveForward), DirectX::XMVectorScale(right, (float)moveRight)));

        moveDir = DirectX::XMVectorSetY(moveDir, (float)moveUp);

        // 次のフレームの位置を予兆
        DirectX::XMFLOAT3 dir;
        DirectX::XMStoreFloat3(&dir, moveDir);
        DirectX::XMFLOAT3 predictedPosition =
        {
            m_Transform.GetPosition().x + dir.x * m_MoveSpeed * dt,
            m_Transform.GetPosition().y + dir.y * m_MoveSpeed * dt,
            m_Transform.GetPosition().z + dir.z * m_MoveSpeed * dt
        };
        m_Transform.Translate(dir, m_MoveSpeed * dt);
  
        return;
    }

   // 移動方向計算
    DirectX::XMVECTOR forward = m_Transform.GetForwardAxisXM();
    DirectX::XMVECTOR right = m_Transform.GetRightAxisXM();

    // 前後
    int moveForward = (ImGui::IsKeyDown(ImGuiKey_S) ? 1 : 0) - (ImGui::IsKeyDown(ImGuiKey_W) ? 1 : 0);
    // 左右
    int moveRight = (ImGui::IsKeyDown(ImGuiKey_A) ? 1 : 0) - (ImGui::IsKeyDown(ImGuiKey_D) ? 1 : 0);



    // 方向
    DirectX::XMVECTOR moveDir = DirectX::XMVector3Normalize(DirectX::XMVectorAdd(DirectX::XMVectorScale(forward, (float)moveForward), DirectX::XMVectorScale(right, (float)moveRight)));

    // 次のフレームの位置を予兆
    DirectX::XMFLOAT3 dir;
    DirectX::XMStoreFloat3(&dir, moveDir);
    DirectX::XMFLOAT3 predictedPosition = 
    {
        m_Transform.GetPosition().x + dir.x * m_MoveSpeed * dt,
        m_Transform.GetPosition().y + dir.y * m_MoveSpeed * dt,
        m_Transform.GetPosition().z + dir.z * m_MoveSpeed * dt
    };

    if (!IsCollision(predictedPosition)) 
    {
        m_Transform.Translate(dir, m_MoveSpeed * dt);
    }

    // 座標の更新

}

void Player::SetMoveSpeed(float speed)
{
    m_MoveSpeed = speed;
}

void Player::ApplyGravity(float dt)
{
    // 重力の影響
    if(!m_isGround)
    {
        m_Velocity.y += m_Gravity * dt;
    }
    else
    {
        // 地面と接触
        m_Velocity.y = 0;
    }

  
    float displacement = m_Velocity.y * dt;
    // 応用
    DirectX::XMFLOAT3 gravityDirection = { 0, -1, 0 }; // 下向き
    m_Transform.Translate(gravityDirection, displacement);
}

void Player::Jump()
{
    m_Velocity.y = -JumpForce;
    m_isGround = false;
}

XMFLOAT2 Player::GetPosition2D()
{
    return XMFLOAT2{ GetTransform().GetPosition().x,GetTransform().GetPosition().z };
}

XMFLOAT2 Player::GetScalePosition2D()
{
    return XMFLOAT2{ GetTransform().GetPosition().x / MAP_SCALE, GetTransform().GetPosition().z / MAP_SCALE };
}



bool Player::isJumpKeyPressed()
{
    return ImGui::IsKeyDown(ImGuiKey_Space);
}

void Player::GetChunkOfPlayer(Chunk* chunk)
{
    m_chunk = chunk;
}

bool Player::IsCollision(const DirectX::XMFLOAT3& position)
{
    BoundingBox predictedBox = GetBoundingBoxAtPosition(position);
    if (m_chunk != nullptr) {
        for (const auto& block : m_chunk->m_BlockCollision) {
            if (block.Intersects(predictedBox)) {
                return true; // 当たり
            }
        }
    }
    return false; // 当たってない
}

bool Player::CheckGrounded()
{
    // プレーヤーの場所を取得
    DirectX::XMFLOAT3 footPosition = m_Transform.GetPosition();
    footPosition.y -= 1.86f*m_Transform.GetScale().y / 2; // 1.86fはプレーヤーモデルの高

    // 当たってボックスをプレーヤーの足を生成
    DirectX::BoundingBox footBox(footPosition, DirectX::XMFLOAT3(0.5f, 0.1f, 0.5f)); // サイズ

    if (m_chunk != nullptr) {
        for (const auto& block : m_chunk->m_BlockCollision) {
            if (block.Intersects(footBox)) {
                return true; // 当たり
            }
        }
    }
    return false;
}

