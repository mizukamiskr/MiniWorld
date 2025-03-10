#pragma once

#include "Player.h"
#include "Effects.h"
#include "ModelManager.h"
#include "PerlinNoise.h"
#include "Collision.h"

//Chunkを砿尖するクラス
//Chunkのロ�`ド、デリ�`ト
class ChunkManager
{
public:

    ChunkManager();
    ChunkManager(int _loadRadius, int _bufferZone);
    ~ChunkManager();

    //ブロックとコリジョン�m喘���譴粒�豚晒
    void init(ModelManager& manager,Player& player);
    //Update、麼に�n融登協を�I尖する
    void Update(Player& player);
    //Chunkをロ�`ド
    void LoadChunk(const XMFLOAT2& position);
    //Chunkをデリ�`ト
    void UnloadChunk(const XMFLOAT2& position);

    //プレ�`ヤ�`の指りに峺協された���譴�Chunkを伏撹する
    void InitChunksAroundPlayer(const XMFLOAT2& playerPosition);
    //Chunkのブロックのデ�`タをvectorに廾彭する
    void LoadBlock(Chunk& chunk);

    //ブロック伏撹�┯澆気覆匹哩vわる��
    void GenerateBlock(Chunk& chunk);
    //峺協された���譴糧�てのブロックをレンダリング���笋鬚靴泙�
    void LoadAllChunk();
    //畠てのChunkをデリ�`ト
    void DeleteAllChunk();

    //Chunk伏撹
    void GenerateChunk(Player& player);
    //プレ�`ヤ�`いま羨ってるChunkを厚仟します���n融登協のため��
    void UpdatePlayerChunk(Player& player);

    //Chunk坪ブロックの���Q塘双
    const std::vector<Transform>& GetBlockTransforms() const {
        return m_BlockTransform;
    }

    //Chunk坪ブロックのインスタンスデ�`タ塘双
    const std::vector<InstanceEffect::InstancedData>& GetBlockInstanceData() const {
        return m_BlockInstanceData;
    }

    //�篳��v�B
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
    //肝のChunkロ�`ド柵び竃すの���譟△海旅��賤發牧��咾垢譴佩造燭�Chunkを厚仟します
    int m_LoadRadius;
    //レンダリングのChunk���譯┘廛讒`ヤ�`が��えた���譯�
    int m_BufferZone;

    //ブロックのマトリックス�┐燭世離謄�スチャ��
    int m_MaterialIndex = 5;

    //ノイズの尅嫌
    float m_Amplitude = 1.0f;

    float m_Octaves = 1.0f;

    //ノイズの�l業(x,y圭���e？)
    float m_FrequencyX = 1.0f;
    float m_FrequencyY = 1.0f;

    //塘双
    std::vector<Chunk> m_Chunk;
    std::vector<Transform> m_BlockTransform; 
    std::vector<InstanceEffect::InstancedData> m_BlockInstanceData; 

    //���譽椒奪�ス
    BoundingBox2D loadBox; 
    BoundingBox2D collisionBox;

    //モデル
    Model* m_Block;

    void CalculateRequiredChunks(const Player& player);
    void LoadRequiredChunks();
    void UnloadExcessChunks();
};

