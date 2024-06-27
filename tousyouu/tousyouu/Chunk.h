#pragma once

#include <DirectXMath.h>
#include "Block.h"

using namespace DirectX;


#define MAX_BLOCK_WIDTH 16
#define MAX_BLOCK_HEIGHT 20
#define MAP_SCALE (MAX_BLOCK_WIDTH*BLOCK_SIZE)
#define MAX_BLOCK_NUM (MAX_BLOCK_WIDTH*MAX_BLOCK_WIDTH*MAX_BLOCK_HEIGHT)


//���`�������Chunk�˷֤��ơ�һ����Chunk������(MAX_BLOCK_WIDTH)*(MAX_BLOCK_WIDTH)*�ߤ��Υ֥�å������Ǥ��ޤ�
/*
//�k�Ⱥ�Υ�������MAX_BLOCK_WIDTH
//�����Υ֥�å��Τ��¶�ʪ�ȸߤ����֤�
+----+----+----+----+    
| 1  | 2  | 3  | 4  |    
+----+----+----+----+    
| 5  | 6  | 7  | 8  |    
+----+----+----+----+
| 9  | 10 | 11 | 12 |
+----+----+----+----+
| 13 | 14 | 15 | 16 |
+----+----+----+----+
*/
class Chunk
{
public:
	Chunk() = default;
	~Chunk() = default;

	Chunk(XMFLOAT2 chunkPosition);

	void SetCenter(XMFLOAT2 center);

	//Chunk���������ˤ򷵤�
	XMFLOAT2 GetCenter();

	//Chunk�ڤθ����֥�å�����������(2D����)
	XMFLOAT2 m_BlockCenter[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];

	//�ߤ�
	float m_BlockHeight[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];

	//�¶�
	float m_BlockTemperature[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];


	//ʪ��
	float m_BlockHumidity[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];

	//�֥�å��Υ��ꥸ���
	std::vector<BoundingBox>m_BlockCollision;

private:
	XMFLOAT2 ChunkCenter;
	
};

