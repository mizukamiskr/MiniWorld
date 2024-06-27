#pragma once

#include <DirectXMath.h>
#include "Block.h"

using namespace DirectX;


#define MAX_BLOCK_WIDTH 16
#define MAX_BLOCK_HEIGHT 20
#define MAP_SCALE (MAX_BLOCK_WIDTH*BLOCK_SIZE)
#define MAX_BLOCK_NUM (MAX_BLOCK_WIDTH*MAX_BLOCK_WIDTH*MAX_BLOCK_HEIGHT)


//ゲーム世界をChunkに分けて、一個のChunkをは最大(MAX_BLOCK_WIDTH)*(MAX_BLOCK_WIDTH)*高さのブロックを管理できます
/*
//縦と横のサイズはMAX_BLOCK_WIDTH
//各々のブロックのに温度湿度高さが持つ
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

	//Chunkの中心座標を返す
	XMFLOAT2 GetCenter();

	//Chunk内の各々ブロックの中心座標(2D方向)
	XMFLOAT2 m_BlockCenter[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];

	//高さ
	float m_BlockHeight[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];

	//温度
	float m_BlockTemperature[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];


	//湿度
	float m_BlockHumidity[MAX_BLOCK_WIDTH][MAX_BLOCK_WIDTH];

	//ブロックのコリジョン
	std::vector<BoundingBox>m_BlockCollision;

private:
	XMFLOAT2 ChunkCenter;
	
};

