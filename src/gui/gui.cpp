#include "gui.h"

#include "../../ext/imgui/imgui.h"
#include "../../ext/imgui/imgui_impl_win32.h"
#include "../../ext/imgui/imgui_impl_dx9.h"
#include "../func/vars.h"
#include <stdexcept>
#include "../func/bunnyhop.h"
// #include "../func/bunnyhop.cpp"



extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

// window process
LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);


bool gui::SetupWindowClass(const char* windowClassName) noexcept
{
	// populate window class
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = NULL;

	// register
	if (!RegisterClassEx(&windowClass))
		return false;

	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(
		windowClass.lpszClassName,
		windowClass.hInstance
	);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	// create temp window
	window = CreateWindow(
		windowClass.lpszClassName,
		windowName,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		100,
		100,
		0,
		0,
		windowClass.hInstance,
		0
	);

	if (!window)
		return false;
	return true;
}

void gui::DestroyWindow() noexcept
{
	if (window)
		DestroyWindow(window);
}

bool gui::SetupDirectX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");

	if (!handle)
		return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
		handle,
		"Direct3DCreate9"
	));

	if (!create)
		return false;

	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9)
		return false;

	D3DPRESENT_PARAMETERS params = { };
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleType = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) return false;

	return true;
}

void gui::DestroyDirectX() noexcept
{
	if (device)
	{
		device->Release();
		device = NULL;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = NULL;
	}
}

void gui::Setup()
{
	if (!SetupWindowClass("hackClass001"))
		throw std::runtime_error("Failed to create window class.");

	if (!SetupWindow("Hack Window"))
		throw std::runtime_error("Failed to create window.");

	if (!SetupDirectX())
		throw std::runtime_error("Failed to create device.");

	DestroyWindow();
	DestroyWindowClass();
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{ };
	device->GetCreationParameters(&params);

	window = params.hFocusWindow;

	originalWindowProcess = reinterpret_cast<WNDPROC>(
		SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
		);
	
	ImGui::CreateContext();
	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/Verdana.ttf", 17.0f);
	io.FontDefault = io.Fonts->Fonts.back();
}

void gui::Destroy() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(
		window,
		GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(originalWindowProcess)
	);

	DestroyDirectX();
}

//Menu

class initWindow {
public:
	ImVec2 window_size{ 870, 500 };

	DWORD window_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize; // 
} iw;

static int tabb = 0;
const float button_width = 130.0f;
const float button_height = 45.0f;
const float spacing = 15.0f;



; // устанавливаем новый шрифт для всего меню



void gui::Render() noexcept
{


	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuiStyle& style = ImGui::GetStyle();
	

	// Background color
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.92f, 0.95f, 0.98f, 1.0f));
	// Header color
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.30f, 0.53f, 0.87f, 0.33f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.30f, 0.53f, 0.87f, 0.70f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.30f, 0.53f, 0.87f, 1.0f));

	// Button color
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.00f)); // белый цвет текста
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.34f, 0.51f, 1.17f, 1.00f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.36f, 0.82f, 1.00f)); // при наведении
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.24f, 0.36f, 0.82f, 1.00f)); // когда нажимаешь


	// Text color for disabled controls
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.60f, 0.60f, 0.60f, 1.0f));

	// Separator color
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, 0.0f, 0.0f, 0.10f));

	// Active/inactive window title color
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.30f, 0.53f, 0.87f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.30f, 0.53f, 0.87f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.30f, 0.53f, 0.87f, 0.75f));

	// Window border color
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.26f, 0.47f, 0.76f, 0.29f));
	ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(1.00f, 1.00f, 1.00f, 0.10f));

	// Popup background color
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.00f, 1.00f, 1.00f, 1.0f));

	// main
	style.WindowPadding = ImVec2(11, 13);
	style.FramePadding = ImVec2(11, 5);
	style.CellPadding = ImVec2(7, 7);
	style.ItemSpacing = ImVec2(15, 0);
	style.ItemInnerSpacing = ImVec2(20, 7);
	style.TouchExtraPadding = ImVec2(4, 0);
	style.IndentSpacing = 14;
	style.ScrollbarSize = 14;
	style.GrabMinSize = 20;
	style.ItemSpacing = ImVec2(13, 13);

	//borders
	style.WindowBorderSize = 0;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FramePadding = ImVec2(0, 0);
	style.TabBorderSize = 0;

	//rounding
	style.WindowRounding = 9;
	style.ChildRounding = 2;
	style.FramePadding = ImVec2(8, 8);
	style.PopupRounding = 5;
	style.ScrollbarRounding = 12;
	style.GrabRounding = 4;
	style.LogSliderDeadzone = 5;
	style.TabRounding = 10;
	style.FrameRounding = 4.0f;

	//aligment
	style.WindowTitleAlign = ImVec2(0.50f, 0.50f);
	style.WindowMenuButtonPosition = 0;
	style.ButtonTextAlign = ImVec2(0.50f, 0.50f);

	ImGui::SetNextWindowSize(iw.window_size);
	ImGui::SetNextWindowBgAlpha(1.0f);
	ImGui::Begin("kiraz internal", &open, iw.window_flags);
	{
		if (ImGui::Button("Aimbot", ImVec2(button_width, button_height))) {
			tabb = 0;
		}
		ImGui::SameLine(0, spacing);

		if (ImGui::Button("Trigerbot", ImVec2(button_width, button_height))) {
			tabb = 1;
		}
		ImGui::SameLine(0, spacing);

		if (ImGui::Button("Radar", ImVec2(button_width, button_height))) {
			tabb = 2;
		}
		ImGui::SameLine(0, spacing);

		if (ImGui::Button("Visuals", ImVec2(button_width, button_height))) {
			tabb = 3;
		}
		ImGui::SameLine(0, spacing);

		if (ImGui::Button("Misc", ImVec2(button_width, button_height))) {
			tabb = 4;
		}
		ImGui::SameLine(0, spacing);

		if (ImGui::Button("Settings", ImVec2(button_width, button_height))) {
			tabb = 5;
		}

		switch (tabb) {
			case 0:
				break;		
			case 1:
				ImGui::Text("Triggerbot");
				break;
			case 2:
				ImGui::Text("Radar");
				break;
			case 3:
				ImGui::Text("Visuals");
				break;
			case 4:
				ImGui::Checkbox("Bunnyhop", &Functional::Misc::bunnyhop);
				break;
			case 5:
				ImGui::Text("Settings");

				break;
		}
	}
	ImGui::End();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
)
{
	// toggle menu
	if (GetAsyncKeyState(VK_INSERT) & 1)
		gui::open = !gui::open;

	// pass messages to imgui
	if (gui::open && ImGui_ImplWin32_WndProcHandler(
		window,
		message,
		wideParam,
		longParam
	)) return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}
