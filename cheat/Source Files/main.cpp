#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <string_view>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <random>

#include "../Tools/xor.hpp"
#include "../Tools/Tools.h"
#include "../Header Files/offsets.h"
#include "../Header Files/Driver.h"
#include "../ShowHealth/DirectX.h"
#include "../ShowHealth/overlay.h"
#include "../ShowHealth/utils.h"
#include "../gui/gui.h"

int screenWeight = 1920; // In-game resolution
int screenHeight = 1080;
int xFOV = 300; //Aimbot horizontal FOV (square)
int yFOV = 300; //Aimbot vertical FOV (square)
int aSmoothAmount = 2; // Aimbot smoothness

uintptr_t localPlayer;
uintptr_t entList;
uintptr_t viewRenderer;
uintptr_t viewMatrix;
typedef struct player
{
	float dist = 0;
	int entity_team = 0;
	float boxMiddle = 0;
	float h_y = 0;
	float width = 0;
	float height = 0;
	float b_x = 0;
	float b_y = 0;
	bool knocked = false;
	bool visible = false;
	int health = 0;
	int shield = 0;
	char name[33] = { 0 };
}player;

uint32_t check = 0xABCD;

int aim_key = VK_RBUTTON;
bool use_nvidia = false;
bool active = true;
bool ready = false;
extern visuals v;
int aim = 0; //read
bool esp = false; //read
bool item_glow = false;
bool glow_goldgun = false;
bool glow_suit = false;
bool glow_gunPart = false;

bool player_glow = false;
float playerglow1[4] = { 0.0f, 0.837104f, 0.056f, 0.9f };//可见敌人
float playerglow2[4] = { 0.0f, 0.0f, 1.0f, 0.9f };//可见倒地敌人
float playerglow3[4] = { 0.914416f, 0.004525f, 1.0f, 0.9f };//不可见敌人
float playerglow4[4] = { 0.0f, 0.0f, 1.0f, 0.9f };//不可见倒地敌人
float gold_item_col[4] = { 0.94f, 1.0f, 0.0f, 0.9f };//金色
float red_item_col[4] = { 1.0f, 0.0f, 0.0f, 0.9f };//红色
float purple_item_col[4] = { 0.588235f, 0.0f, 1.0f, 0.9f };//紫色
float zoom_col[4] = { 0.0f, 0.986425f, 1.0f, 0.9f };//瞄准镜颜色
float gun_glow_col[4] = { 1.0f, 0.607f, 0.0f, 0.9f };//枪械发光

bool aim_no_recoil = false;
bool aiming = false; //read
uint64_t g_Base = 0; //write
float max_dist = 200.0f * 40.0f; //read
float smooth = 12.0f;
float max_fov = 15.0f;
int bone = 2;
int glow_num = -1;
bool thirdperson = false;

bool valid = true; //write
bool next = true; //read write

uint64_t add[16];

bool k_f5 = 0;
bool k_f6 = 0;
bool k_f8 = 0;

bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

player players[100];

void Overlay::RenderEsp()
{
	next = false;
	if (g_Base != 0 && esp)
	{
		memset(players, 0, sizeof(players));
		while (!next && esp)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		if (next && valid)
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2((float)getWidth(), (float)getHeight()));
			ImGui::Begin(XorStr("##esp"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);

			for (int i = 0; i < 100; i++)
			{
				if (players[i].health > 0)
				{
					std::string distance = std::to_string(players[i].dist / 39.62);
					distance = distance.substr(0, distance.find('.')) + "m(" + std::to_string(players[i].entity_team) + ")";
					if (v.box)
					{
						if (players[i].visible)
						{
							if (players[i].dist < 1600.0f)
								DrawBox(RED, players[i].boxMiddle, players[i].h_y, players[i].width, players[i].height); //BOX
							else
								DrawBox(ORANGE, players[i].boxMiddle, players[i].h_y, players[i].width, players[i].height); //BOX
						}
						else
						{
							DrawBox(WHITE, players[i].boxMiddle, players[i].h_y, players[i].width, players[i].height); //white if player not visible
						}
					}

					if (v.line)
						DrawLine(ImVec2((float)(getWidth() / 2), (float)getHeight()), ImVec2(players[i].b_x, players[i].b_y), BLUE, 1); //LINE FROM MIDDLE SCREEN

					if (v.distance)
					{
						if (players[i].knocked)
							String(ImVec2(players[i].boxMiddle, (players[i].b_y + 1)), RED, distance.c_str());  //DISTANCE
						else
							String(ImVec2(players[i].boxMiddle, (players[i].b_y + 1)), GREEN, distance.c_str());  //DISTANCE
					}

					if (v.healthbar)
						ProgressBar((players[i].b_x - (players[i].width / 2.0f) - 4), (players[i].b_y - players[i].height), 3, players[i].height, players[i].health, 100); //health bar
					if (v.shieldbar)
						ProgressBar((players[i].b_x + (players[i].width / 2.0f) + 1), (players[i].b_y - players[i].height), 3, players[i].height, players[i].shield, 125); //shield bar

					if (v.name)
						String(ImVec2(players[i].boxMiddle, (players[i].b_y - players[i].height - 15)), WHITE, players[i].name);
				}
			}

			ImGui::End();
		}
	}
}

struct GlowMode
{
	int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
};

DWORD64 GetEntityById(int Ent, DWORD64 Base)
{
	DWORD64 EntityList = Base + OFFSET_ENTITYLIST; //updated
	DWORD64 BaseEntity = read<DWORD64>(EntityList);
	if (!BaseEntity)
		return NULL;
	return  read<DWORD64>(EntityList + (Ent << 5));
}

int crosshairX = screenWeight / 2;
int crosshairY = screenHeight / 2;

int entX = 0;
int entY = 0;

int closestX = 0;
int closestY = 0;

int aX = 0;
int aY = 0;

float entNewVisTime = 0;
float entOldVisTime[100];
int visCooldownTime[100];

struct Vector3 {
	float x, y, z;
};

struct Matrix {
	float matrix[16];
};

struct Vector3 _WorldToScreen(const struct Vector3 pos, struct Matrix matrix) {
	struct Vector3 out;
	float _x = matrix.matrix[0] * pos.x + matrix.matrix[1] * pos.y + matrix.matrix[2] * pos.z + matrix.matrix[3];
	float _y = matrix.matrix[4] * pos.x + matrix.matrix[5] * pos.y + matrix.matrix[6] * pos.z + matrix.matrix[7];
	out.z = matrix.matrix[12] * pos.x + matrix.matrix[13] * pos.y + matrix.matrix[14] * pos.z + matrix.matrix[15];

	_x *= 1.f / out.z;
	_y *= 1.f / out.z;

	int width = screenWeight;
	int height = screenHeight;

	out.x = width * .5f;
	out.y = height * .5f;

	out.x += 0.5f * _x * width + 0.5f;
	out.y -= 0.5f * _y * height + 0.5f;

	return out;
}

uintptr_t GetEntityBoneArray(uintptr_t ent)
{
	return read<uintptr_t>(ent + OFFSET_BONES);
}

Vector3 GetEntityBonePosition(uintptr_t ent, uint32_t BoneId, Vector3 BasePosition)
{
	unsigned long long pBoneArray = GetEntityBoneArray(ent);

	Vector3 EntityHead = Vector3();

	EntityHead.x = read<float>(pBoneArray + 0xCC + (BoneId * 0x30)) + BasePosition.x;
	EntityHead.y = read<float>(pBoneArray + 0xDC + (BoneId * 0x30)) + BasePosition.y;
	EntityHead.z = read<float>(pBoneArray + 0xEC + (BoneId * 0x30)) + BasePosition.z;

	return EntityHead;
}

Vector3 GetEntityBasePosition(uintptr_t ent)
{
	return read<Vector3>(ent + OFFSET_ORIGIN);
}

static bool mouse_move(int x, int y) {
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(input));
	return true;
}
int gui(/*int argc, char** argv*/)
{
	add[0] = (uintptr_t)&check;
	add[1] = (uintptr_t)&aim;
	add[2] = (uintptr_t)&esp;
	add[3] = (uintptr_t)&aiming;
	add[4] = (uintptr_t)&g_Base;
	add[5] = (uintptr_t)&next;
	add[6] = (uintptr_t)&players[0];
	add[7] = (uintptr_t)&valid;
	add[8] = (uintptr_t)&max_dist;
	add[9] = (uintptr_t)&item_glow;
	add[10] = (uintptr_t)&player_glow;
	add[11] = (uintptr_t)&aim_no_recoil;
	add[12] = (uintptr_t)&smooth;
	add[13] = (uintptr_t)&max_fov;
	add[14] = (uintptr_t)&bone;
	add[15] = (uintptr_t)&thirdperson;
	printf(XorStr("add offset: 0x%I64x\n"), (uint64_t)&add[0] - (uint64_t)GetModuleHandle(NULL));

	Overlay ov1 = Overlay();
	ov1.Start();
	printf(XorStr("Waiting for host process...\n"));
	while (check != 0xABCD)
	{
		if (IsKeyDown(VK_F4))
		{
			active = false;
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	if (active)
	{
		ready = true;
		printf(XorStr("Ready\n"));
	}

	while (active)
	{
		//printf("%f\t%f\t%f\n", col2[0], col2[1], col2[2]);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (IsKeyDown(VK_F4))
		{
			active = false;
		}

		if (IsKeyDown(VK_F5) && k_f5 == 0)
		{
			k_f5 = 1;
			esp = !esp;
		}
		else if (!IsKeyDown(VK_F5) && k_f5 == 1)
		{
			k_f5 = 0;
		}

		if (IsKeyDown(VK_F6) && k_f6 == 0)
		{
			k_f6 = 1;
			switch (aim)
			{
			case 0:
				aim = 1;
				break;
			case 1:
				aim = 2;
				break;
			case 2:
				aim = 0;
				break;
			default:
				break;
			}
		}
		else if (!IsKeyDown(VK_F6) && k_f6 == 1)
		{
			k_f6 = 0;
		}

		if (IsKeyDown(VK_F8) && k_f8 == 0)
		{
			k_f8 = 1;
			item_glow = !item_glow;
		}
		else if (!IsKeyDown(VK_F8) && k_f8 == 1)
		{
			k_f8 = 0;
		}

		if (IsKeyDown(VK_LEFT))
		{
			if (max_dist > 100.0f * 40.0f)
				max_dist -= 50.0f * 40.0f;
			std::this_thread::sleep_for(std::chrono::milliseconds(130));
		}

		if (IsKeyDown(VK_RIGHT))
		{
			if (max_dist < 800.0f * 40.0f)
				max_dist += 50.0f * 40.0f;
			std::this_thread::sleep_for(std::chrono::milliseconds(130));
		}

		if (IsKeyDown(aim_key))
			aiming = true;
		else
			aiming = false;
	}
	ready = false;
	ov1.Clear();
	if (!use_nvidia)
		//system(XorStr("taskkill /F /T /IM overlay.exe"));
		system(XorStr("taskkill /F /T /IM overlay_ap.exe")); //custom overlay process name
	return 0;
}

int main(int argCount, char** argVector)
{
	srand(time(NULL));
	std::string filePath = argVector[0];
	RenameFile(filePath);
	Overlay ov1 = Overlay();
	Direct dx = ov1.CurrentDirectX;

	while (!hwnd)
	{
		hwnd = FindWindowA(NULL, ("Apex Legends"));
		Sleep(500);
	}

	while (!oPID) // get the process id
	{
		oPID = GetPID("r5apex.exe");
		printf(" [+] Driver Loader\n [+] Status Apex:Detected\n [+] Contact Rakuza#7789");
		Sleep(500);
	}

	while (!oBaseAddress) // request the module base from driver
	{
		oBaseAddress = GetModuleBaseAddress(oPID, "r5apex.exe");
		Sleep(500);
	}
	
	while (true)
	{
		// Matrix set up
		uint64_t viewRenderer = read<uint64_t>(oBaseAddress + OFFSET_RENDER);
		uint64_t viewMatrix = read<uint64_t>(viewRenderer + OFFSET_MATRIX);
		Matrix m = read<Matrix>(viewMatrix);

		// Local player set up
		uintptr_t locPlayer = read<uintptr_t>(oBaseAddress + OFFSET_LOCAL_ENT);

		// Before entity loop starts
		int closestX = 9999;
		int closestY = 9999;

		// Entity loop starts here
		for (int i = 0; i < 64; i++)
		{
			DWORD64 Entity = GetEntityById(i, oBaseAddress);
			if (Entity == 0)
				continue;
			DWORD64 EntityHandle = read<DWORD64>(Entity + OFFSET_NAME);
			std::string Identifier = read<std::string>(EntityHandle);
			LPCSTR IdentifierC = Identifier.c_str();
			if (strcmp(IdentifierC, "player"))
			{

				Vector3 HeadPosition = GetEntityBonePosition(Entity, 8, GetEntityBasePosition(Entity));
				// Convert to screen position

				Vector3 w2sHeadAimPos = _WorldToScreen(HeadPosition, m);

				// Get screen position
				int entX = w2sHeadAimPos.x;
				int entY = w2sHeadAimPos.y;

				// Get entity total visible time
				entNewVisTime = read<float>(Entity + OFFSET_VISIBLE_TIME);

				// Get entity knocked state
				int entKnockedState = read<int>(Entity + OFFSET_BLEED_OUT_STATE);

				// Get player team ID
				int playerTeamID = read<int>(locPlayer + OFFSET_TEAM);

				// Get entity team ID
				int entTeamID = read<int>(Entity + OFFSET_TEAM);

				//Get player health
				float health = read<int>(Entity + OFFSET_HEALTH);

				//Get player shield
				float shield = read<int>(Entity + OFFSET_SHIELD);

				// Is it an enemy
				if (entTeamID != playerTeamID)
				{
					write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
					write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 2); // glow through walls: 2 = enabled, 5 = disabled
					write<GlowMode>(Entity + GLOW_TYPE, { 101,101,46,90 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;

					std::wstring health1 = std::to_wstring(health);
					health1 = L"血：" + health1.substr(0, health1.find('.')) ;

					std::wstring shield1 = std::to_wstring(shield);
					shield1 = L"护甲：" + shield1.substr(0, shield1.find('.'));

					//dx.Fill(hs.x - 2.5f, hs.y, 5, 5, 0, 0, 255, 255); //HEAD
					//dx.DrawBox2(boxMiddle, hs.y, width, height, 255, 0, 0, 255); //BOX
					dx.DrawString(HeadPosition.x, HeadPosition.y + 1, 255, 0, 255, 0, health1.c_str());  //Health
					dx.DrawString(HeadPosition.x, HeadPosition.y + 2, 255, 0, 255, 0, shield1.c_str());  //Health
					//dx.DrawLine(ov->getWidth() / 2, ov->getHeight(), bs.x, bs.y, 255, 0, 0, 255); //LINE FROM MIDDLE SCREEN

					// Is visible
					if (entNewVisTime != entOldVisTime[i])
					{
						visCooldownTime[i] = 0; // low values mean less latency, increase if you observe the color changes on visible enemies

						//visible enemie color
						if (entKnockedState == 0)
						{
							write<float>(Entity + 0x1D0, 0); // red color/brightness of visible enemies
							write<float>(Entity + 0x1D4, 61); // green							
							write<float>(Entity + 0x1D8, 0); // blue

							// Aimbot fov
							if (abs(crosshairX - entX) < abs(crosshairX - closestX) && abs(crosshairX - entX) < xFOV && abs(crosshairY - entY) < abs(crosshairY - closestY) && abs(crosshairY - entY) < yFOV)
							{
								// Aimbot find closest target
								closestX = entX;
								closestY = entY;
							}
						}
						else
						{
							//visible enemie down color
							write<float>(Entity + 0x1D0, 0); // r color/brightness of knocked enemies
							write<float>(Entity + 0x1D4, 0); // g 
							write<float>(Entity + 0x1D8, 255); // b
						}

						entOldVisTime[i] = entNewVisTime;
					}
					else
					{
						if (visCooldownTime[i] <= 0)
						{
							//unvisible enemie color
							if (entKnockedState == 0)
							{
								write<float>(Entity + 0x1D0, 61); // r color/brightness of not visible enemies
								write<float>(Entity + 0x1D4, 0);  // g
								write<float>(Entity + 0x1D8, 0); // b
							}
							else
							{
								//unvisible enemie down color
								write<float>(Entity + 0x1D0, 0); // r color/brightness of knocked enemies
								write<float>(Entity + 0x1D4, 0); // g 
								write<float>(Entity + 0x1D8, 255); // b
							}
						}
					}

					if (visCooldownTime[i] >= 0) visCooldownTime[i] -= 1;
				}
			}
		}
		// After entity loop ends
		if (closestX != 9999 && closestY != 9999)
		{
		//	 If aimbot key pressed
			if (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON))
			{
		//		 If mouse cursor shown
				CURSORINFO ci = { sizeof(CURSORINFO) };
				if (GetCursorInfo(&ci))
				{
					if (ci.flags == 0){
						aX = (closestX - crosshairX) / aSmoothAmount;
						aY = (closestY - crosshairY) / aSmoothAmount;
					}
					mouse_move((int)aX, (int)(aY));
					//mouse_event(MOUSEEVENTF_MOVE, aX, aY, 0, 0); // enable aimbot when mouse cursor is hidden
				}
			}
		}
		//Sleep(100);
	}
}
