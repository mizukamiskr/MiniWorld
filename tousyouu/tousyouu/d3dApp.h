#pragma once

#ifndef D3DAPP_H
#define D3DAPP_H

#include <wrl/client.h>
#include <string>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <random>
#include "CpuTimer.h"
#include "GpuTimer.h"

#include "WinMin.h"
#include "CameraController.h"
#include "RenderStates.h"
#include "Texture2D.h"
#include "Buffer.h"
#include "TextureManager.h"
#include "LImGui.h"
#include "ShaderDefines.h"
#include "ChunkManager.h"

#include <wincodec.h>
#include <DirectXTex.h>

class D3DApp
{
public:
    D3DApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight);
    virtual ~D3DApp();

    HINSTANCE AppInst()const;       
    HWND      MainWnd()const;        
    float     AspectRatio()const;   

    int Run();                     

    // ベース
    virtual bool Init();                      
    virtual void OnResize();                  
    virtual void UpdateScene(float dt) = 0;   
    virtual void DrawScene() = 0;             
    virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); 

protected:
    bool InitMainWindow();      // 初期化
    bool InitDirect3D();        // Direct3D初期化

    void CalculateFrameStats(); // フレーム数の計算と画面に示す
    ID3D11RenderTargetView* GetBackBufferRTV() { return m_pRenderTargetViews[m_FrameCount % m_BackBufferCount].Get(); }

protected:
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    HINSTANCE m_hAppInst;        // アプリケーションのインスタンスハンドル
    HWND      m_hMainWnd;        // メインウィンドウのハンドル
    bool      m_AppPaused;       // アプリケーションが一時停止しているか
    bool      m_Minimized;       // アプリケーションが最小化されているか
    bool      m_Maximized;       // アプリケーションが最大化されているか
    bool      m_Resizing;        // ウィンドウのサイズが変更されているか

    bool m_IsDxgiFlipModel = false; // DXGIフリップモデルを使用しているか
    UINT m_BackBufferCount = 0;     // バックバッファの数
    UINT m_FrameCount = 0;          // 現在のフレーム数
    ComPtr<ID3D11RenderTargetView> m_pRenderTargetViews[2];     // すべてのバックバッファに対応するレンダーターゲットビュー

    CpuTimer m_Timer;            // タイマー

    // Direct3D 11
    ComPtr<ID3D11Device> m_pd3dDevice;                          // D3D11デバイス
    ComPtr<ID3D11DeviceContext> m_pd3dImmediateContext;         // D3D11デバイスコンテキスト
    ComPtr<IDXGISwapChain> m_pSwapChain;                        // D3D11スワップチェーン
    // Direct3D 11.1
    ComPtr<ID3D11Device1> m_pd3dDevice1;                        // D3D11.1デバイス
    ComPtr<ID3D11DeviceContext1> m_pd3dImmediateContext1;       // D3D11.1デバイスコンテキスト
    ComPtr<IDXGISwapChain1> m_pSwapChain1;                      // D3D11.1スワップチェーン

    std::wstring m_MainWndCaption;                              // メインウィンドウのキャプション
    int m_ClientWidth;                                          // クライアント領域の幅
    int m_ClientHeight;                                         // クライアント領域の高さ
};

#endif // D3DAPP_H