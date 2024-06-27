#pragma once
#include "GameObject.h"
#include "Chunk.h"

#define Power (10)

class Player :
    public GameObject
{
public:
    //debug(自由移動)
    bool m_isGodMode = false;

    //Update
    void Update(float dt)override;
    //移動処理
    void Move(float dt);
    //移動速度設置
    void SetMoveSpeed(float speed);
    //重力
    void ApplyGravity(float dt);
    //ジャンプ
    void Jump();

    //プレーヤー座標を返す
    XMFLOAT2 GetPosition2D();
    //マップサイズによって縮小したプレーヤー座標を返す
    XMFLOAT2 GetScalePosition2D();

    //Rayとブロックの衝突判定
    bool CheckGroundedWithRaycast();

    //ジャンプフラグ
    bool isJumpKeyPressed();

    //今プレーヤーを立っていますのChunkを指定する（衝突判定軽くのため）
    void GetChunkOfPlayer(Chunk* chunk);;

    //衝突判定
    bool IsCollision(const DirectX::XMFLOAT3& position);

    //着地判定
    bool CheckGrounded();

private:
    //移動速度
    float m_MoveSpeed;

    //画面移動速度
    float m_MouseSensitivityX = 0.005f;
    float m_MouseSensitivityY = 0.005f;


    float m_Gravity = 9.8f*Power; // 重力加速度
    DirectX::XMFLOAT3 m_Velocity = { 0,0,0 };  //今の速度

    bool m_isGround = true;
    float JumpForce = 5.0f*Power;

    Chunk* m_chunk;
};

