#ifndef GAMEAPP_H
#define GAMEAPP_H

#include "d3dApp.h"

//Chunk関連

const int buffSize = 3;
const int LoadRadius = 2;

class GameApp : public D3DApp
{

public:
    GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth = 1280, int initHeight = 720);
    ~GameApp();

    bool Init();
    void OnResize();
    void UpdateScene(float dt);
    void DrawScene();

private:
    bool InitResource();

    void RenderSkybox(ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);
    void RenderMapChip(ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);


    void OutPutNoiseP();
private:

    int m_SceneMode = 0;
    CameraMode m_CameraMode = FIRST;
    GpuTimer m_GpuTimer_Instancing;

    // GPUタイマー
    GpuTimer m_GpuTimer_Shadow;
    GpuTimer m_GpuTimer_Lighting;
    GpuTimer m_GpuTimer_Skybox;


    // MSAA
    int m_MsaaSamples = 1;

    // シャドウ


    // リソース
    TextureManager m_TextureManager;                                // TextureLoader
    ModelManager m_ModelManager;									// ModelLoader
    ChunkManager m_ChunkManager;

    std::unique_ptr<Texture2DMS> m_pLitBuffer;                      // レンダリングバッファ
    std::unique_ptr<Depth2DMS> m_pDepthBuffer;                      // 深度バッファ
    std::string m_Material;
    std::unique_ptr<Texture2D> m_pLitTexture;                           // 中间场景缓冲区

    // 模型
    GameObject m_Skybox;											// 天空盒模型
    Block m_Block;                                               //マップチップ
    Player m_Player;                                                //プレイヤー
    
    ComPtr<ID3D11ShaderResourceView> mBlockTexArray;             //マップチップテクスチャ配列
    std::unique_ptr<Buffer> m_pInstancedBuffer;                         // インスタンスバッファ



    // 特效
    SkyboxEffect m_SkyboxEffect;			                        // スガイボックスエフェクト
    InstanceEffect m_InstanceEffect;                                //実体化レンダリング
    PostProcessEffect m_PostProcessEffect;						        // 後処理エフェクト

    // 摄像机
    std::shared_ptr<Camera> m_pViewerCamera;						// ユーザーカメラ
    std::shared_ptr<Camera> m_pThirdCamera;							// 第三人称カメラ
    FirstPersonCameraController m_FPSCameraController;				// カメラコントローラー
    ThirdPersonCameraController m_ThirdCameraController;            //第三人称コントローラー

    bool m_EnableInstancing = true;								        //インスタンス生成
    float m_frustum = 1000;

    //Title
    ComPtr<ID3D11ShaderResourceView> m_TitleLogo;
    ComPtr<ID3D11ShaderResourceView> m_Start;
    ComPtr<ID3D11ShaderResourceView> m_End;
    ComPtr<ID3D11ShaderResourceView> m_Noise;

    bool isStart = false;
    bool m_EnableNoise = false;

    //Fade
    bool m_FadeUse = true;
    float m_FadeAmount = 0.0f;               //Fade係数
    float m_FadeState = 1.0f;               //1.0はFade-in,0.0はFade-out
};


#endif