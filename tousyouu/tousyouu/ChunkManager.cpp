#include "ChunkManager.h"


ChunkManager::ChunkManager()
{
    m_LoadRadius = 1;
    m_BufferZone = 3;
}

ChunkManager::ChunkManager(int _loadRadius, int _bufferZone)
{
    m_LoadRadius = _loadRadius;
    m_BufferZone = _bufferZone;
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::init(ModelManager& manager, Player& player)
{
    m_Chunk.reserve(36);
    // �ƥ��ȥޥåץ��åץ�`��
    m_Block= manager.CreateFromGeometry("Cubes", Geometry::CreateBox(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
    m_Block->SetDebugObjectName("ChunkBlock");

    XMFLOAT2 playerPosition = player.GetPosition2D();
    XMFLOAT2 CenterPosition = Ffloor(playerPosition);
    collisionBox = { ((CenterPosition.x - 0.5f) * MAP_SCALE)
        ,((CenterPosition.y + 0.5f) * MAP_SCALE)
        ,((CenterPosition.x + 0.5f) * MAP_SCALE)
        ,((CenterPosition.y - 0.5f) * MAP_SCALE) };

}

void ChunkManager::Update(Player& player)
{
    //�ץ�`��`�����Ĥ��¤���Chunk�����ɤ��ޤ�
    if (!loadBox.Contains(player.GetPosition2D().x,player.GetPosition2D().y))
    {
        GenerateChunk(player);
    }

    //�nͻ�ж��������
    if (!collisionBox.Contains(player.GetPosition2D().x, player.GetPosition2D().y)) 
    {
        XMFLOAT2 playerPosition = player.GetScalePosition2D();

        XMFLOAT2 CenterPosition = ChunkFfloor(playerPosition);

        collisionBox = { ((CenterPosition.x - 0.5f) * MAP_SCALE)
        ,((CenterPosition.y + 0.5f) * MAP_SCALE)
        ,((CenterPosition.x + 0.5f) * MAP_SCALE)
        ,((CenterPosition.y - 0.5f) * MAP_SCALE) };

        UpdatePlayerChunk(player);
    }
}

void ChunkManager::LoadChunk(const XMFLOAT2& position)
{

}

void ChunkManager::UnloadChunk(const XMFLOAT2& position)
{
}

void ChunkManager::InitChunksAroundPlayer(const XMFLOAT2& playerPosition)
{
    //Chunk�Ȍ��ꤹ�뤿�ᡢ�ץ쥤��`�����ˤ�sС����
    XMFLOAT2 scaledPlayerPosition = { playerPosition.x / MAP_SCALE,
        playerPosition.y / MAP_SCALE };

    XMFLOAT2 centerPosition = ChunkFfloor(scaledPlayerPosition);
 
    Chunk p_Chunk;

    for (int i = -m_BufferZone; i <= m_BufferZone; ++i) {
        for (int j = -m_BufferZone; j <= m_BufferZone; ++j) {
            p_Chunk.SetCenter(XMFLOAT2(centerPosition.x + i, centerPosition.y + j));
            GenerateBlock(p_Chunk);
            m_Chunk.push_back(p_Chunk);
        }
    }


    //Chunk���¤Τ���ˤι���
    loadBox = { ((scaledPlayerPosition.x - m_LoadRadius) * MAP_SCALE)
        ,((scaledPlayerPosition.y + m_LoadRadius) * MAP_SCALE)
        ,((scaledPlayerPosition.x + m_LoadRadius) * MAP_SCALE)
        ,((scaledPlayerPosition.y - m_LoadRadius) * MAP_SCALE) };
}

void ChunkManager::LoadBlock(Chunk& chunk)
{
    Transform transform;
    InstanceEffect::InstancedData data;
    chunk.m_BlockCollision.reserve(MAX_BLOCK_NUM);

    for (int i = 0; i < MAX_BLOCK_WIDTH; ++i) {
        for (int j = 0; j < MAX_BLOCK_WIDTH; ++j) {
            for (int y = 0; y < chunk.m_BlockHeight[i][j]; ++y) {
                transform.SetPosition(XMFLOAT3(chunk.m_BlockCenter[i][j].x * MAP_SCALE, y * BLOCK_SIZE, chunk.m_BlockCenter[i][j].y * MAP_SCALE));
                //ͨ���軭���ä������ˉ�Q����
                m_BlockTransform.push_back(transform);
                if (y >= chunk.m_BlockHeight[i][j] - 1) {
                    data.index =m_MaterialIndex;
                }
                else {
                    data.index = 0;
                }
                XMMATRIX W = transform.GetLocalToWorldMatrixXM();
                XMMATRIX WInvT = XMath::InverseTranspose(W);
                XMStoreFloat4x4(&data.world, XMMatrixTranspose(W));
                XMStoreFloat4x4(&data.worldInvTranspose, XMMatrixTranspose(WInvT));
                m_BlockInstanceData.push_back(data);

                BoundingBox transformedBox;
                m_Block->boundingbox.Transform(transformedBox, transform.GetLocalToWorldMatrixXM());

                chunk.m_BlockCollision.push_back(transformedBox);

            }
        }
    }
}

//void ChunkManager::GenerateBlock(Chunk& chunk)
//{
//    float blockSizeX = 1.0f / MAX_BLOCK_WIDTH;
//    float blockSizeY = 1.0f / MAX_BLOCK_WIDTH;
//
//    XMFLOAT2 currChunkCenter = chunk.GetCenter();
//
//    for (int i = 0; i < MAX_BLOCK_WIDTH; ++i) {
//        // ���� currX��Chunk���Ĥ����X�S�Ƅ���
//        float currX = (i - MAX_BLOCK_WIDTH / 2.0f) * blockSizeX + currChunkCenter.x;
//
//        for (int j = 0; j < MAX_BLOCK_WIDTH; ++j) {
//            // ���� currY��Chunk���Ĥ����Y�S�Ƅ���
//            float currY = (j - MAX_BLOCK_WIDTH / 2.0f) * blockSizeY + currChunkCenter.y;
//            chunk.m_BlockHeight[i][j] = Noise::Perlin(XMFLOAT2(currX*m_FrequencyX, currY*m_FrequencyY),m_Amplitude);
//            chunk.m_BlockHeight[i][j] *= MAX_BLOCK_HEIGHT;
//            chunk.m_BlockCenter[i][j] = XMFLOAT2(currX, currY);
//        }
//    }
//}

void ChunkManager::GenerateBlock(Chunk& chunk)
{
    float blockSizeX = 1.0f / MAX_BLOCK_WIDTH;
    float blockSizeY = 1.0f / MAX_BLOCK_WIDTH;

    XMFLOAT2 currChunkCenter = chunk.GetCenter();

    for (int i = 0; i < MAX_BLOCK_WIDTH; ++i) {
        // ���� currX��Chunk���Ĥ����X�S�Ƅ���
        float currX = (i - MAX_BLOCK_WIDTH / 2.0f) * blockSizeX + currChunkCenter.x;

        for (int j = 0; j < MAX_BLOCK_WIDTH; ++j) {
            // ���� currY��Chunk���Ĥ����Y�S�Ƅ���
            float currY = (j - MAX_BLOCK_WIDTH / 2.0f) * blockSizeY + currChunkCenter.y;

            float totalHeight = 0;
            float frequency = m_FrequencyX; // ����Ƶ��
            float amplitude = m_Amplitude;  // �������
            float persistence = .2;        // ����ξA������
            int octaves = m_Octaves;                // �Υ����λ���

            for (int octave = 0; octave < octaves; octave++) {
                totalHeight += Noise::Perlin(XMFLOAT2(currX * frequency, currY * frequency), amplitude);
                frequency *= 2.0f; 
                amplitude *= persistence;  
            }
            chunk.m_BlockHeight[i][j] = totalHeight / octaves; // �ߤ���ƽ����
            chunk.m_BlockHeight[i][j] *= MAX_BLOCK_HEIGHT;

            //chunk.m_BlockHeight[i][j] = Noise::Perlin(XMFLOAT2(currX*m_FrequencyX, currY*m_FrequencyY),m_Amplitude);
            //chunk.m_BlockHeight[i][j] *= MAX_BLOCK_HEIGHT;
            chunk.m_BlockCenter[i][j] = XMFLOAT2(currX, currY);
        }
    }
}

void ChunkManager::LoadAllChunk()
{
    


    for (int i = 0; i < m_Chunk.size(); ++i) {
        LoadBlock(m_Chunk[i]);
    }
}

void ChunkManager::DeleteAllChunk()
{
    m_Chunk.clear();
    m_BlockInstanceData.clear();
    m_BlockTransform.clear();
}

void ChunkManager::GenerateChunk(Player& player)
{
    DeleteAllChunk();
    InitChunksAroundPlayer(player.GetPosition2D());
    LoadAllChunk();
    UpdatePlayerChunk(player);
}

void ChunkManager::UpdatePlayerChunk(Player& player)
{
    XMFLOAT2 playerPosition = player.GetScalePosition2D();

    XMFLOAT2 CenterPosition = ChunkFfloor(playerPosition);

    for (auto& playerChunk : m_Chunk) {
        if (playerChunk.GetCenter().x == CenterPosition.x && playerChunk.GetCenter().y == CenterPosition.y) {
            player.GetChunkOfPlayer(&playerChunk);
            break;
        }
    }
}



void ChunkManager::CalculateRequiredChunks(const Player& player)
{
}

void ChunkManager::LoadRequiredChunks()
{
}

void ChunkManager::UnloadExcessChunks()
{
}
