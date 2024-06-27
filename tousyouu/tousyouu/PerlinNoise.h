#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <algorithm>
#include <cmath>

#undef max
#undef min

using namespace DirectX;

class Noise {
public:

	// 一次元のハッシュを生成します
	static int CreateHashOne(int position);

	// シード値を使ってハッシュを生成します
	static int createHashWithSeed(int position);

	// 二次元の位置から二次元のハッシュを生成します
	static XMFLOAT2 CreateHashTow(XMFLOAT2 position2D);

	// 二次元の位置から一次元のハッシュを生成します
	static int CreateHashOneForTwo(XMFLOAT2 position2D);

	// 三次元の位置から三次元のハッシュを生成します
	static XMFLOAT3 CreateHashThree(XMFLOAT3 position3D);

	// 三次元の位置から一次元のハッシュを生成します
	static int CreateHashOneForThree(XMFLOAT3 position3D);

	// ノイズ生成のためのシード値を設定します
	static void SetSeed(int _seed);

	// 勾配ノイズ関数です
	static float grad_f(XMFLOAT2 vertex, XMFLOAT2 position2D);

	// 勾配ノイズ関数です
	static float grad(XMFLOAT2 vertex, XMFLOAT2 position2D);

	// 二次元の位置でのPerlinノイズ値を計算します
	static float PerlinNoise(XMFLOAT2 position2D);

	// 二次元の位置でのValueノイズ値を計算します
	static float ValueNoise(XMFLOAT2 position2D);

	// 二次元の位置でのSimplexノイズ値を計算します
	static float SimplexNoise(XMFLOAT2 position2D);

	// 三次元の位置でのSimplexノイズ値を計算します
	static float SimplexNoise(XMFLOAT3 position3D);

	// Perlinノイズの事前処理を行います
	static void prehandlePerlinNoise(XMFLOAT2 position2D, int crystalSize);

public:
	// バージョン2のPerlinノイズ関数です
	static float Perlin(XMFLOAT2 position2D, float amplitude);

	// グリッド上の点とのドット積を計算します
	static float dotGridGrad(int ix, int iy, float x, float y);

	// ランダムな勾配ベクトルを生成します
	static XMFLOAT2 randomGrad(int ix, int iy);

private:
	static XMFLOAT2 Vertex[4]; // 頂点配列
	static XMFLOAT3 Vertex3D[3]; // 三次元の頂点配列
	static XMFLOAT2 offset; // オフセット
	static XMFLOAT3 offset3D; // 三次元のオフセット
	static int seed; // シード値

	Noise() = delete;
	~Noise() = delete; 
};

// 二つのXMFLOAT2ベクトルのドット積を計算します
inline float DotProduct(const XMFLOAT2& a, const XMFLOAT2& b)
{
	XMVECTOR va = XMLoadFloat2(&a);
	XMVECTOR vb = XMLoadFloat2(&b);

	XMVECTOR dot = XMVector2Dot(va, vb);

	float result;
	XMStoreFloat(&result, dot);

	return result;
}

// 線形補間を行います
inline float Lerp(float x, float y, float t)
{
	return (1 - t) * x + t * y;
}

// 値を指定された最小値と最大値の間に制限します
template <typename T>
T Clamp(const T& value, const T& min, const T& max)
{
	return std::max(min, std::min(max, value));
}

// 二つのXMFLOAT2ベクトルを減算します
inline XMFLOAT2 SubtractXMFLOAT2(const XMFLOAT2& a, const XMFLOAT2& b)
{
	XMFLOAT2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

// 
inline XMFLOAT2 Ffloor(XMFLOAT2 Position)
{
	XMFLOAT2 floorPosition;
	floorPosition.x = Position.x >= 0 ? (int)Position.x : (int)Position.x - 1;
	floorPosition.y = Position.y >= 0 ? (int)Position.y : (int)Position.y - 1;
	return floorPosition;
}

// Chunkの中心座標を正しく指定するために、0.5ごとにチェックします
inline XMFLOAT2 ChunkFfloor(XMFLOAT2 Position) {
	XMFLOAT2 floorPosition;
	floorPosition.x = floor(Position.x + 0.5f);
	floorPosition.y = floor(Position.y + 0.5f);
	return floorPosition;
}

// Floor関数
inline float Ffloor_f(float Position)
{
	return Position >= 0 ? (int)Position : (int)Position - 1;
}

// フェード関数
inline float Ffade(float t)
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

// 補間関数
inline float interpolate(float a0, float a1, float w)
{
	float f = Ffade(w);
	return (1 - f) * a0 + f * a1;
}