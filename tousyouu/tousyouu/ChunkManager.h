#pragma once

#include "Player.h"
#include "Effects.h"
#include "ModelManager.h"
#include "PerlinNoise.h"
#include "Collision.h"

//Chunkを管理するクラス
//Chunkのロード、デリート
class ChunkManager
{
public:

    ChunkManager();
    ChunkManager(int _loadRadius, int _bufferZone);
    ~ChunkManager();

    //ブロックとコリジョン適用範囲の初期化
    void init(ModelManager& manager,Player& player);
    //Update、主に衝突判定を処理する
    void Update(Player& player);
    //Chunkをロード
    void LoadChunk(const XMFLOAT2& position);
    //Chunkをデリート
    void UnloadChunk(const XMFLOAT2& position);

    //プレーヤーの回りに指定された範囲のChunkを生成する
    void InitChunksAroundPlayer(const XMFLOAT2& playerPosition);
    //Chunkのブロックのデータをvectorに装着する
    void LoadBlock(Chunk& chunk);

    //ブロック生成（高さなどに関わる）
    void GenerateBlock(Chunk& chunk);
    //指定された範囲の全てのブロックをレンダリング準備をします
    void LoadAllChunk();
    //全てのChunkをデリート
    void DeleteAllChunk();

    //Chunk生成
    void GenerateChunk(Player& player);
    //プレーヤーいま立ってるChunkを更新します（衝突判定のため）
    void UpdatePlayerChunk(Player& player);

    //Chunk内ブロックの変換配列
    const std::vector<Transform>& GetBlockTransforms() const {
        return m_BlockTransform;
    }

    //Chunk内ブロックのインスタンスデータ配列
    const std::vector<InstanceEffect::InstancedData>& GetBlockInstanceData() const {
        return m_BlockInstanceData;
    }

    //変数関連
    int GetLoadRadius() const { return m_LoadRadius; }
    void SetLoadRadius(int radius) { m_LoadRadius = radius; }

    int GetBufferZone() const { return m_BufferZone; }
    void SetBufferZone(int zone) { m_BufferZone = zone; }

    int GetMaterialIndex() const { return m_MaterialIndex; }
    void SetMaterialIndex(int material) { m_MaterialIndex = material; }

    float GetAmplitude() const { return m_Amplitude; }
    void SetAmplitude(float amplitude) { m_Amplitude = amplitude; }

    float GetFrequencyX() const { return m_FrequencyX; }
    void SetFrequencyX(float frequency_x) { m_FrequencyX = frequency_x; }

    float GetFrequencyY() const { return m_FrequencyY; }
    void SetFrequencyY(float frequency_y) { m_FrequencyY = frequency_y; }

    float GetOctaves() const { return m_Octaves; }
    void SetOctaves(float octaves) { m_Octaves = octaves; }

private:
    //次のChunkロード呼び出すの範囲、この範囲外に移動すれば新たなChunkを更新します
    int m_LoadRadius;
    //レンダリングのChunk範囲（プレーヤーが見えた範囲）
    int m_BufferZone;

    //ブロックのマトリックス（ただのテクスチャ）
    int m_MaterialIndex = 5;

    //ノイズの振幅
    float m_Amplitude = 1.0f;

    float m_Octaves = 1.0f;

    //ノイズの頻度(x,y方向別々)
    float m_FrequencyX = 1.0f;
    float m_FrequencyY = 1.0f;

    //配列
    std::vector<Chunk> m_Chunk;
    std::vector<Transform> m_BlockTransform; 
    std::vector<InstanceEffect::InstancedData> m_BlockInstanceData; 

    //範囲ボックス
    BoundingBox2D loadBox; 
    BoundingBox2D collisionBox;

    //モデル
    Model* m_Block;

    void CalculateRequiredChunks(const Player& player);
    void LoadRequiredChunks();
    void UnloadExcessChunks();
};

