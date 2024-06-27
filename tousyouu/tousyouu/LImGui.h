#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class LImGui {
public:
	static bool Initialize(HWND Hwnd,ID3D11Device* device, ID3D11DeviceContext* context)
	{
		//初期化Imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // 允许键盘控制
        io.ConfigWindowsMoveFromTitleBarOnly = true;              // 仅允许标题拖动

        // Imguiスタイル設置
        ImGui::StyleColorsDark();

        // レンダリング設置、ウィンドウズ設置
        ImGui_ImplWin32_Init(Hwnd);
        ImGui_ImplDX11_Init(device, context);

        return true;
	}

    static void BeginFrame()
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    static void Render()
    {
        ImGui::Render();
        // この関数を呼び出すとDirect3dを描画します
        // なのでこの前はバックバッファをレンダリングパイプラインにbind
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    static void ShowImguiTutorialWindow()
    {
        ImGui::ShowAboutWindow();
        ImGui::ShowDemoWindow();
        ImGui::ShowUserGuide();
    }

    static void CreateUiWithImage(const char* windowId,ID3D11ShaderResourceView* pTarget,int width,int height,int _x,int _y,int pSizeX,int pSizeY) {
        ImGui::SetNextWindowPos(ImVec2(_x, _y)); // ウィンドウズ座標
        ImGui::SetNextWindowSize(ImVec2(width, height)); //　サイズ
        ImGui::SetNextWindowBgAlpha(0.0f); // 透明になる

        // IMGUI元々のウィンドウズを消します
        ImGui::Begin(windowId, nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground);

        ImGui::Image((void*)pTarget, ImVec2(pSizeX, pSizeY));
        ImGui::End();
    }

    static void CreateUiWithImageAndWindow(const char* windowId, ID3D11ShaderResourceView* pTarget, int width, int height, int _x, int _y, int pSizeX, int pSizeY) {
        ImGui::SetNextWindowSize(ImVec2(width, height)); //　サイズ

        // IMGUI元々のウィンドウズを消します
        ImGui::Begin(windowId, nullptr);

        ImGui::Image((void*)pTarget, ImVec2(pSizeX, pSizeY));
        ImGui::End();
    }

    static bool CreateUiWithImageButton(const char* windowId, ID3D11ShaderResourceView* pTarget, int width, int height, int _x, int _y, int pSizeX, int pSizeY) {
        ImGui::SetNextWindowPos(ImVec2(_x, _y));
        ImGui::SetNextWindowSize(ImVec2(width, height));
        ImGui::SetNextWindowBgAlpha(0.0f);

        ImGui::Begin(windowId, nullptr,
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBackground);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); 
        bool isClicked = ImGui::ImageButton((void*)pTarget, ImVec2(pSizeX, pSizeY));
        ImGui::PopStyleColor(3);
        //ImGui::PopStyleColor();
        ImGui::End();
        
        return isClicked;
    }
};