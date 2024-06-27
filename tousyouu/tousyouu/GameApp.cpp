#define NOMINMAX
#include "GameApp.h"
#include "XUtil.h"
#include "DXTrace.h"
#include "PerlinNoise.h"

using namespace DirectX;

#pragma warning(disable: 26812)

GameApp::GameApp(HINSTANCE hInstance, const std::wstring& windowName, int initWidth, int initHeight)
    : D3DApp(hInstance, windowName, initWidth, initHeight)
{
}

GameApp::~GameApp()
{

}

bool GameApp::Init()
{
    if (!D3DApp::Init())
        return false;

    m_TextureManager.Init(m_pd3dDevice.Get());
    m_ModelManager.Init(m_pd3dDevice.Get());

    m_GpuTimer_Instancing.Init(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get());



    //エフェクト使用する前に、必ず全てのレンダリング状態を初期化する
    RenderStates::InitAll(m_pd3dDevice.Get());

    if (!m_SkyboxEffect.InitAll(m_pd3dDevice.Get()))
        return false;

    if (!m_InstanceEffect.InitAll(m_pd3dDevice.Get()))
        return false;

    if (!m_PostProcessEffect.InitAll(m_pd3dDevice.Get()))
        return false;

    if (!InitResource())
        return false;

    OutPutNoiseP();

    return true;
}

void GameApp::OnResize()
{
    D3DApp::OnResize();

    DXGI_SAMPLE_DESC sampleDesc{};
    sampleDesc.Count = m_MsaaSamples;
    sampleDesc.Quality = 0;
    m_pLitBuffer = std::make_unique<Texture2DMS>(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, sampleDesc);
    m_pDepthBuffer = std::make_unique<Depth2DMS>(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight, sampleDesc, DepthStencilBitsFlag::Depth_32Bits);
    m_pLitTexture = std::make_unique<Texture2D>(m_pd3dDevice.Get(), m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM);


}

void GameApp::UpdateScene(float dt)
{

    // 画面遷移
    if (m_FadeUse)
    {
        m_FadeAmount += m_FadeState * dt / 2.0f;	// 画面遷移の時間は2秒間を設置
        if (m_FadeState > 0.0f && m_FadeAmount > 1.0f)
        {
            m_FadeAmount = 1.0f;
            m_FadeUse = false;	// FadeIn終了
        }
    }

    if (!isStart) {
        LImGui::CreateUiWithImage("title", m_TitleLogo.Get(), 500, 100, m_ClientWidth / 2 - 470 / 2, m_ClientHeight / 6, 470, 78);
        if (LImGui::CreateUiWithImageButton("start", m_Start.Get(), 300, 100, (m_ClientWidth / 2) - (260 / 2) - m_ClientWidth / 4, m_ClientHeight / 1.5, 260, 78)) {
            isStart = true;
        }
        if (LImGui::CreateUiWithImageButton("end", m_End.Get(), 250, 100, (m_ClientWidth / 2) - (170 / 2) + m_ClientWidth / 4, m_ClientHeight / 1.5, 170, 78)) {
            DestroyWindow(m_hMainWnd);
        }
        return;
    }

    // カメラを更新
    m_FPSCameraController.Update(dt);
    m_Player.Update(dt);
    m_ChunkManager.Update(m_Player);

    
    if (m_EnableNoise) 
    {
        LImGui::CreateUiWithImageAndWindow("noise", m_Noise.Get(), 300, 300, m_ClientWidth / 2, m_ClientHeight / 2, 200, 200);
    }


    if (m_CameraMode == CameraMode::FIRST)
    {
        m_FPSCameraController.SetCharacter(m_Player);
        m_Player.GetTransform().SetRotation(XMFLOAT3(0.0f, m_pViewerCamera.get()->GetRotationY() + XM_PI, 0.0f));
    }
    if (m_CameraMode == CameraMode::THIRD)
    {

    }
    m_InstanceEffect.SetViewMatrix(m_pViewerCamera->GetViewMatrixXM());
    m_SkyboxEffect.SetViewMatrix(m_pViewerCamera->GetViewMatrixXM());
    m_SkyboxEffect.SetProjMatrix(m_pViewerCamera->GetProjMatrixXM(true));


#pragma region IMGUI
    bool need_gpu_timer_reset = false;
    if (ImGui::Begin("MiniWorald Main Config"))
    {
        static int curr_camera = 0;
        static const char* camera_type_str[] = { "First","Thrid" };

        static int curr_material = 5;
        static const char* block_type_str[] = { "Stone","Brick","Pumpkin","Wood","Sand","Grass" };
        if (ImGui::Checkbox("Enable Instancing", &m_EnableInstancing))
        {
            m_GpuTimer_Instancing.Reset(m_pd3dImmediateContext.Get());
        }
        ImGui::Checkbox("See Noise Picture", &m_EnableNoise);
        ImGui::Checkbox("GodMode", &m_Player.m_isGodMode);

        if (ImGui::Combo("Camera", &curr_camera, camera_type_str, ARRAYSIZE(camera_type_str)))
        {
            //カメラ切り替え
            if (curr_camera == 0 && m_CameraMode != CameraMode::FIRST)
            {
                m_CameraMode = CameraMode::FIRST;
            }
            if (curr_camera == 1 && m_CameraMode != CameraMode::THIRD)
            {
                m_CameraMode = CameraMode::THIRD;
            }
        }

        if (ImGui::Combo("Material", &curr_material, block_type_str, ARRAYSIZE(block_type_str))) 
        {
            if(curr_material==0&&m_ChunkManager.GetMaterialIndex()!=0)
            {
                m_ChunkManager.SetMaterialIndex(curr_material);
                m_ChunkManager.GenerateChunk(m_Player);
            }
            if (curr_material == 1 && m_ChunkManager.GetMaterialIndex() != 1)
            {
                m_ChunkManager.SetMaterialIndex(curr_material);
                m_ChunkManager.GenerateChunk(m_Player);
            }
            if (curr_material == 2 && m_ChunkManager.GetMaterialIndex() != 2)
            {
                m_ChunkManager.SetMaterialIndex(curr_material);
                m_ChunkManager.GenerateChunk(m_Player);
            }
            if (curr_material == 3 && m_ChunkManager.GetMaterialIndex() != 3)
            {
                m_ChunkManager.SetMaterialIndex(curr_material);
                m_ChunkManager.GenerateChunk(m_Player);
            }
            if (curr_material == 4 && m_ChunkManager.GetMaterialIndex() != 4)
            {
                m_ChunkManager.SetMaterialIndex(curr_material);
                m_ChunkManager.GenerateChunk(m_Player);
            }
            if (curr_material == 5 && m_ChunkManager.GetMaterialIndex() != 5)
            {
                m_ChunkManager.SetMaterialIndex(curr_material);
                m_ChunkManager.GenerateChunk(m_Player);
            }
        }
        
        int bufferZone = m_ChunkManager.GetBufferZone();
        if (ImGui::SliderInt("Buffer Zone", &bufferZone, 0, 5)) {
            m_ChunkManager.SetBufferZone(bufferZone);
            m_ChunkManager.GenerateChunk(m_Player);
        }
        float noiseAmblitude = m_ChunkManager.GetAmplitude();
        if (ImGui::SliderFloat("Amplitude", &noiseAmblitude, 0.0f, 3.0f)) {
            m_ChunkManager.SetAmplitude(noiseAmblitude);
            //m_ChunkManager.GenerateChunk(m_Player);
        }
        float noiseFrequencyX = m_ChunkManager.GetFrequencyX();
        if (ImGui::SliderFloat("FrequencyX", &noiseFrequencyX, 0.0f, 3.0f)) {
            m_ChunkManager.SetFrequencyX(noiseFrequencyX);
            //m_ChunkManager.GenerateChunk(m_Player);
        }
        float noiseFrequencyY = m_ChunkManager.GetFrequencyY();
        if (ImGui::SliderFloat("FrequencyY", &noiseFrequencyY, 0.0f, 3.0f)) {
            m_ChunkManager.SetFrequencyY(noiseFrequencyY);
            //m_ChunkManager.GenerateChunk(m_Player);
        }

        int noiseOctaves = m_ChunkManager.GetOctaves();
        if (ImGui::SliderInt("Octaves", &noiseOctaves, 1, 5)) {
            m_ChunkManager.SetOctaves(noiseOctaves);
            m_ChunkManager.GenerateChunk(m_Player);
        }

        if (ImGui::Button("GenerateMap")) {
            m_ChunkManager.GenerateChunk(m_Player);
            if (m_EnableNoise) {
                OutPutNoiseP();
            }
        }


    }
    
    ImGui::End();

    ImGui::Begin("Performance");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

void GameApp::DrawScene()
{
    assert(m_pd3dImmediateContext);
    assert(m_pSwapChain);

    // バックバッファのレンダターゲットリソースを生成する（创建后备缓冲区的渲染目标视图）
    if (m_FrameCount < m_BackBufferCount)
    {
        ComPtr<ID3D11Texture2D> pBackBuffer;
        m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(pBackBuffer.GetAddressOf()));
        CD3D11_RENDER_TARGET_VIEW_DESC rtvDesc(D3D11_RTV_DIMENSION_TEXTURE2D, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
        m_pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), &rtvDesc, m_pRenderTargetViews[m_FrameCount].ReleaseAndGetAddressOf());
    }

    //
    // レンダリング
    //
    
    RenderSkybox(m_FadeUse ? m_pLitBuffer->GetRenderTarget() : GetBackBufferRTV(),
        m_pDepthBuffer->GetDepthStencil());
    RenderMapChip(m_FadeUse ? m_pLitTexture->GetRenderTarget() : GetBackBufferRTV(),
        m_pDepthBuffer->GetDepthStencil());

    if (m_FadeUse)
    {
        // fade-in
        m_PostProcessEffect.RenderScreenFade(
            m_pd3dImmediateContext.Get(),
            m_pLitTexture->GetShaderResource(),
            //mEnd.Get(),
            GetBackBufferRTV(),
            m_pViewerCamera->GetViewPort(),
            m_FadeAmount
        );
    } 
    
    D3D11_VIEWPORT vp = m_pViewerCamera->GetViewPort();
    m_pd3dImmediateContext->RSSetViewports(1, &vp);
    ImGui::Render();

    ID3D11RenderTargetView* pRTVs[] = { GetBackBufferRTV() };
    m_pd3dImmediateContext->OMSetRenderTargets(1, pRTVs, nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    HR(m_pSwapChain->Present(0, m_IsDxgiFlipModel ? DXGI_PRESENT_ALLOW_TEARING : 0));

}

bool GameApp::InitResource()
{
    // ******************
    // カメラとコントローラー初期化
    //

    //サイズを設置
    m_pInstancedBuffer = std::make_unique<Buffer>(m_pd3dDevice.Get(),
        CD3D11_BUFFER_DESC(sizeof(InstanceEffect::InstancedData) * 700000, D3D11_BIND_VERTEX_BUFFER,
            D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE));


    //FPS Camera
    auto viewerCamera = std::make_shared<FirstPersonCamera>();
    m_pViewerCamera = viewerCamera;
    //レンダリング範囲設置
    m_pViewerCamera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
    m_pViewerCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, m_frustum);
    viewerCamera->LookAt(XMFLOAT3(100.0f, 250.0f, 5.0f), XMFLOAT3(), XMFLOAT3(0.0f, 2500.0f, 0.0f));
    m_FPSCameraController.InitCamera(viewerCamera.get());
    m_FPSCameraController.SetMoveSpeed(100.0f);


    //カメラ（freeCamera）
    auto thirdViewCamera = std::make_shared<ThirdPersonCamera>();
    m_pThirdCamera = thirdViewCamera;
    m_pThirdCamera->SetViewPort(0.0f, 0.0f, (float)m_ClientWidth, (float)m_ClientHeight);
    m_pThirdCamera->SetFrustum(XM_PI / 3, AspectRatio(), 0.5f, 300.0f);
    thirdViewCamera->SetTarget(m_Player.GetTransform().GetPosition());
    thirdViewCamera->SetDistance(8.0f);
    thirdViewCamera->SetDistanceMinMax(3.0f, 20.0f);
    m_ThirdCameraController.InitCamera(thirdViewCamera.get());
    m_ThirdCameraController.SetMoveSpeed(100.0f);

    // ******************
    // レンダリングパス初期化
    //

    m_SkyboxEffect.SetMsaaSamples(1);
    m_InstanceEffect.SetViewMatrix(m_pViewerCamera->GetViewMatrixXM());
    m_InstanceEffect.SetProjMatrix(m_pViewerCamera->GetProjMatrixXM());


    // ******************
    // オブジェクト初期化
    //

    //SkyBox
    m_ModelManager.CreateFromGeometry("cube", Geometry::CreateBox());
    m_TextureManager.CreateFromFile("Assets\\Texture\\Clouds.dds");
    m_ModelManager.CreateFromGeometry("skyboxCube", Geometry::CreateBox());
    Model* pModel = m_ModelManager.GetModel("skyboxCube");
    pModel->materials[0].Set<std::string>("$Skybox", "Assets\\Texture\\Clouds.dds");
    m_Skybox.SetModel(pModel);

    //Player
    pModel = m_ModelManager.CreateFromFile("Player", "Assets\\Model\\mcsteve.obj");
    m_Player.SetModel(pModel);
    m_Player.GetTransform().SetScale(10.0f, 10.0f, 10.0f);
    m_Player.SetMoveSpeed(50.0f);
    m_Player.GetTransform().SetPosition(XMFLOAT3(0.0f,300.0f,0.0f));

    //カメラをプレイヤーの目のところに設置、方向とプレイヤーの向きと同じくなる
    viewerCamera->LookAt(XMFLOAT3(0.0f, 6.0f, 1.1f),
        XMFLOAT3(m_Player.GetTransform().GetForwardAxis().x, m_Player.GetTransform().GetForwardAxis().y + 6, m_Player.GetTransform().GetForwardAxis().z),
        XMFLOAT3(0.0f, 1.0f, 0.0f));

    //FreeCameraはいつでもプレイヤーを見ています
    thirdViewCamera->SetTarget(m_Player.GetTransform().GetPosition());


    //マップチップ用のテクスチャ配列初期化
    HR(CreateTexture2DArrayFromFile(
        m_pd3dDevice.Get(),
        m_pd3dImmediateContext.Get(),
        std::vector<std::wstring>{
        L"Assets\\Texture\\MineCraft\\Stone.png",
            L"Assets\\Texture\\MineCraft\\Brick.png",
            L"Assets\\Texture\\MineCraft\\Pumpkin.png",
            L"Assets\\Texture\\MineCraft\\Wood.png",
            L"Assets\\Texture\\MineCraft\\Sand.png",
            L"Assets\\Texture\\MineCraft\\GrassTop.png",
            L"Assets\\Texture\\MineCraft\\GrassSide.png",
            L"Assets\\Texture\\MineCraft\\GrassBottom.png"},
        nullptr,
        mBlockTexArray.GetAddressOf()));

    m_InstanceEffect.SetTextureArray(mBlockTexArray.Get());

    // ******************
    // Chunk関連初期化
    //

    // テストマップチップロード
    pModel = m_ModelManager.CreateFromGeometry("Block", Geometry::CreateBox(BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE));
    m_Block.SetModel(pModel);
    pModel->SetDebugObjectName("Block");

    m_ChunkManager.init(m_ModelManager,m_Player);

    m_ChunkManager.InitChunksAroundPlayer(m_Player.GetPosition2D());

    m_ChunkManager.LoadAllChunk();

    m_ChunkManager.UpdatePlayerChunk(m_Player);

    // ******************
    // TITLE関連
    //
    m_TitleLogo = m_TextureManager.CreateFromFile("Assets\\Texture\\TITLE\\titleLogo.png");
    m_Start = m_TextureManager.CreateFromFile("Assets\\Texture\\TITLE\\Start.png");
    m_End = m_TextureManager.CreateFromFile("Assets\\Texture\\TITLE\\End.png");


    // ******************
    // デバッグ名を設置
    //
    m_pLitBuffer->SetDebugObjectName("LitBuffer");

    return true;
}



void GameApp::RenderSkybox(ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
{

    float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    m_pd3dImmediateContext->ClearRenderTargetView(pRTV, black);

    D3D11_VIEWPORT skyboxViewport = m_pViewerCamera->GetViewPort();
    skyboxViewport.MinDepth = 1.0f;
    skyboxViewport.MaxDepth = 1.0f;
    m_pd3dImmediateContext->RSSetViewports(1, &skyboxViewport);

    m_SkyboxEffect.SetRenderDefault();
    m_SkyboxEffect.SetLitTexture(m_pLitBuffer->GetShaderResource());
    m_SkyboxEffect.SetDepthTexture(m_pDepthBuffer->GetShaderResource());

    //全画面描画のため、深度バッファは必要ないので、バックバッファをクリアせずに描画する
    m_pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);
    ID3D11RenderTargetView* pRTVs[] = { pRTV };
    m_pd3dImmediateContext->OMSetRenderTargets(1, pRTVs, nullptr);
    m_Skybox.Draw(m_pd3dImmediateContext.Get(), m_SkyboxEffect);

    // クリア
    m_pd3dImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_SkyboxEffect.SetLitTexture(nullptr);
    m_SkyboxEffect.SetDepthTexture(nullptr);
    m_SkyboxEffect.Apply(m_pd3dImmediateContext.Get());
}

void GameApp::RenderMapChip(ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
{


    m_pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    ID3D11RenderTargetView* pRTVs[] = { pRTV };
    m_pd3dImmediateContext->OMSetRenderTargets(1, pRTVs, pDSV);
    D3D11_VIEWPORT viewport = m_pViewerCamera->GetViewPort();
    m_pd3dImmediateContext->RSSetViewports(1, &viewport);


    // インスタンスの数を記録する
    auto& instancedData = m_ChunkManager.GetBlockInstanceData();
    const auto& refTransforms = m_ChunkManager.GetBlockTransforms();
    auto& refObject = m_Block;

    m_GpuTimer_Instancing.Start();
    uint32_t objectCount = (uint32_t)instancedData.size();
    uint32_t drawCount = (uint32_t)instancedData.size();

    if (m_EnableInstancing) {
        // インスタンス描画
        const auto& refData = instancedData;
        // インスタンスデータをアップする
        memcpy_s(m_pInstancedBuffer->MapDiscard(m_pd3dImmediateContext.Get()),
            m_pInstancedBuffer->GetByteWidth(), refData.data(), refData.size() * sizeof(InstanceEffect::InstancedData));
        m_pInstancedBuffer->Unmap(m_pd3dImmediateContext.Get());
        m_InstanceEffect.DrawInstanced(m_pd3dImmediateContext.Get(), *m_pInstancedBuffer, refObject, (uint32_t)refData.size());
    }
    else {
        m_InstanceEffect.SetRenderDefault();
        size_t sz = refTransforms.size();
        for (size_t i = 0; i < sz; ++i)
        {
            refObject.GetTransform() = refTransforms[i];
            m_InstanceEffect.SetDiffuseColor(instancedData[i].color);
            refObject.Draw(m_pd3dImmediateContext.Get(), m_InstanceEffect);
        }
    }

    m_InstanceEffect.SetRenderDefault();
    m_Player.Draw(m_pd3dImmediateContext.Get(), m_InstanceEffect);

    m_pd3dImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

    m_GpuTimer_Instancing.Stop();

    if (isStart) {
        if (ImGui::Begin("MiniWorald Main Config"))
        {
            ImGui::Text("Objects: %u/%u", drawCount, objectCount);

            m_GpuTimer_Instancing.TryGetTime(nullptr);
            double avgTime = m_GpuTimer_Instancing.AverageTime();

            ImGui::Text("Instance Pass: %.3fms", avgTime * 1000.0);
        }

        ImGui::End();
    }
}



void GameApp::OutPutNoiseP()
{
    

    int width = 800;
    int height = 800;


    //プレーヤーに中心点して生成したいならcameraPosition,Chunkに中心点として生成したいの場合はCenterPosition
    XMFLOAT2 cameraPosition = { m_pViewerCamera.get()->GetPosition().x / (MAX_BLOCK_WIDTH * BLOCK_SIZE) ,
    m_pViewerCamera.get()->GetPosition().z / (MAX_BLOCK_WIDTH * BLOCK_SIZE) };

    XMFLOAT2 CenterPosition = Ffloor(cameraPosition);


    float CurrWidth = 1 / width;
    float CurrHeight = 1 / height;
    ID3D11Texture2D* texture = nullptr;
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    m_pd3dDevice->CreateTexture2D(&desc, nullptr, &texture);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    m_pd3dImmediateContext->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    float noiseInputX = cameraPosition.x - buffSize - 0.5f;
    // テクスチャデータ打ち込む
    for (int y = 0; y < height; ++y) {
        noiseInputX += 0.0075;
        float noiseInputY = cameraPosition.y + buffSize + 0.5f;
        for (int x = 0; x < width; ++x) {
            noiseInputY -= 0.0075;

            
            XMFLOAT2 outPutNoise = { noiseInputX* m_ChunkManager.GetFrequencyX() ,noiseInputY* m_ChunkManager.GetFrequencyY() };

            float noiseValue = Noise::Perlin(outPutNoise,m_ChunkManager.GetAmplitude()); // パリーノイズ
            // ノイズは色を転換
            unsigned char color = static_cast<unsigned char>(noiseValue * 255);
            unsigned int* pixel = reinterpret_cast<unsigned int*>(
                static_cast<char*>(mappedResource.pData) + y * mappedResource.RowPitch) + x;
            unsigned int rgbaColor = (color << 16) | (color << 8) | color | (0xFF << 24);
            *pixel = rgbaColor; // RGB
        }
    }

    

    m_pd3dImmediateContext->Unmap(texture, 0);

    ID3D11ShaderResourceView* srv = nullptr;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;
    m_pd3dDevice->CreateShaderResourceView(texture, &srvDesc, &srv);

    DirectX::ScratchImage scratchImage;
    HRESULT hr = DirectX::CaptureTexture(m_pd3dDevice.Get(), m_pd3dImmediateContext.Get(), texture, scratchImage);
    m_Noise = srv;
    const DirectX::Image* img = scratchImage.GetImages(); //第一位の画像を読み込む
    hr = DirectX::SaveToWICFile(
        *img,                           // Image
        DirectX::WIC_FLAGS_NONE,        // flags
        GUID_ContainerFormatPng,        // PNG
        L"NoisTest.png"                   // ファイル名
    );
}


