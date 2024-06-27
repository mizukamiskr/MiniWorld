#pragma once

#include "Player.h"
#include "Effects.h"
#include "ModelManager.h"
#include "PerlinNoise.h"
#include "Collision.h"

//Chunk������륯�饹
//Chunk�Υ�`�ɡ��ǥ�`��
class ChunkManager
{
public:

    ChunkManager();
    ChunkManager(int _loadRadius, int _bufferZone);
    ~ChunkManager();

    //�֥�å��ȥ��ꥸ����m�ù���γ��ڻ�
    void init(ModelManager& manager,Player& player);
    //Update�������nͻ�ж���I����
    void Update(Player& player);
    //Chunk���`��
    void LoadChunk(const XMFLOAT2& position);
    //Chunk��ǥ�`��
    void UnloadChunk(const XMFLOAT2& position);

    //�ץ�`��`�λؤ��ָ�����줿�����Chunk�����ɤ���
    void InitChunksAroundPlayer(const XMFLOAT2& playerPosition);
    //Chunk�Υ֥�å��Υǩ`����vector��װ�Ť���
    void LoadBlock(Chunk& chunk);

    //�֥�å����ɣ��ߤ��ʤɤ��v��룩
    void GenerateBlock(Chunk& chunk);
    //ָ�����줿�����ȫ�ƤΥ֥�å��������󥰜ʂ�򤷤ޤ�
    void LoadAllChunk();
    //ȫ�Ƥ�Chunk��ǥ�`��
    void DeleteAllChunk();

    //Chunk����
    void GenerateChunk(Player& player);
    //�ץ�`��`�������äƤ�Chunk����¤��ޤ����nͻ�ж��Τ��ᣩ
    void UpdatePlayerChunk(Player& player);

    //Chunk�ڥ֥�å��Ή�Q����
    const std::vector<Transform>& GetBlockTransforms() const {
        return m_BlockTransform;
    }

    //Chunk�ڥ֥�å��Υ��󥹥��󥹥ǩ`������
    const std::vector<InstanceEffect::InstancedData>& GetBlockInstanceData() const {
        return m_BlockInstanceData;
    }

    //�����v�B
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
    //�Τ�Chunk��`�ɺ��ӳ����ι��졢���ι�������ƄӤ�����¤���Chunk����¤��ޤ�
    int m_LoadRadius;
    //������󥰤�Chunk���죨�ץ�`��`��Ҋ�������죩
    int m_BufferZone;

    //�֥�å��Υޥȥ�å����������Υƥ������㣩
    int m_MaterialIndex = 5;

    //�Υ��������
    float m_Amplitude = 1.0f;

    float m_Octaves = 1.0f;

    //�Υ������l��(x,y����e��)
    float m_FrequencyX = 1.0f;
    float m_FrequencyY = 1.0f;

    //����
    std::vector<Chunk> m_Chunk;
    std::vector<Transform> m_BlockTransform; 
    std::vector<InstanceEffect::InstancedData> m_BlockInstanceData; 

    //����ܥå���
    BoundingBox2D loadBox; 
    BoundingBox2D collisionBox;

    //��ǥ�
    Model* m_Block;

    void CalculateRequiredChunks(const Player& player);
    void LoadRequiredChunks();
    void UnloadExcessChunks();
};

