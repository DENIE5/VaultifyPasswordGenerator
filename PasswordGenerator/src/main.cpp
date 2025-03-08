#include <iostream>
#include <windows.h>
#include <conio.h> 
#include "main.h"
#include "..\ext\imgui\imgui.h"
#include "..\ext\imgui\imgui_impl_dx11.h"
#include "..\ext\imgui\imgui_impl_win32.h"
#include <d3d11.h>
#include <tchar.h>
#include <cstdlib>
#include <random>
#include <algorithm>
#include <string>


static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

    //variables
    int passwordLength;
    const char lowercaseLetters[] = "abcdefghijklmnopqrstuvwxyz";
    const char uppercaseLetters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char numbers[] = "0123456789";
    const char symbols[] = "!\"#$%&'()*+,-./:;<=>?@^_`{|}~\\";
    static bool includeLowercase = false;
    static bool includeUppercase = false;
    static bool includeNumbers = false;
    static bool includeSymbols = false;
    std::string currentPassword;
    int totalLength = 3;


    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Vaultify - Password Generator", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    
    colors[ImGuiCol_WindowBg] = ImVec4(5.00f / 255.00f, 33.00f / 255.00f, 77.00f / 255.00f, 0.90f);
    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);

    io.Fonts->AddFontDefault();
    ImFont* mainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 30.0f);
    IM_ASSERT(mainFont != NULL);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        RECT rect;
        GetClientRect(hwnd, &rect);

        {

            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);  // Position at top-left corner
            ImGui::SetNextWindowSize(ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top)), ImGuiCond_Always); // Use the full client area size



            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Vaultify", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);     //ImGuiWindowFlags_NoTitleBar  - removes native imgui titlebar
            
            
            ImGui::PushFont(mainFont);

            ImGui::SetCursorPos(ImVec2(200,137.5));
            ImGui::PushItemWidth(352);
            static int i12 = 0;
            ImGui::Checkbox(" Lowercase Letters", &includeLowercase);
            ImGui::PopItemWidth();

			
            ImGui::SetCursorPos(ImVec2(200,234.5));
            ImGui::PushItemWidth(352);
            static int i14 = 0;
            ImGui::Checkbox(" Uppercase Letters", &includeUppercase);
            ImGui::PopItemWidth();

			
            ImGui::SetCursorPos(ImVec2(200,338.5));
            ImGui::PushItemWidth(352);
            static int i16 = 0;
           ImGui::Checkbox(" Numbers", &includeNumbers);
            
            ImGui::PopItemWidth();

			
            ImGui::SetCursorPos(ImVec2(200,428.5));
            ImGui::PushItemWidth(352);
            static int i19 = 0;
            ImGui::Checkbox(" Symbols", &includeSymbols);
            ImGui::PopItemWidth();

            //assign generated password to currentPassword variable
            ImGui::SetCursorPos(ImVec2(1000,300));
            if (ImGui::Button("Generate Password", ImVec2(400, 56))) {
				currentPassword = generatePassword(includeLowercase, includeUppercase, includeNumbers, includeSymbols,
                lowercaseLetters, uppercaseLetters, numbers, symbols, totalLength);

            }
            ImGui::SetCursorPos(ImVec2(200,500));
            ImGui::PushItemWidth(352);
            ImGui::SliderInt("Password Length", &totalLength, 3, 20);
            ImGui::PopItemWidth();
            ImGui::SetCursorPos(ImVec2(1000,400));

            if (ImGui::Button("Copy to Clipboard", ImVec2(400, 56))) {
                ImGui::SetClipboardText(currentPassword.c_str());
            }

            ImGui::SetCursorPos(ImVec2(1000,200));
            ImGui::Text("Generated Password: %s", currentPassword.c_str()); //display generated password once button is clicked

            ImGui::SetCursorPos(ImVec2(30,950));
            ImGui::Text("Made by Yanis");
            ImGui::PopFont();
            ImGui::End();
        }

        

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);



    return 0;
}

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
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


std::string generatePassword(bool includeLowercase, bool includeUppercase, bool includeNumbers, bool includeSymbols,const char* lowercaseLetters, const char* uppercaseLetters, const char* numbers, const char* symbols, int totalLength) {
    

    std::string password = "";

    // Use a better random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    if(includeLowercase) {
		for (int i = 0; i < totalLength; ++i) {
			password += lowercaseLetters[gen() % 26];
		}
	}
	if(includeUppercase) {
		for (int i = 0; i < totalLength; ++i) {
			password += uppercaseLetters[gen() % 26];
		}
	}
	if(includeNumbers) {
		for (int i = 0; i < totalLength; ++i) {
			password += numbers[gen() % 10];
		}
	}


    for (int i = 0; i < totalLength; ++i) {
        password += symbols[gen() % 30];
    }

    if (password.size() > totalLength) {
        password = password.substr(0, totalLength);
    }


    random_shuffle(password.begin(), password.end());
    return password;
}
