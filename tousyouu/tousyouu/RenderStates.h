//***************************************************************************************
// RenderStates.h by X_Jun(MKXJun) (C) 2018-2022 All Rights Reserved.
// Licensed under the MIT License.
//
// 提供一些渲染状态
// Provide some render states.
//***************************************************************************************

#pragma once

#ifndef RENDER_STATES_H
#define RENDER_STATES_H

#include "WinMin.h"
#include <wrl/client.h>
#include <d3d11_1.h>


class RenderStates
{
public:
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    static bool IsInit();

    static void InitAll(ID3D11Device* device);
    // ComPtrを使用すると、手動で解放する必要がありません

public:
    static ComPtr<ID3D11RasterizerState> RSWireframe;		            // ラスタライザステート：ワイヤーフレームモード
    static ComPtr<ID3D11RasterizerState> RSNoCull;			            // ラスタライザステート：背面カリングなしモード
    static ComPtr<ID3D11RasterizerState> RSCullClockWise;	            // ラスタライザステート：時計回りカリングモード
    static ComPtr<ID3D11RasterizerState> RSShadow;						// ラスタライザステート：シャドウマップ用の深度バイアスモード

    static ComPtr<ID3D11SamplerState> SSPointClamp;						// サンプラーステート：ポイントフィルタリングとClampモード
    static ComPtr<ID3D11SamplerState> SSLinearWrap;			            // サンプラーステート：リニアフィルタリングとWrapモード
    static ComPtr<ID3D11SamplerState> SSLinearClamp;					// サンプラーステート：リニアフィルタリングとClampモード
    static ComPtr<ID3D11SamplerState> SSAnistropicWrap16x;		        // サンプラーステート：16x異方性フィルタリングとWrapモード
    static ComPtr<ID3D11SamplerState> SSAnistropicClamp2x;		        // サンプラーステート：2x異方性フィルタリングとClampモード
    static ComPtr<ID3D11SamplerState> SSAnistropicClamp4x;		        // サンプラーステート：4x異方性フィルタリングとClampモード
    static ComPtr<ID3D11SamplerState> SSAnistropicClamp8x;		        // サンプラーステート：8x異方性フィルタリングとClampモード
    static ComPtr<ID3D11SamplerState> SSAnistropicClamp16x;		        // サンプラーステート：16x異方性フィルタリングとClampモード
    static ComPtr<ID3D11SamplerState> SSShadowPCF;						// サンプラーステート：PCFを使ったシャドウマッピング用

    static ComPtr<ID3D11BlendState> BSTransparent;		                // ブレンドステート：透明度ブレンディング
    static ComPtr<ID3D11BlendState> BSAlphaToCoverage;	                // ブレンドステート：アルファトゥカバレッジ
    static ComPtr<ID3D11BlendState> BSAdditive;			                // ブレンドステート：加算ブレンディング
    static ComPtr<ID3D11BlendState> BSAlphaWeightedAdditive;            // ブレンドステート：アルファ重み付け加算ブレンディング

    static ComPtr<ID3D11DepthStencilState> DSSEqual;					// 深度/ステンシルステート：深度値が等しいピクセルのみ描画
    static ComPtr<ID3D11DepthStencilState> DSSLessEqual;                // 深度/ステンシルステート：従来のスカイボックス描画用
    static ComPtr<ID3D11DepthStencilState> DSSGreaterEqual;             // 深度/ステンシルステート：リバースZ用
    static ComPtr<ID3D11DepthStencilState> DSSNoDepthWrite;             // 深度/ステンシルステート：深度テストのみ、深度書き込み無し
    static ComPtr<ID3D11DepthStencilState> DSSNoDepthTest;              // 深度/ステンシルステート：深度テスト無し
    static ComPtr<ID3D11DepthStencilState> DSSWriteStencil;		        // 深度/ステンシルステート：深度テスト無し、ステンシル値書き込み
    static ComPtr<ID3D11DepthStencilState> DSSEqualStencil;	            // 深度/ステンシルステート：リバースZ、ステンシル値チェック
};



#endif
