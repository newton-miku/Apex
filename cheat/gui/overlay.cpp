#include "overlay.h"
using namespace std;

extern int aim;
extern bool aim_enable;
extern int aim_key;
extern bool esp;

extern int aSmoothAmount; // Aimbot smoothness
extern int xFOV; //Aimbot horizontal FOV (square)
extern int yFOV; //Aimbot vertical FOV (square)

extern bool player_glow;
extern float playerglow1[4];
extern float playerglow2[4];
extern float playerglow3[4];
extern float playerglow4[4];

extern bool item_glow;
extern int glow_gun_num;
extern int max_check_glow_item_num;
extern bool glow_gun;
extern float gun_glow_col[4];

extern bool glow_supply_gun;

extern bool glow_goldgun;
extern bool glow_suit;
extern bool glow_gunPart;
extern float gold_item_col[4];
extern float red_item_col[4];
extern float purple_item_col[4];
extern float blue_item_col[4];

extern int8_t item_main_glow_type;
extern int8_t item_border_glow_type;
extern float item_glow_distance;

extern int8_t player_main_glow_type;
extern int8_t player_border_glow_type;
extern float player_glow_distance;

extern bool zoom_glow;
extern int zoom_num;
extern float zoom_col[4];
extern bool turbo_glow;
extern bool fast_reload_glow;

extern bool blue_glow;
extern bool heat_shield_glow;
extern bool Shield_Battery_glow;
extern bool respawn_glow;
extern bool blue_backpack_glow;

extern bool aim_no_recoil;
extern bool ready;
extern bool use_nvidia;
extern float max_dist;
extern float smooth;
extern float max_fov;
extern int bone;
extern bool thirdperson;
int width;
int height;
bool k_leftclick = false;
bool k_ins = false;
bool show_menu = false;
visuals v;

extern bool IsKeyDown(int vk);

LONG nv_default = WS_POPUP | WS_CLIPSIBLINGS;
LONG nv_default_in_game = nv_default | WS_DISABLED;
LONG nv_edit = nv_default_in_game | WS_VISIBLE;

LONG nv_ex_default = WS_EX_TOOLWINDOW;
LONG nv_ex_edit = nv_ex_default | WS_EX_LAYERED | WS_EX_TRANSPARENT;
LONG nv_ex_edit_menu = nv_ex_default | WS_EX_TRANSPARENT;



static DWORD WINAPI StaticMessageStart(void* Param)
{
	Overlay* ov = (Overlay*)Param;
	ov->CreateOverlay();
	return 0;
}

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam)
{
	wchar_t className[255] = L"";
	GetClassName(hwnd, className, 255);
	if (use_nvidia)
	{
		if (wcscmp(XorStrW(L"CEF-OSC-WIDGET"), className) == 0) //Nvidia overlay
		{
			HWND* w = (HWND*)lParam;
			if (GetWindowLong(hwnd, GWL_STYLE) != nv_default && GetWindowLong(hwnd, GWL_STYLE) != nv_default_in_game)
				return TRUE;
			*w = hwnd;
			return TRUE;
		}
	}
	else
	{
		if (wcscmp(XorStrW(L"overlay"), className) == 0) //Custom overlay
		{
			HWND* w = (HWND*)lParam;
			*w = hwnd;
			return TRUE;
		}
	}
	return TRUE;
}

// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

void Overlay::RenderMenu()
{

	static bool vis_check = false;
	static bool spec_disable = false;
	static bool all_spec_disable = false;

	/*if (aim > 0)
	{
		aim_enable = true;
		if (aim > 1)
		{
			vis_check = true;
		}
		else
		{
			vis_check = false;
		}
	}
	else
	{
		aim_enable = false;
		vis_check = false;
	}*/
	static ImGuiColorEditFlags alpha_flags = 0;
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(700, 500));
	ImGui::Begin(XorStr(u8"仅供内部使用"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);//| ImGuiWindowFlags_NoScrollbar);
	if (ImGui::BeginTabBar(XorStr("Tab")))
	{
		if (ImGui::BeginTabItem(XorStr(u8"物品发光")))
		{
			ImGui::Checkbox(XorStr("ESP"), &esp);


			/*if (aim_enable)
			{
				ImGui::SameLine();
				ImGui::Checkbox(XorStr("Visibility check"), &vis_check);
				ImGui::SameLine();
				ImGui::Checkbox(XorStr("No recoil/sway"), &aim_no_recoil);
				if (vis_check)
				{
					aim = 2;
				}
				else
				{
					aim = 1;
				}
			}
			else
			{
				aim = 0;
			}*/

			static int glow_item_index = -1;
			ImGui::Checkbox(XorStr(u8"物品发光"), &item_glow);
			static bool output_only_modified = true;

			if (item_glow)
			{
				ImGui::Checkbox(XorStr(u8"空投枪和红甲"), &glow_supply_gun);
				ImGui::Checkbox(XorStr(u8"金枪"), &glow_goldgun);
				ImGui::Checkbox(XorStr(u8"涡轮"), &turbo_glow);
				ImGui::Checkbox(XorStr(u8"加速装填器"), &fast_reload_glow);
				ImGui::Checkbox(XorStr(u8"紫金装备"), &glow_suit);
				ImGui::Checkbox(XorStr(u8"紫金配件"), &glow_gunPart);
				ImGui::Checkbox(XorStr(u8"枪械发光"), &glow_gun);

				if (glow_gun) {
					if (ImGui::Combo(u8"总发光类型", &glow_item_index, u8"敖犬\0 Lstar\0哈沃克\0专注\0三重\0平行\0汗落\0转换者\0 r99\0猎兽\0长弓\0滋崩\0 r301\0 eva8\0莫桑比克\0和平喷\0小帮手\0 p2020\0 re45\0哨兵\0弓\0 3030\0暴走\0 car\0\0"))
					{
						switch (glow_item_index)
						{
						case 0: glow_gun_num = 2; break;//敖犬
						case 1: glow_gun_num = 7; break;//Lstar
						case 2: glow_gun_num = 12; break;//哈沃克
						case 3: glow_gun_num = 17; break;//专注
						case 4: glow_gun_num = 22; break;//三重
						case 5: glow_gun_num = 27; break;//平行
						case 6: glow_gun_num = 32; break;//汗洛
						case 7: glow_gun_num = 42; break;//转换者
						case 8: glow_gun_num = 47; break;//r99
						case 9: glow_gun_num = 52; break;//猎兽
						case 10: glow_gun_num = 58; break;//长弓
						case 11: glow_gun_num = 63; break;//滋崩
						case 12: glow_gun_num = 69; break;//r301
						case 13: glow_gun_num = 74; break;//eva8
						case 14: glow_gun_num = 79; break;//莫桑比克
						case 15: glow_gun_num = 84; break;//和平
						case 16: glow_gun_num = 90; break;//小帮手
						case 17: glow_gun_num = 95; break;//p2020
						case 18: glow_gun_num = 100; break;//re45
						case 19: glow_gun_num = 105; break;//哨兵
						case 20: glow_gun_num = 110; break;//弓
						case 21: glow_gun_num = 115; break;//3030
						case 22: glow_gun_num = 127; break;//暴走
						case 23: glow_gun_num = 132; break;//car
						}
					}
				}
			}

			//ImGui::Checkbox(XorStr("Thirdperson"), &thirdperson);
			ImGui::EndTabItem();
		}
		if (item_glow) {
			if (ImGui::BeginTabItem(XorStr(u8"蓝色物资发光")))
			{
				ImGui::Checkbox(XorStr(u8"蓝色物资"), &blue_glow);
				if (blue_glow) {
					ImGui::Checkbox(XorStr(u8"隔热板"), &heat_shield_glow);
					ImGui::Checkbox(XorStr(u8"移动重生信标"), &respawn_glow);
					ImGui::Checkbox(XorStr(u8"大电"), &Shield_Battery_glow);
					ImGui::Checkbox(XorStr(u8"蓝包"), &blue_backpack_glow);
				}
				ImGui::EndTabItem();
			}
		}

		if (item_glow) {
			if (ImGui::BeginTabItem(XorStr(u8"物品发光设置")))
			{
				ImGui::Text(XorStr(u8"最大检测数目"));
				ImGui::SameLine();
				HelpMarker(u8"将该值调低可能会影响物品发光检测范围，甚至可能检测不到\n但较低的值可降低检测敌人延迟");
				ImGui::SliderInt(XorStr("##3"), &max_check_glow_item_num, 1000, 100000, "%d");
				static int main_glow1 = -1;
				static int border_glow1 = -1;
				ImGui::ColorEdit4(u8"蓝色品质", (float*)blue_item_col, ImGuiColorEditFlags_AlphaBar | alpha_flags);
				ImGui::ColorEdit4(u8"紫色品质", (float*)purple_item_col, ImGuiColorEditFlags_AlphaBar | alpha_flags);
				ImGui::ColorEdit4(u8"金色品质", (float*)gold_item_col, ImGuiColorEditFlags_AlphaBar | alpha_flags);
				ImGui::ColorEdit4(u8"枪械发光颜色", (float*)gun_glow_col, ImGuiColorEditFlags_AlphaBar | alpha_flags);
				ImGui::Text(XorStr(u8"发光距离")); ImGui::SameLine();
				HelpMarker(u8"可调整范围为0-800m,ctrl+鼠标单击即可输入值");
				//ImGui::SliderFloat(XorStr("#dis"), &item_glow_distance, 0.0f, 10000.0f, "%.0fm");//此为滑块调整条，疑似无法手动输入值
				ImGui::DragFloat(XorStr("#dis"), &item_glow_distance, 50.0f, 0.0f, 800.0f, "%.0fm");
				ImGui::Text(u8"物品发光类型设置");
				if (ImGui::Combo(u8"总发光类型", &main_glow1, u8"1\0 2\0 3\0 4\0 5\0 6\0\0"))
				{
					switch (main_glow1)
					{
					case 0: item_main_glow_type = 101; break;
					case 1: item_main_glow_type = 102; break;
					case 2: item_main_glow_type = 103; break;
					case 3: item_main_glow_type = 104; break;
					case 4: item_main_glow_type = 105; break;
					case 5: item_main_glow_type = 106; break;
					}
				}
				if (ImGui::Combo(u8"边界发光类型", &border_glow1, u8"1\0 2\0 3\0 4\0 5\0 6\0\0"))
				{
					switch (border_glow1)
					{
					case 0: item_border_glow_type = 101; break;
					case 1: item_border_glow_type = 102; break;
					case 2: item_border_glow_type = 103; break;
					case 3: item_border_glow_type = 104; break;
					case 4: item_border_glow_type = 105; break;
					case 5: item_border_glow_type = 106; break;
					}
				}
				ImGui::EndTabItem();
			}
		}
		if (item_glow) {
			if (ImGui::BeginTabItem(XorStr(u8"倍镜发光")))
			{
				ImGui::Checkbox(XorStr(u8"倍镜发光"), &zoom_glow);
				if (zoom_glow) {
					ImGui::Combo(u8"总发光类型", &zoom_num, u8"1x\0 2x\0圆1x\0 1-2x\0金1x\0 3x\0 2-4x\0 6x\0 4-8x\0 4-10x\0\0");
					ImGui::ColorEdit4(u8"倍镜发光颜色", (float*)zoom_col, ImGuiColorEditFlags_AlphaBar | alpha_flags);
				}
				ImGui::EndTabItem();
			}
		}

		if (ImGui::BeginTabItem(XorStr("Config")))
		{
			/*ImGui::Text(XorStr("Max distance:"));
			ImGui::SliderFloat(XorStr("##1"), &max_dist, 100.0f * 40, 800.0f * 40, "%.2f");
			ImGui::SameLine();
			ImGui::Text("(%d meters)", (int)(max_dist / 40));*/

			ImGui::Text(XorStr(u8"自瞄平滑度"));
			ImGui::SliderInt(XorStr("##2"), &aSmoothAmount, 1, 40, "%d");

			/*ImGui::Text(XorStr("Max FOV:"));
			ImGui::SliderFloat(XorStr("##3"), &max_fov, 5.0f, 250.0f, "%.2f");*/
			ImGui::Combo(u8"自瞄键", &aim_key, u8"鼠标左右键\0鼠标左键\0鼠标右键\0键盘右Alt\0\0");
			ImGui::Text(XorStr(u8"Aim at (bone id):"));
			ImGui::SliderInt(XorStr("##4"), &bone, 1, 20,"%d");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(XorStr("Visuals")))
		{
			ImGui::Text(XorStr("ESP options:"));
			ImGui::Checkbox(XorStr("Box"), &v.box);
			ImGui::SameLine(0, 70.0f);
			ImGui::Checkbox(XorStr("Name"), &v.name);
			ImGui::Checkbox(XorStr("Line"), &v.line);
			ImGui::Checkbox(XorStr("Distance"), &v.distance);
			ImGui::Checkbox(XorStr("Health bar"), &v.healthbar);
			ImGui::Checkbox(XorStr("Shield bar"), &v.shieldbar);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::Text(XorStr("Overlay FPS: %.3f ms/frame (%.1f FPS)"), 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	if (ImGui::BeginTabBar(XorStr(u8"玩家发光")))
	{
		static int main_glow2 = -1;
		static int border_glow2 = -1;
		ImGui::Checkbox(XorStr(u8"玩家发光"), &player_glow);
		ImGui::Checkbox(XorStr("AIM"), &aim_enable);
		if (aim_enable)
		{
			ImGui::Text(XorStr(u8"自瞄检测窗口大小"));
			ImGui::SameLine(); HelpMarker(u8"可调整范围为50-300");
			ImGui::DragInt(XorStr("FOV"), &xFOV, 10, 10, 300, "%d px");
		}
			if (player_glow)
			{
			ImGui::ColorEdit4(u8"可见敌人颜色", (float*)playerglow1, ImGuiColorEditFlags_AlphaBar | alpha_flags);
			ImGui::ColorEdit4(u8"可见倒地颜色", (float*)playerglow2, ImGuiColorEditFlags_AlphaBar | alpha_flags);
			ImGui::ColorEdit4(u8"不可见敌人颜色", (float*)playerglow3, ImGuiColorEditFlags_AlphaBar | alpha_flags);
			ImGui::ColorEdit4(u8"不可见倒地颜色", (float*)playerglow4, ImGuiColorEditFlags_AlphaBar | alpha_flags);
			ImGui::Text(XorStr(u8"发光距离"));
			ImGui::SameLine(); HelpMarker(u8"可调整范围为0-10000m,ctrl+鼠标单击即可输入值");
			//ImGui::SliderFloat(XorStr("#dis"), &player_glow_distance, 0.0f, 10000.0f, "%.0fm");
			ImGui::DragFloat(XorStr("#dis"), &player_glow_distance, 100.0f, 0.0f, 10000.0f, "%.0fm");
			if (ImGui::Combo(u8"总发光类型", &main_glow2, u8"1\0 2\0 3\0 4\0 5\0 6\0\0"))
			{
				switch (main_glow2)
				{
				case 0: player_main_glow_type = 101; break;
				case 1: player_main_glow_type = 102; break;
				case 2: player_main_glow_type = 103; break;
				case 3: player_main_glow_type = 104; break;
				case 4: player_main_glow_type = 105; break;
				case 5: player_main_glow_type = 106; break;
				}
			}
			if (ImGui::Combo(u8"边界发光类型", &border_glow2, u8"1\0 2\0 3\0 4\0 5\0 6\0\0"))
			{
				switch (border_glow2)
				{
				case 0: player_border_glow_type = 101; break;
				case 1: player_border_glow_type = 102; break;
				case 2: player_border_glow_type = 103; break;
				case 3: player_border_glow_type = 104; break;
				case 4: player_border_glow_type = 105; break;
				case 5: player_border_glow_type = 106; break;
				}
			}
		}
		ImGui::EndTabBar();
	}
	//ImGui::Separator();

	ImGui::End();
}

void Overlay::RenderInfo()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(22, 12));
	ImGui::Begin(XorStr("##info"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
	DrawLine(ImVec2(7, 5), ImVec2(17, 5), RED, 4);
	ImGui::End();
}

void Overlay::ClickThrough(bool v)
{
	if (v)
	{
		nv_edit = nv_default_in_game | WS_VISIBLE;
		if (GetWindowLong(overlayHWND, GWL_EXSTYLE) != nv_ex_edit)
			SetWindowLong(overlayHWND, GWL_EXSTYLE, nv_ex_edit);
	}
	else
	{
		nv_edit = nv_default | WS_VISIBLE;
		if (GetWindowLong(overlayHWND, GWL_EXSTYLE) != nv_ex_edit_menu)
			SetWindowLong(overlayHWND, GWL_EXSTYLE, nv_ex_edit_menu);
	}
}

DWORD Overlay::CreateOverlay()
{
	EnumWindows(EnumWindowsCallback, (LPARAM)&overlayHWND);
	Sleep(300);
	if (overlayHWND == 0)
	{
		printf(XorStr("Can't find the overlay\n"));
		Sleep(1000);
		exit(0);
	}

	HDC hDC = ::GetWindowDC(NULL);
	width = ::GetDeviceCaps(hDC, HORZRES);
	height = ::GetDeviceCaps(hDC, VERTRES);

	running = true;

	// Initialize Direct3D
	if (!CreateDeviceD3D(overlayHWND))
	{
		CleanupDeviceD3D();
		return 1;
	}

	// Show the window
	::ShowWindow(overlayHWND, SW_SHOWDEFAULT);
	::UpdateWindow(overlayHWND);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromMemoryTTF((void*)font_data, font_size, 17.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());//   GetGlyphRangesChineseSimplifiedCommon());
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	ImGui::GetStyle().WindowMinSize = ImVec2(1, 1);

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(overlayHWND);
	ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ClickThrough(true);
	while (running)
	{
		HWND wnd = GetWindow(GetForegroundWindow(), GW_HWNDPREV);
		if (use_nvidia)
		{
			if (GetWindowLong(overlayHWND, GWL_STYLE) != nv_edit)
				SetWindowLong(overlayHWND, GWL_STYLE, nv_edit);
			if (show_menu)
			{
				ClickThrough(false);
			}
			else
			{
				if (GetWindowLong(overlayHWND, GWL_EXSTYLE) != nv_ex_edit)
					SetWindowLong(overlayHWND, GWL_EXSTYLE, nv_ex_edit);
				ClickThrough(true);
			}
		}
		if (wnd != overlayHWND)
		{
			SetWindowPos(overlayHWND, wnd, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE);
			::UpdateWindow(overlayHWND);
		}

		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (IsKeyDown(VK_LBUTTON) && !k_leftclick)
		{
			io.MouseDown[0] = true;
			k_leftclick = true;
		}
		else if (!IsKeyDown(VK_LBUTTON) && k_leftclick)
		{
			io.MouseDown[0] = false;
			k_leftclick = false;
		}

		if (IsKeyDown(VK_INSERT) && !k_ins && ready)
		{
			show_menu = !show_menu;
			ClickThrough(!show_menu);
			k_ins = true;
		}
		else if (!IsKeyDown(VK_INSERT) && k_ins)
		{
			k_ins = false;
		}

		if (show_menu)
			RenderMenu();
		else
			RenderInfo();

		RenderEsp();

		// Rendering
		ImGui::EndFrame();
		ImGui::Render();
		const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
		g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		g_pSwapChain->Present(1, 0); // Present with vsync

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	ClickThrough(true);

	CleanupDeviceD3D();
	::DestroyWindow(overlayHWND);
	return 0;
}

void Overlay::Start()
{
	DWORD ThreadID;
	CreateThread(NULL, 0, StaticMessageStart, (void*)this, 0, &ThreadID);
}

void Overlay::Clear()
{
	running = 0;
	Sleep(50);
	if (use_nvidia)
	{
		SetWindowLong(overlayHWND, GWL_STYLE, nv_default);
		SetWindowLong(overlayHWND, GWL_EXSTYLE, nv_ex_default);
	}
}

int Overlay::getWidth()
{
	return width;
}

int Overlay::getHeight()
{
	return height;
}

// Helper functions

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
	{
		g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
		pBackBuffer->Release();
	}
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
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void Overlay::DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
{
	ImGui::GetWindowDrawList()->AddLine(a, b, color, width);
}

void Overlay::DrawBox(ImColor color, float x, float y, float w, float h)//input the position of left bottom corner position
{
	DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.0f);//bottom
	DrawLine(ImVec2(x, y), ImVec2(x, y + h), color, 1.0f);//left
	DrawLine(ImVec2(x + w, y), ImVec2(x + w, y + h), color, 1.0f);//right
	DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.0f);//top
}

void Overlay::Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
}

void Overlay::String(ImVec2 pos, ImColor color, const char* text)
{
	Text(pos, color, text, text + strlen(text), 200, 0);
}

void Overlay::RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
{
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

void Overlay::ProgressBar(float x, float y, float w, float h, int value, int v_max)
{
	ImColor barColor = ImColor(
		min(510 * (v_max - value) / 100, 255),
		min(510 * value / 100, 255),
		25,
		255
	);

	RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), barColor, 0.0f, 0);
}