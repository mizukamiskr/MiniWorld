#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class LImGui {
public:
	static bool Initialize(HWND Hwnd,ID3D11Device* device, ID3D11DeviceContext* context)
	{
		//���ڻ�Imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // ������̿���
        io.ConfigWindowsMoveFromTitleBarOnly = true;              // ����������϶�

        // Imgui���������O��
        ImGui::StyleColorsDark();

        // ��������O�á�������ɥ����O��
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
        // �����v������ӳ�����Direct3d���軭���ޤ�
        // �ʤΤǤ���ǰ�ϥХå��Хåե��������󥰥ѥ��ץ饤���bind
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    static void ShowImguiTutorialWindow()
    {
        ImGui::ShowAboutWindow();
        ImGui::ShowDemoWindow();
        ImGui::ShowUserGuide();
    }

    static void CreateUiWithImage(const char* windowId,ID3D11ShaderResourceView* pTarget,int width,int height,int _x,int _y,int pSizeX,int pSizeY) {
        ImGui::SetNextWindowPos(ImVec2(_x, _y)); // ������ɥ�������
        ImGui::SetNextWindowSize(ImVec2(width, height)); //��������
        ImGui::SetNextWindowBgAlpha(0.0f); // ͸���ˤʤ�

        // IMGUIԪ���Υ�����ɥ����������ޤ�
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
        ImGui::SetNextWindowSize(ImVec2(width, height)); //��������

        // IMGUIԪ���Υ�����ɥ����������ޤ�
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