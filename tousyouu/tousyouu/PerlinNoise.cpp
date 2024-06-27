#include "PerlinNoise.h"

//
XMFLOAT2 Noise::Vertex[4] = {};
XMFLOAT3 Noise::Vertex3D[3] = {};

//
XMFLOAT2 Noise::offset = { 0,0 };
XMFLOAT3 Noise::offset3D = { 0,0,0 };

//
int Noise::seed = 116252532;

int Noise::CreateHashOne(int position)
{
	const int BIT_NOISE1 = 0x85297A4D;
	const int BIT_NOISE2 = 0x68E31DA4;
	const int BIT_NOISE3 = 0x1B56C4E9;
	int mangled = position;
	mangled *= BIT_NOISE1;
	mangled ^= (mangled >> 8);
	mangled += BIT_NOISE2;
	mangled ^= (mangled << 8);
	mangled *= BIT_NOISE3;
	mangled ^= (mangled >> 8);
	return mangled % 1024;
}

int Noise::createHashWithSeed(int position)
{
	const int BIT_NOISE1 = 0x85297A4D;
	const int BIT_NOISE2 = 0x68E31DA4;
	const int BIT_NOISE3 = 0x1B56C4E9;
	int mangled = position ^ seed;
	mangled *= BIT_NOISE1;
	mangled ^= (mangled >> 8);
	mangled += BIT_NOISE2;
	mangled ^= (mangled << 8);
	mangled *= BIT_NOISE3;
	mangled ^= (mangled >> 8);
	return mangled % 1024;
}

XMFLOAT2 Noise::CreateHashTow(XMFLOAT2 position2D)
{
	XMFLOAT2 v(
		CreateHashOne(0x651A6BE3 * (int)position2D.x - (int)position2D.y) % 1024,
		CreateHashOne((int)position2D.x * (int)position2D.y + 0x218AE247) % 1024
	);
	v.x /= 1024.0f;
	v.y /= 1024.0f;
	return v;
}

int Noise::CreateHashOneForTwo(XMFLOAT2 position2D)
{
	return CreateHashOne(0x651A6BE1 * (int)position2D.x + (int)position2D.y) % 1024;
}

XMFLOAT3 Noise::CreateHashThree(XMFLOAT3 position3D)
{
	XMFLOAT3 v(
		CreateHashOne((int)position3D.x ^ 0x651A6BE3 + (int)position3D.y ^ 0x218A6147 - (int)position3D.z ^ 0x118A5191) % 1024,
		CreateHashOne((int)position3D.x ^ 0x118A5191 - (int)position3D.y ^ 0x218AE247 + (int)position3D.z ^ 0x2B8AE147) % 1024,
		CreateHashOne((int)position3D.x ^ 0x21613122 - (int)position3D.y ^ 0x118A5191 - (int)position3D.z ^ 0x218AE247) % 1024
	);
	v.x /= 1024.0f;
	v.y /= 1024.0f;
	v.z /= 1024.0f;
	return v;
}

int Noise::CreateHashOneForThree(XMFLOAT3 position3D)
{
	return CreateHashOne(position3D.x * 0x651A6BE6 - position3D.y * 0xCB251062 + position3D.z);
}

void Noise::SetSeed(int _seed)
{
	seed = _seed;
}

float Noise::grad(XMFLOAT2 vertex, XMFLOAT2 position2D)
{

	switch ((0x651A6BE1 * (int)vertex.x + (int)vertex.y) % 4)
	{
	case 1: return  position2D.x + position2D.y;  //(0,0)
	case 2: return -position2D.x + position2D.y;  //(-1,1)
	case 3: return  position2D.x - position2D.y;  //(1,-1)
	case 4: return -position2D.x - position2D.y;  //(-1,-1)
	default: return 0;
	}
}


float Noise::grad_f(XMFLOAT2 vertex, XMFLOAT2 position2D)
{
	return DotProduct(vertex, position2D);
}


float Noise::PerlinNoise(XMFLOAT2 position2D)
{
	position2D.x = offset.x + position2D.x;
	position2D.y = offset.y + position2D.y;

	XMFLOAT2 w = position2D;


	XMFLOAT2 vertexRD = SubtractXMFLOAT2(position2D, XMFLOAT2(1.0f, 0.0f));
	XMFLOAT2 vertexLU = SubtractXMFLOAT2(position2D, XMFLOAT2(0.0f, 1.0f));
	XMFLOAT2 vertexRU = SubtractXMFLOAT2(position2D, XMFLOAT2(1.0f, 1.0f));

	return Clamp<float>(
			Lerp(
				Lerp(grad_f(Vertex[0], position2D),grad_f(Vertex[1], vertexRD),w.x),
				Lerp(grad_f(Vertex[2], vertexLU),grad_f(Vertex[3], vertexRU),w.x),w.y),
		-1, 1);
		
}


void Noise::prehandlePerlinNoise(XMFLOAT2 position2D, int crystalSize)
{
	XMFLOAT2 pi = XMFLOAT2(floor(position2D.x / crystalSize), floor(position2D.y / crystalSize));
	XMFLOAT2 vertex[4] = { {pi.x,pi.y},{pi.x + 1,pi.y},{pi.x,pi.y + 1},{pi.x + 1,pi.x + 1} };

	for (int i = 0; i < 4; ++i)
		Vertex[i] = Noise::CreateHashTow(vertex[i]);
	XMFLOAT2 plusReslut = { pi.x * crystalSize,pi.y * crystalSize };
	XMFLOAT2 result = SubtractXMFLOAT2(position2D, plusReslut);

	XMFLOAT2 currOffset = { result.x / crystalSize,result.y / crystalSize };

	offset = currOffset;
}


//ver2
float Noise::Perlin(XMFLOAT2 position2D,float amplitude)
{
	float x0 = Ffloor_f(position2D.x);
	float y0 = Ffloor_f(position2D.y);
	float x1 = x0 + 1;
	float y1 = y0 + 1;

	//
	float sx = position2D.x - x0;
	float sy = position2D.y - y0;

	//
	float v0 = dotGridGrad(x0, y0, position2D.x, position2D.y);
	float v1 = dotGridGrad(x1, y0, position2D.x, position2D.y);
	float ix0 = interpolate(v0, v1, sx);

	//
	v0 = dotGridGrad(x0, y1, position2D.x, position2D.y);
	v1 = dotGridGrad(x1, y1, position2D.x, position2D.y);
	float ix1 = interpolate(v0, v1, sx);

	//
	float value = interpolate(ix0, ix1, sy);

	value *= amplitude;

	value = (value + 1.0f) / 2.0f;

	return value;
}


float Noise::dotGridGrad(int ix, int iy, float x, float y)
{	
	XMFLOAT2 gradient = randomGrad(ix, iy);

	float dx = x - (float)ix;
	float dy = y - (float)iy;

	//
	return (dx * gradient.x + dy * gradient.y);
}

XMFLOAT2 Noise::randomGrad(int ix, int iy)
{

	// 固定的梯度向量集合
	static const XMFLOAT2 gradients[] = {
		XMFLOAT2(1.0f, 0.0f), XMFLOAT2(-1.0f, 0.0f),
		XMFLOAT2(0.0f, 1.0f), XMFLOAT2(0.0f, -1.0f),
		XMFLOAT2(1.0f, 1.0f), XMFLOAT2(-1.0f, -1.0f),
		XMFLOAT2(1.0f, -1.0f), XMFLOAT2(-1.0f, 1.0f)
	};
	//// 使用简单的伪随机数生成器
	//int random = 2920 * sin(ix * 21942 + iy * 171324 + 8912) * cos(ix * 23157 * iy * 217832 + 9758);
	//return gradients[random & 7];  // 取模运算限制索引在0-7之间

	// random
	int hash = ix * 73856093 ^ iy * 19349663 ^ seed * 83492791;

	// 使用哈希值的某些位来选择梯度向量
	return gradients[hash & 7];

}