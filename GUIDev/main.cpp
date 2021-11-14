// GUIDev.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"

#include <d3d11.h>
#include "Contour_Path.h"
#include "ImGui_Style.h"
#include "ImGui_Anim.h"
#include <Windows.h>
#include <tchar.h>
#include <D3DX11tex.h>
#include "image.h"
#include "INRIX_API.h"
#include "font.h"

ID3D11ShaderResourceView* map = NULL;
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;
ImFont* Smallfont;
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
int frc_index = 0;
int opacity = 100;
int penWidth = 1;
std::vector<std::vector<std::string>> FRCOption = {
    {"all","Report all road types."},
    {"1","First class roads, such as national highway network roads." },
    {"2","Second class roads, such as state highway network roads."},
    {"3","Third class roads, such as state interconnecting network roads."},
    {"4","Fourth class roads, such as major connecting roads."},
    {"5","Fifth class roads, such as minor roads connecting suburbs and destination and destination collector roads"}
};

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code

char quadkey[100] = "023010203332";
char UTC_DATE[100] = "NULL";
bool paused = false;
std::vector<Map_Color> res;


void utc_date()
{
    std::time_t now = std::time(0);
    std::tm* now_tm = std::gmtime(&now);
    char buf[42];
    std::strftime(buf, 42, "%Y-%m-%d %X", now_tm);
    strcpy(UTC_DATE, buf);
}



int get_images_thread()
{
    INRIX::get_images(std::string(quadkey).c_str(),FRCOption[frc_index][0].c_str(), std::string(UTC_DATE).c_str(),opacity,penWidth);
    vis::vectorize_image(res, stitched);
    wheel = 1.0;
    deltas = ImVec4(0, 0, 0, 0);
    fetching_data = false;
    return 0;

}
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    AllocConsole();

    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    utc_date();
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("ImGui Example"), NULL };
    ::RegisterClassEx(&wc);
    HWND hwnd = ::CreateWindow(wc.lpszClassName, _T("Dear ImGui DirectX11 Example"),WS_POPUP, 0, 0, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    DWORD window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar;

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    ::ShowWindow(hwnd, SW_HIDE);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    Smallfont = io.Fonts->AddFontFromMemoryCompressedTTF(Medium_compressed_data, Medium_compressed_size, 20.0f);

    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImVec4 clear_color = ImVec4(0.0, 0.0f, 0.0f, 1.0f);

    ImGui::init_style();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    D3DX11_IMAGE_LOAD_INFO info;
    D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rawData,sizeof(rawData), &info,
        NULL, &map, 0);
    
    /*std::vector<Map_Color> res;
    vis::vectorize_image(res);*/
    std::vector<GridData> grid_data;
    //INRIX::get_images(std::string(quadkey).c_str());
    
    //vis::vectorize_image(res,stitched);
    bool show_demo_window = true;
    bool show_another_window = false;
    bool done = false;
    bool open = true;
    while (!done)
    {
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        static float f = 0.0f;
        static int counter = 0;
        ImGui::SetNextWindowSize(ImVec2(1300, 830));
        ImGui::Begin("test", &open, window_flags);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 1.0));
        ImGui::TextCenter("INRIX Hackathon 2021 Traffic Visualizer");
        ImGui::Checkbox("No Traffic",&none);
        ImGui::SameLine();
        ImGui::Checkbox("Light Traffic", &light);
        ImGui::SameLine();
        ImGui::Checkbox("Heavy Traffic",&heavy);
        ImGui::Text("QUADKEY");
        ImGui::SameLine();
        ImGui::AlignTextToFramePadding();
        ImGui::InputText("##QUADKEY", quadkey, sizeof(quadkey), ImGuiInputTextFlags_CharsNoBlank);
        ImGui::SameLine();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 255));
        /*if (ImGui::IsItemActive() || ImGui::IsItemHovered() || ImGui::IsItemToggledSelection())
            ImGui::SetTooltip(("Use DX11 version for default\nUse DX9 version if having issues with DX11").c_str());*/
        if (!fetching_data)
        {
            if (ImGui::Button("Fetch Traffic Tile", ImVec2(200, 24)))
            {
                fetching_data = true;
                CreateThread(0, 0, (LPTHREAD_START_ROUTINE)get_images_thread, 0, 0, 0);
            }
        }
        else {
            ImGui::Text("Fetching data...");
        }
        ImGui::Text("FRCLevel");
        ImGui::SameLine(97);
        ImGui::PushItemWidth(165);
        if (ImGui::BeginCombo("##frc", (FRCOption[frc_index][0].c_str())))
        {
            for (int n = 0; n < FRCOption.size(); n++)
            {
                bool isSelected = (FRCOption[frc_index][0] == FRCOption[n][0]);
                if (ImGui::Selectable(FRCOption[n][0].c_str(), isSelected))
                {
                    frc_index = n;
                }
                if (isSelected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine(324);
        ImGui::Text("Opacity");
        ImGui::SameLine(410);
        ImGui::SliderInt("##opacity",&opacity, 0, 100,"%d%%");
        ImGui::Text("UTC-Date");
        ImGui::SameLine(97);
        ImGui::InputText("##UTCDATE", UTC_DATE, sizeof(UTC_DATE));
        /*ImGui::SameLine();
        if (ImGui::Button("Now", ImVec2(50, 30)))
        {
            utc_date();
        }*/
        ImGui::SameLine(324);
        ImGui::Text("PenWidth");
        ImGui::SameLine();
        ImGui::SliderInt("##penwidth", &penWidth, 1, 6,"%dpx");
        ImGui::PopItemWidth();
        
        ImGui::PopStyleColor();

        ImGui::BeginChild("Map", ImVec2(1200, 650), true);
        if(GetAsyncKeyState(VK_ESCAPE) & WM_KEYUP)
        {
            paused = !paused;
        }
        if (fetching_data)
        {
            ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x/2, ImGui::GetWindowSize().y / 2));
            ImGui::Spinner("Spinner",20.0,4.0, IM_COL32(35, 255, 0, 255));
        }
        else {
            if (paused) {
                ImGui::SetCursorPosY(300);
                ImGui::TextCenter("Paused");
                ImGui::TextCenter("Press 'ESCAPE' to resume");
            }
            else {
                if (res.size() > 0)
                    ImGui::DrawPoints(res);
                else {
                    ImGui::SetCursorPosY(300);
                    ImGui::TextCenter("No Tiles to Display");
                }
            }
        }
        ImGui::EndChild();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 255));
        ImGui::TextCenter("WASD to navigate. Arrow Keys to Zoom");
        ImGui::PopStyleColor(2);
        ImGui::End();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        g_pSwapChain->Present(1, 0);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
