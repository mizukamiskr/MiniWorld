#include "Chunk.h"

Chunk::Chunk(XMFLOAT2 chunkPosition):
	m_BlockHeight{0},m_BlockTemperature{0},m_BlockHumidity{0}
{
	ChunkCenter = chunkPosition;
}

void Chunk::SetCenter(XMFLOAT2 center)
{
	ChunkCenter = center;
}

XMFLOAT2 Chunk::GetCenter()
{
	return ChunkCenter;
}
