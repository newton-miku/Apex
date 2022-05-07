#include <iostream>
#include "../verify.c"
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
#include <thread>
#include"stdio.h"

#include "../Tools/xor.hpp"
#include "../Tools/Tools.h"
#include "../Header Files/offsets.h"
#include "../Header Files/Driver.h"
#include "../gui/gui.h"
#include "../Header Files/ini.cpp"


int screenWeight = 1920; // In-game resolution
int screenHeight = 1080;
int xFOV = 70; //Aimbot horizontal FOV (square) //自瞄窗口大小
int yFOV = 70; //Aimbot vertical FOV (square)
int aSmoothAmount = 12; // Aimbot smoothness //自瞄平滑度

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
	int dead = 0;
	int max_health = 0;
	int shield = 0;
	int max_shield = 0;
	int Observer = 0;
	char name[64] = { 0 };
}player;

uint32_t check = 0xABCD;

bool show_text_menu = false;//显示文字菜单
bool show_watcher = false;//显示观众

int aim_key = 0;
bool use_nvidia = true;
bool active = true;
bool ready = false;
extern visuals v;
int aim = 0; //read
bool aim_enable = false;

bool esp = false; //read
float esp_max_dist = 800.1f;//ESP最大显示距离
float esp_font_size = 1.0f;//ESP字体大小

float text_menu_font_size = 1.0f;//文字菜单字体大小

int max_check_glow_item_num = 10000;//物品发光最大遍历数
bool item_glow = false;//物品发光
bool item_glow_ok = true;//物品发光进程是否完成
bool turbo_glow = false;//涡轮
bool fast_reload_glow = false;//加速装填器
bool glow_goldgun = false;//金枪
bool glow_supply_gun = false;//空投枪和红甲
bool glow_suit = false;//装备
bool glow_gunPart = false;//配件
float gold_item_col[4] = { 0.94f, 1.0f, 0.0f, 0.9f };//金色
float red_item_col[4] = { 1.0f, 0.0f, 0.0f, 0.9f };//红色
float purple_item_col[4] = { 0.588235f, 0.0f, 1.0f, 0.9f };//紫色
float blue_item_col[4] = { 0.2805f, 0.7558f, 1.0f, 0.9f };//蓝色

float gun_glow_col[4] = { 1.0f, 0.607f, 0.0f, 0.9f };//枪械发光

int item_main_glow_type = 1;//物品主发光类型
int item_border_glow_type = 1;//物品发光边界类型
float item_glow_distance = 200.0f;//物品发光距离

int player_main_glow_type = 1;//玩家发光主类型
int player_border_glow_type = 1;//玩家发光边界类型
int8_t BorderSize_c = 46;//玩家发光边缘尺寸
int8_t TransparentLevel_c = 80;//玩家发光透明度
int BorderSize = 46;//玩家发光边界尺寸
int TransparentLevel = 80;//玩家发光透明度
float player_glow_distance = 500.0f;//玩家发光距离

bool player_glow = true;//玩家发光
float playerglow1[4] = { 0.0f, 0.837104f, 0.056f, 0.9f };//可见敌人
float playerglow2[4] = { 0.0f, 0.0f, 1.0f, 0.9f };//可见倒地敌人
float playerglow3[4] = { 0.914416f, 0.004525f, 1.0f, 0.9f };//不可见敌人
float playerglow4[4] = { 0.0f, 0.0f, 1.0f, 0.9f };//不可见倒地敌人

bool zoom_glow = false;//瞄准镜发光
int zoom_num = -1;
float zoom_col[4] = { 1.0f, 1.0f, 1.0f, 0.9f };//瞄准镜发光颜色

bool blue_glow = false;//蓝色物资发光
bool heat_shield_glow = false;//隔热板
bool Shield_Battery_glow = false;//大电
bool respawn_glow = false;//重生信标
bool blue_backpack_glow = false;//蓝包

bool aim_no_recoil = false;
bool aiming = false; //read
uint64_t g_Base = 0; //write
float max_dist = 200.0f * 40.0f; //read
float smooth = 12.0f;
float max_fov = 15.0f;
int bone = 3;//head - 8,chest - 3     2/5?

bool glow_gun = false;
int glow_gun_num = -1;
bool thirdperson = false;
bool tmp_third_person = false;
bool fakeduck = false;
bool keepjump = false;

bool valid = false; //write
bool next = true; //read write

uint64_t add[16];

bool k_f5 = 0;
bool k_f6 = 0;
bool k_f8 = 0;

uintptr_t init_main() {
	while (!hwnd)
	{
		hwnd = FindWindowA(NULL, ("Apex Legends"));
		Sleep(500);
	}

	while (!oPID) // get the process id
	{
		oPID = GetPID("r5apex.exe");
		Sleep(500);
	}

	while (!oBaseAddress) // request the module base from driver
	{
		oBaseAddress = GetModuleBaseAddress(oPID, "r5apex.exe");
		Sleep(500);
	}
	printf("进程id：%d\n", oPID);
	printf("基址：%d\n", oBaseAddress);
	printf(/*" [+] Driver Loader\n [+] Status Apex:Detected\n */"[+] Contact newton_miku\n [+]啊，哈哈哈哈\n [+]寄汤来喽\n");
	return oBaseAddress;
}

bool IsKeyDown(int vk)
{
	return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

player players[100];

std::string get_aim_key();


void Overlay::RenderEsp()
{
	next = false;
	
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)getWidth(), (float)getHeight()));
		ImGui::Begin(XorStr("Text_Menu"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
		if (show_text_menu)
		{
			std::string show_menu = XorStr(u8"显示文字菜单—[HOME]");
			std::string menu0 = XorStr(u8"方框透视 - [F5]");
			std::string menu1 = XorStr(u8"敌人发光 - [F6]");
			std::string menu2 = XorStr(u8"物品发光 - [F7]");
			std::string menu3 = XorStr(u8"观众名单 - [F9]");
			std::string menu5 = XorStr(u8"开关假蹲 - [F10]");
			std::string menu4 = XorStr(u8"开关自瞄 - [F8]");
			std::string menu4_1 = XorStr(u8"自瞄平滑度: ") + std::to_string(aSmoothAmount) + XorStr(u8" — [<-] [->]");
			std::string menu4_2 = XorStr(u8"自瞄键： ") + get_aim_key() + XorStr(u8" — [Shift + <-] [Shift + ->]");

			String(ImVec2(200, 290), GREEN, show_menu.c_str(), text_menu_font_size);
			if (!esp) {
				String(ImVec2(200, 290 + 10 * text_menu_font_size), RED, menu0.c_str(), text_menu_font_size);

			}
			else
			{
				String(ImVec2(200, 290 + 10 * text_menu_font_size), GREEN, menu0.c_str(), text_menu_font_size);
			}if (!player_glow) {
				String(ImVec2(200, 290 + 20 * text_menu_font_size), RED, menu1.c_str(), text_menu_font_size);
			}
			else
			{
				String(ImVec2(200, 290 + 20 * text_menu_font_size), GREEN, menu1.c_str(), text_menu_font_size);
			}
			if (!item_glow) {
				String(ImVec2(200, 290 + 30 * text_menu_font_size), RED, menu2.c_str(), text_menu_font_size);
			}
			else
			{
				String(ImVec2(200, 290 + 30 * text_menu_font_size), GREEN, menu2.c_str(), text_menu_font_size);
			}if (!show_watcher) {
				String(ImVec2(200, 290 + 40 * text_menu_font_size), RED, menu3.c_str(), text_menu_font_size);
			}
			else
			{
				String(ImVec2(200, 290 + 40 * text_menu_font_size), GREEN, menu3.c_str(), text_menu_font_size);
			}if (!aim_enable) {
				String(ImVec2(200, 290 + 50 * text_menu_font_size), RED, menu4.c_str(), text_menu_font_size);
			}
			else
			{
				String(ImVec2(200, 290 + 50 * text_menu_font_size), GREEN, menu4.c_str(), text_menu_font_size);
				String(ImVec2(200, 290 + 60 * text_menu_font_size), GREEN, menu4_1.c_str(), text_menu_font_size);
				String(ImVec2(200, 290 + 70 * text_menu_font_size), GREEN, menu4_2.c_str(), text_menu_font_size);
			}if (!fakeduck) {
				String(ImVec2(200, 290 + 80 * text_menu_font_size), RED, menu5.c_str(), text_menu_font_size);
			}
			else
			{
				String(ImVec2(200, 290 + 80 * text_menu_font_size), GREEN, menu5.c_str(), text_menu_font_size);
			}
		}
		ImGui::End();
	if (g_Base != 0 && (esp||show_watcher))
	{
		memset(players, 0, sizeof(players));
		while (!next && (esp||show_watcher))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		if (next && valid)
		{
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2((float)getWidth(), (float)getHeight()));
			ImGui::Begin(XorStr("##esp"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
			int watch_num = 0;
			std::string watcher = XorStr(u8"观众人数：");
			std::string Line = XorStr(u8"————————————");
			String(20, ImVec2(10, 290), RED, watcher.c_str());
			String(20, ImVec2(10, 290 + 20), RED, Line.c_str());
			for (int i = 0; i < 64; i++)
			{
				if (esp) {
					if (players[i].health > 0 && players[i].dist > 1 && (players[i].dist / 39.62) < esp_max_dist)
					{
						std::string distance = std::to_string(players[i].dist / 39.62);
						distance = distance.substr(0, distance.find('.')) + "m(" + std::to_string(players[i].entity_team) + ")";
						std::string health = std::to_string(players[i].health);
						health = XorStr(u8"血：") + health;
						std::string shield = std::to_string(players[i].shield);
						shield = XorStr(u8"甲：") + shield;
						if (v.box)
						{
							if (players[i].visible)
							{
								if (players[i].dist < esp_max_dist)
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
								String(ImVec2(players[i].boxMiddle, (players[i].b_y + 1)), RED, distance.c_str(), esp_font_size);  //DISTANCE
							else
								String(ImVec2(players[i].boxMiddle, (players[i].b_y + 1)), GREEN, distance.c_str(), esp_font_size);  //DISTANCE
						}

						if (v.healthbar)
						{
							ProgressBar((players[i].b_x - (players[i].width / 2.0f) - 4), (players[i].b_y - players[i].height), 5, players[i].height, players[i].health, 100); //health bar
							String(ImVec2(players[i].boxMiddle, (players[i].b_y + 10 * esp_font_size)), GREEN, health.c_str(), esp_font_size);//show health
						}
						if (v.shieldbar) {
							ProgressBar((players[i].b_x + (players[i].width / 2.0f) + 1), (players[i].b_y - players[i].height), 5, players[i].height, players[i].shield, 125); //shield bar
							switch (players[i].max_shield)
							{
							case 50:String(ImVec2(players[i].boxMiddle, (players[i].b_y + 20 * esp_font_size)), WHITE, shield.c_str(), esp_font_size); break;
							case 75:String(ImVec2(players[i].boxMiddle, (players[i].b_y + 20 * esp_font_size)), BLUE, shield.c_str(), esp_font_size); break;
							case 100:String(ImVec2(players[i].boxMiddle, (players[i].b_y + 20 * esp_font_size)), PURPLE, shield.c_str(), esp_font_size); break;
							case 125:String(ImVec2(players[i].boxMiddle, (players[i].b_y + 20 * esp_font_size)), RED, shield.c_str(), esp_font_size); break;
							default:String(ImVec2(players[i].boxMiddle, (players[i].b_y + 20 * esp_font_size)), WHITE, shield.c_str(), esp_font_size); break;
							}
						}
						if (v.name)
							String(ImVec2(players[i].boxMiddle, (players[i].b_y - players[i].height - 15)), WHITE, players[i].name);
					}
				}
				if (show_watcher) {
						
						if (players[i].Observer != 0&& players[i].Observer != 7 && players[i].dead == 1)
						{
							String(ImVec2(5, 290 + (watch_num + 2) * 20), GREEN, std::to_string(players[i].dead).c_str());
							String(ImVec2(10, 290 + (watch_num + 2) * 20), BLUE, std::to_string(players[i].Observer).c_str());
							String(ImVec2(20, 290 + (watch_num + 2) * 20), GREEN, players[i].name);
							watch_num++;
						}
						else if (players[i].Observer == 0 && players[i].Observer != 7 && players[i].dead !=0 )
						{
							String(ImVec2(5, 290 + (watch_num + 2) * 20), GREEN, std::to_string(players[i].dead).c_str());
							String(ImVec2(10, 290 + (watch_num + 2) * 20), BLUE, std::to_string(players[i].Observer).c_str());
							String(ImVec2(20, 290 + (watch_num + 2) * 20), RED, players[i].name);
							watch_num++;
						}
				}
			}
			String(ImVec2(100, 290), GREEN, std::to_string(watch_num).c_str());
			watch_num = 0;
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

	if (out.z < 0.01f)
	{
		out.x = out.y = out.z = 0.0f;
		return out; 
	}

	_x *= 1.0f / out.z;
	_y *= 1.0f / out.z;

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

Vector3 GetEntityBonePosition(uintptr_t ent, int BoneId, Vector3 BasePosition)
{
	uintptr_t pBoneArray = GetEntityBoneArray(ent);

	Vector3 EntityHead = Vector3();

	EntityHead.x = read<float>(pBoneArray + 0xCC + (BoneId * 0x30)) + BasePosition.x;//8-head
	EntityHead.y = read<float>(pBoneArray + 0xDC + (BoneId * 0x30)) + BasePosition.y;
	EntityHead.z = read<float>(pBoneArray + 0xEC + (BoneId * 0x30)) + BasePosition.z;

	return EntityHead;
}

Vector3 GetEntityBasePosition(uintptr_t ent)
{
	return read<Vector3>(ent + OFFSET_ORIGIN);
}

void esp_init_func(DWORD64 Entity, Matrix m, uintptr_t locPlayer, player* players) {
	//float boxThickness = 2;

	Vector3 entHead = GetEntityBonePosition(Entity, 8, GetEntityBasePosition(Entity));
	Vector3 w2sHead = _WorldToScreen(entHead, m); //if (w2sHead.z <= 0.f) return;

	Vector3 entPos = GetEntityBasePosition(Entity);
	Vector3 w2sPos = _WorldToScreen(entPos, m); //if (w2sPos.z <= 0.f) return;

	float height = abs(abs(w2sHead.y) - abs(w2sPos.y));
	float width = height / 2.f;
	float middle = w2sPos.x - (width / 2.f);

	Vector3 vec = GetEntityBasePosition(locPlayer);
	players->dist = sqrt(
		pow(vec.x - entPos.x, 2) +
		pow(vec.y - entPos.y, 2)
	);

	players->boxMiddle = middle;
	players->b_x = w2sPos.x;
	players->b_y = w2sPos.y;
	players->h_y = w2sHead.y;// -boxThickness;
	players->width = width;
	players->height = height;
	//return players;
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

void player_glow_f(DWORD64 Entity, float* color)
{
	if (player_glow) {
		write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
		write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 2); // glow through walls: 2 = enabled, 5 = disabled
		//BorderSize_c = (int8_t)BorderSize;
		//TransparentLevel_c = (int8_t)TransparentLevel;
		write<GlowMode>(Entity + GLOW_TYPE, { (int8_t)(player_main_glow_type+100),(int8_t)(player_border_glow_type+100),(int8_t)BorderSize,(int8_t)TransparentLevel }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
		write<float>(Entity + GLOW_DISTANCE, player_glow_distance * 3000.0f / 70.0f);//玩家发光距离
		write<float>(Entity + 0x1D0, color[0] * 255); // r color/brightness of enemies
		write<float>(Entity + 0x1D4, color[1] * 255);  // g
		write<float>(Entity + 0x1D8, color[2] * 255); // b
	}
}
void get_name(uintptr_t oBaseAddress, uintptr_t Entity, uint64_t index, char* name)
{
	uintptr_t nameindex = read<uintptr_t>(Entity + 0x38);
	index *= 0x10;
	uintptr_t name_ptr = read<uintptr_t>(oBaseAddress + OFFSET_NAME_LIST + ((nameindex - 1) << 4));
	//std::string names = read<std::string>(name_ptr);
	//return names;
	std::string names;
	names = readmem<std::string>(name_ptr, 24);
	strcpy(name, names.c_str());
}
void item_glow_f(DWORD64 Entity, float* color)
{
	write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
	write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 2); // glow through walls: 2 = enabled, 5 = disabled
	write<GlowMode>(Entity + ITEM_GLOW_TYPE, { (int8_t)(item_main_glow_type+100),(int8_t)(item_border_glow_type+100),30,70 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
	write<float>(Entity + GLOW_DISTANCE, item_glow_distance * 3000.0f / 70.0f);//物品发光距离
	write<float>(Entity + 0x1D0, color[0] * 255); // r color/brightness
	write<float>(Entity + 0x1D4, color[1] * 255);  // g
	write<float>(Entity + 0x1D8, color[2] * 255); // b
}
void item_glow_func(uintptr_t oBaseAddress) {
	item_glow_ok = false;
	for (int i = 0; i <= max_check_glow_item_num; i++) {
		DWORD64 Entity = GetEntityById(i, oBaseAddress);
		int itemid = read<int>(Entity + OFFSET_ITEM_ID);
		if (glow_gun && itemid == glow_gun_num)
		{
			item_glow_f(Entity, gun_glow_col);//选择的武器
		}
		else if (zoom_glow && zoom_num != -1 && itemid == (zoom_num + 193))
		{
			item_glow_f(Entity, zoom_col);//选择的瞄准镜
		}
		else if (turbo_glow && itemid == 232) {
			item_glow_f(Entity, gold_item_col);//涡轮
		}
		else if (fast_reload_glow && itemid == 245) {
			item_glow_f(Entity, gold_item_col);//加速装填器
		}
		else if (glow_supply_gun && (itemid == 180 || itemid == 1 || itemid == 37 || itemid == 57 || itemid == 68)) {
			item_glow_f(Entity, red_item_col);//空投枪和红甲
		}
		else if (blue_glow && (itemid == 252 || itemid == 166 || itemid == 253 || itemid == 187)) {
			if (heat_shield_glow && itemid == 252)
			{
				item_glow_f(Entity, blue_item_col);//隔热板
			}
			else if (Shield_Battery_glow && itemid == 166) {
				item_glow_f(Entity, blue_item_col);//大电
			}
			else if (respawn_glow && itemid == 253) {
				item_glow_f(Entity, blue_item_col);//重生信标
			}
			else if (blue_backpack_glow && itemid == 187)
			{
				item_glow_f(Entity, blue_item_col);//蓝包
			}
		}
		else if (glow_suit && (itemid == 170 || itemid == 179 || itemid == 188 || itemid == 171 || itemid == 175 || itemid == 185 || itemid == 189)) {
			if (itemid == 170 || itemid == 179 || itemid == 188) {
				item_glow_f(Entity, purple_item_col);//紫装备
			}
			else if (itemid == 171 || itemid == 175 || itemid == 185 || itemid == 189)
			{
				item_glow_f(Entity, gold_item_col);//金装备
			}
		}
		else if (glow_gunPart && (itemid == 205 || itemid == 209 || itemid == 213 || itemid == 217 || itemid == 221 || itemid == 225 || itemid == 228 || itemid == 231 || itemid == 202 || itemid == 210 || itemid == 214 || itemid == 218 || itemid == 222)) {
			if (itemid == 205 || itemid == 209 || itemid == 213 || itemid == 217 || itemid == 221 || itemid == 225 || itemid == 228 || itemid == 231) {
				item_glow_f(Entity, purple_item_col);//紫配件
			}
			else if (itemid == 202 || itemid == 210 || itemid == 214 || itemid == 218 || itemid == 222) {
				item_glow_f(Entity, gold_item_col);//金配件
			}
		}
		else if (glow_goldgun && (itemid == 6 || itemid == 11 || itemid == 21 || itemid == 26 || itemid == 31 || itemid == 36 || itemid == 46 || itemid == 51 || itemid == 56 || itemid == 62 || itemid == 67 || itemid == 73 || itemid == 78 || itemid == 83 || itemid == 88 || itemid == 94 || itemid == 99 || itemid == 104 || itemid == 109 || itemid == 114 || itemid == 119 || itemid == 131 || itemid == 136)) {
			item_glow_f(Entity, gold_item_col);//金枪
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	item_glow_ok = true;
};
std::string get_aim_key() {
	switch (aim_key)
	{
	case 0: return XorStr(u8"鼠标左键+鼠标右键"); break;//鼠标左右键
	case 1: return XorStr(u8"鼠标左键"); break;//鼠标左键
	case 2: return XorStr(u8"鼠标右键"); break;//鼠标右键
	case 3: return XorStr(u8"键盘右Alt"); break;//键盘右Alt
	}
}
bool aim_key_on() {
	bool enable = false;
	switch (aim_key)
	{
	case 0: enable = (GetAsyncKeyState(VK_LBUTTON) || GetAsyncKeyState(VK_RBUTTON)); break;//鼠标左右键
	case 1: enable = GetAsyncKeyState(VK_LBUTTON); break;//鼠标左键
	case 2: enable = GetAsyncKeyState(VK_RBUTTON); break;//鼠标右键
	case 3: enable = GetAsyncKeyState(VK_RMENU); break;//键盘右Alt
	}
	return enable;
}
void aim_func(int closestX, int closestY, int crosshairX, int crosshairY) {
	// After entity loop ends
	if (closestX != 9999 && closestY != 9999)
	{
		//	 If aimbot key pressed
		if (aim_key_on())
		{
			//		 If mouse cursor shown

			CURSORINFO ci = { sizeof(CURSORINFO) };
			if (GetCursorInfo(&ci))
			{
				if (ci.flags == 0) {
					aX = (closestX - crosshairX) / aSmoothAmount;
					aY = (closestY - crosshairY) / aSmoothAmount;
				}
				//printf("%d\t%d\n", aX,aY);
				//mouse_move((int)aX, (int)(aY));
				mouse_event(MOUSEEVENTF_MOVE, aX, aY, 0, 0); // enable aimbot when mouse cursor is hidden
			}
		}
	}
}
/*void third_person_func(uintptr_t oBaseAddress) {
	if (thirdperson && !tmp_third_person)
	{
		write<int>(oBaseAddress + OFFSET_THIRDPERSON2, 2);
		write<int>(oBaseAddress + OFFSET_THIRDPERSON3, 2);
		//int thridperson1 = read<int>(oBaseAddress + OFFSET_THIRDPERSON1);
		//int thridperson2 = read<int>(oBaseAddress + OFFSET_THIRDPERSON2);
		//int thridperson3 = read<int>(oBaseAddress + OFFSET_THIRDPERSON3);
		//printf("%d %d %d\n",thridperson1, thridperson2, thridperson3);
		tmp_third_person = true;
	}
	else if (!thirdperson && tmp_third_person)
	{
		write<int>(oBaseAddress + OFFSET_THIRDPERSON2, -1);
		write<int>(oBaseAddress + OFFSET_THIRDPERSON2, 0);
		tmp_third_person = false;
	}
}/**/

int gui(uintptr_t oBaseAddress/*int argc, char** argv*/)
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
	//printf(XorStr("Waiting for host process...\n"));
	if (active)
	{
		ready = true;
		printf(XorStr("Ready\n"));
	}
	printf("按F4即可正常关闭软件\n");
	printf("按F5开关ESP\n");
	printf("按F6可开关敌人发光\n");
	printf("按F7可开关物品发光\n");
	printf("按F8开关自瞄\n");


	while (active)
	{
		int j = 0;
		if (IsKeyDown(VK_F4))
		{
			active = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//printf("%f\t%f\t%f\n", col2[0], col2[1], col2[2]);
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		/*if (IsKeyDown(VK_F4))
		{
			active = false;
		}*/
		if (IsKeyDown(VK_F5) && k_f5 == 0)//F5 ESP方框
		{
			k_f5 = 1;
			esp = !esp;
		}
		else if (!IsKeyDown(VK_F5) && k_f5 == 1)
		{
			k_f5 = 0;
		}

		if (IsKeyDown(VK_F6))//F6 敌人发光
		{
			player_glow = !player_glow;
			Sleep(100);
		}
		if (IsKeyDown(VK_F7))//F7 物品发光
		{
			item_glow = !item_glow;
			Sleep(100);
		}
		if (IsKeyDown(VK_F8))//F8 开关自瞄
		{
			//aim_enable = true;
			aim_enable = !aim_enable;
			Sleep(100);
		}
		if (IsKeyDown(VK_F9))//F9 开关观众列表
		{
			show_watcher = !show_watcher;
			Sleep(200);
		}if (IsKeyDown(VK_F10))//F9 开关假蹲
		{
			fakeduck = !fakeduck;
			Sleep(100);
		}
		if ( !(GetAsyncKeyState(VK_SHIFT) & 0x8000) && IsKeyDown(VK_LEFT) && aSmoothAmount > 0) {
			aSmoothAmount -= 1;//←降低自瞄平滑度
			Sleep(100);
		}
		if ( !(GetAsyncKeyState(VK_SHIFT) & 0x8000) && IsKeyDown(VK_RIGHT) && aSmoothAmount < 30) {
			aSmoothAmount += 1;//→增加自瞄平滑度
			Sleep(100);
		}
		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && IsKeyDown(VK_LEFT)) {
			if (aim_key > 0)
			{
				aim_key -= 1;//Shift+←更改自瞄按键
				Sleep(100);
			}
			else {
				aim_key = 3;
				Sleep(100);
			}
		}
		if (((GetAsyncKeyState(VK_SHIFT) & 0x8000)) && IsKeyDown(VK_RIGHT)) {
			if (aim_key < 3) {
				aim_key += 1;//Shift+→更改自瞄按键
				Sleep(100);
			}
			else {
				aim_key = 0;
				Sleep(100);
			}
		}
		if (IsKeyDown(VK_HOME)) {
			show_text_menu = !show_text_menu;//HOME 键开关文字菜单显示
			Sleep(200);
		}
		/*if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && IsKeyDown(VK_HOME)) {
			show_watcher = !show_watcher;//SHIFT + HOME 键开关观众名单
			Sleep(200);
		}*/
		if (fakeduck) {
			if (!GetAsyncKeyState(0x41) && !GetAsyncKeyState(0x57) && !GetAsyncKeyState(0x53) && !GetAsyncKeyState(0x44) && !GetAsyncKeyState(VK_CONTROL) && !GetAsyncKeyState(VK_LSHIFT)) {
				write<int>(oBaseAddress + OFFSET_KEY_DUCK + 0x08, 5);
				Sleep(25);
				write<int>(oBaseAddress + OFFSET_KEY_DUCK + 0x08, 4);
				Sleep(25);
			}
		}

		/*if (IsKeyDown(VK_F6) && k_f6 == 0)
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
			aiming = false;*/
		//third_person_func(oBaseAddress);//第三人称视角
		if (keepjump) {
			if (GetAsyncKeyState(VK_SPACE)) {//连跳
				write<int>(oBaseAddress + OFFSET_KEY_JUMP + 0x08, 5);
				Sleep(5);
				write<int>(oBaseAddress + OFFSET_KEY_JUMP + 0x08, 4);
				Sleep(5);
			}
		}
		if (item_glow && item_glow_ok)
		{
			std::thread item_th(item_glow_func, oBaseAddress);//,std::ref(item_glow_ok));
			item_glow_ok = false;
			if (item_th.joinable())
			{
				item_th.detach();

			}
		}
		if (player_glow || esp)
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
			j = 0;
			for (int i = 0; i < 64; i++)
			{
				DWORD64 Entity = GetEntityById(i, oBaseAddress);
				DWORD64 EntityList = oBaseAddress + OFFSET_ENTITYLIST;
				if (Entity == 0)
					continue;
				DWORD64 EntityHandle = read<DWORD64>(Entity + OFFSET_NAME);
				std::string Identifier = read<std::string>(EntityHandle);//"player" or null
				LPCSTR IdentifierC = Identifier.c_str();
				valid = false; next = false;
				if (strcmp(IdentifierC, "player"))
				{
					get_name(oBaseAddress, Entity, j-1, players[j].name);
					//strcpy(players[j].name,name.c_str());
					//printf("%s\n", players[i].name);
					//printf("0:%s\t", name.c_str());
					//printf("1:%s\n", players[j].name);
					//printf("2:%s\t", IdentifierC);
					//printf("3:%s\n", Identifier.c_str());
					Vector3 HeadPosition = GetEntityBonePosition(Entity, bone, GetEntityBasePosition(Entity));

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

					//Get player health
					players[j].dead = read<int>(Entity + OFFSET_LIFESTATE);//dead == 1，player is dead
					if(players[j].dead != 0)
					{
						players[j].health = 0;
					}
					else
					{
						players[j].health = read<int>(Entity + OFFSET_HEALTH);
					}

					// Get entity team ID
					int entTeamID = read<int>(Entity + OFFSET_TEAM);
					players[j].entity_team = entTeamID;

					//Get player shield
					players[j].shield = read<int>(Entity + OFFSET_SHIELD);
					players[j].max_shield = read<int>(Entity + OFFSET_SHIELD_MAX);


					players[j].Observer = read<int>(Entity + OFFSET_OBSERVER_MODE);
					//if(players[j].Observer!=7)
					//printf("%s %d\n", players[j].name, players[j].Observer);
					

					// Is it an enemy
					if (entTeamID != playerTeamID)
					{
						esp_init_func(Entity, m, locPlayer, &players[j]);
						/*
						write<int>(Entity + OFFSET_GLOW_ENABLE, 1); // glow enable: 1 = enabled, 2 = disabled
						write<int>(Entity + OFFSET_GLOW_THROUGH_WALLS, 2); // glow through walls: 2 = enabled, 5 = disabled
						write<GlowMode>(Entity + GLOW_TYPE, { 101,101,46,90 }); // glow type: GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
						*/

						// Is visible
						if (entNewVisTime != entOldVisTime[i])
						{
							visCooldownTime[i] = 0; // low values mean less latency, increase if you observe the color changes on visible enemies
							players[j].visible = true;

							//visible enemie color
							if (entKnockedState == 0)
							{
								/*
								write<float>(Entity + 0x1D0, 0); // red color/brightness of visible enemies
								write<float>(Entity + 0x1D4, 61); // green
								write<float>(Entity + 0x1D8, 0); // blue
								*/
								player_glow_f(Entity, playerglow1);
								players[j].knocked = false;
								// Aimbot fov
								yFOV = xFOV;
								if (abs(crosshairX - entX) < abs(crosshairX - closestX) && abs(crosshairX - entX) < xFOV && abs(crosshairY - entY) < abs(crosshairY - closestY) && abs(crosshairY - entY) < yFOV)
								{
									// Aimbot find closest target
									closestX = entX;
									closestY = entY;
								}
							}
							else
							{
								/*
								//visible enemie down color
								write<float>(Entity + 0x1D0, 0); // r color/brightness of knocked enemies
								write<float>(Entity + 0x1D4, 0); // g
								write<float>(Entity + 0x1D8, 255); // b
								*/
								player_glow_f(Entity, playerglow2);
								players[j].knocked = true;
							}

							entOldVisTime[i] = entNewVisTime;
						}
						else
						{
							if (visCooldownTime[i] <= 0)
							{
								players[j].visible = false;
								//unvisible enemie color
								if (entKnockedState == 0)
								{
									/*
									write<float>(Entity + 0x1D0, 61); // r color/brightness of not visible enemies
									write<float>(Entity + 0x1D4, 0);  // g
									write<float>(Entity + 0x1D8, 0); // b
									*/
									player_glow_f(Entity, playerglow3);
									players[j].knocked = false;
								}
								else
								{
									/*
									//unvisible enemie down color
									write<float>(Entity + 0x1D0, 0); // r color/brightness of knocked enemies
									write<float>(Entity + 0x1D4, 0); // g
									write<float>(Entity + 0x1D8, 255); // b
									*/
									player_glow_f(Entity, playerglow4);
									players[j].knocked = true;
								}
							}
						}

						if (visCooldownTime[i] >= 0) visCooldownTime[i] -= 1;
					}
					valid = true;
					j++;
				}
			}
			next = true;
			//std::this_thread::sleep_for(std::chrono::milliseconds(1));
			if (aim_enable)
				aim_func(closestX, closestY, crosshairX, crosshairY);
			//Sleep(100);
		}

		/*if (item_glow) {
			//for (int i = 0; i <= max_check_glow_item_num; i++) {
			//	DWORD64 Entity = GetEntityById(i, oBaseAddress);
			//	int itemid = read<int>(Entity + OFFSET_ITEM_ID);
			//	if (glow_gun && itemid == glow_gun_num)
			//	{
			//		item_glow_f(Entity, gun_glow_col);//选择的武器
			//	}
			//	else if (zoom_glow && zoom_num!= -1 &&itemid == (zoom_num+193))
			//	{
			//		item_glow_f(Entity, zoom_col);//选择的瞄准镜
			//	}
			//	else if(turbo_glow&& itemid == 232){
			//		item_glow_f(Entity, gold_item_col);//涡轮
			//	}
			//	else if (fast_reload_glow&&itemid== 245) {
			//		item_glow_f(Entity, gold_item_col);//加速装填器
			//	}
			//	else if (glow_supply_gun&& (itemid == 180 || itemid == 1 || itemid == 37 || itemid == 57 || itemid == 68)) {
			//			item_glow_f(Entity, red_item_col);//空投枪和红甲
			//	}
			//	else if (blue_glow&&(itemid==252||itemid == 166||itemid==253||itemid == 187)) {
			//		if (heat_shield_glow && itemid == 252)
			//		{
			//			item_glow_f(Entity, blue_item_col);//隔热板
			//		}
			//		else if (Shield_Battery_glow && itemid == 166) {
			//			item_glow_f(Entity, blue_item_col);//大电
			//		}
			//		else if (respawn_glow && itemid == 253) {
			//			item_glow_f(Entity, blue_item_col);//重生信标
			//		}
			//		else if (blue_backpack_glow && itemid == 187)
			//		{
			//			item_glow_f(Entity, blue_item_col);//蓝包
			//		}
			//	}
			//	else if (glow_suit&& (itemid == 170 || itemid == 179 || itemid == 188|| itemid == 171 || itemid == 175 || itemid == 185 || itemid == 189)) {
			//		if (itemid==170||itemid == 179 || itemid == 188) {
			//		item_glow_f(Entity, purple_item_col);//紫装备
			//	}
			//		else if (itemid == 171 || itemid == 175 || itemid == 185 || itemid == 189)
			//		{
			//			item_glow_f(Entity, gold_item_col);//金装备
			//		}
			//	}
			//	else if (glow_gunPart&& (itemid == 205 || itemid == 209 || itemid == 213 || itemid == 217 || itemid == 221 || itemid == 225 || itemid == 228 || itemid == 231|| itemid == 202 || itemid == 210 || itemid == 214 || itemid == 218 || itemid == 222)) {
			//		if (itemid == 205 || itemid == 209 || itemid == 213 || itemid == 217 || itemid == 221 ||itemid == 225 ||itemid == 228 || itemid == 231) {
			//			item_glow_f(Entity, purple_item_col);//紫配件
			//		}
			//		else if ( itemid == 202 || itemid == 210 || itemid == 214 || itemid == 218 || itemid == 222) {
			//			item_glow_f(Entity, gold_item_col);//金配件
			//		}
			//	}
			//	else if (glow_goldgun&& (itemid == 6 || itemid == 11 || itemid == 21 || itemid == 26 || itemid == 31 || itemid == 36 || itemid == 46 || itemid == 51 || itemid == 56 || itemid == 62 || itemid == 67 || itemid == 73 || itemid == 78 || itemid == 83 || itemid == 88 || itemid == 94 || itemid == 99 || itemid == 104 || itemid == 109 || itemid == 114 || itemid == 119 || itemid == 131 || itemid == 136)) {
			//			item_glow_f(Entity, gold_item_col);//金枪
			//	}
			//}
			//if (item_glow_th.joinable()) {
				std::thread item_glow_th(item_glow_func, oBaseAddress);
				if (item_glow_th.joinable())
				{
					item_glow_th.detach();
				}
			//}
		}*/
	}
	ready = false;
	ov1.Clear();
	if (!use_nvidia)
		//system(XorStr("taskkill /F /T /IM overlay.exe"));
		system(XorStr("taskkill /F /T /IM overlay_ap.exe")); //custom overlay process name
	return 0;
}
std::string get_path() {
	char path[200];                        //用于存放获取路径的信息    
	getcwd(path, sizeof(path));
	return path;
}
bool file_to_setting(std::string path) {
	FILE* fp;
	path += "\\setting.ini";
	if (NULL == (fp = fopen(path.c_str(), "r")))
		return false;
	else fclose(fp);
	char c[30];
	float f[3] = {0,0,0};
	int ref;
	//[自瞄]
	GetIniKeyString("自瞄", "自瞄按键", path.c_str(), &aim_key);// if (ref == -2)return false;
	GetIniKeyString("自瞄", "自瞄窗口大小", path.c_str(), &xFOV);
	GetIniKeyString("自瞄", "平滑度", path.c_str(), &aSmoothAmount);
	
	//[ESP]
	GetIniKeyString(XorStr("ESP"), XorStr("ESP字体大小"), path.c_str(), &esp_font_size);
	GetIniKeyString(XorStr("ESP"), XorStr("文字菜单字体大小"), path.c_str(), &text_menu_font_size);
	GetIniKeyString(XorStr("ESP"), "名字", path.c_str(), c); v.name = c2bool(c);
	GetIniKeyString(XorStr("ESP"), "护甲条", path.c_str(), c); v.shieldbar = c2bool(c);
	GetIniKeyString(XorStr("ESP"), "血条", path.c_str(), c); v.healthbar = c2bool(c);
	GetIniKeyString(XorStr("ESP"), "距离", path.c_str(), c); v.distance = c2bool(c);
	GetIniKeyString(XorStr("ESP"), "方框", path.c_str(), c); v.box = c2bool(c);
	GetIniKeyString(XorStr("ESP"), "连线", path.c_str(), c); v.line = c2bool(c);
	
	//[物体发光]
	GetIniKeyString("物品发光", "物品发光最大遍历数", path.c_str(), &max_check_glow_item_num);
	GetIniKeyString("物体发光", "涡轮", path.c_str(), c); turbo_glow = c2bool(c); //if (c2bool(c))printf("true\n"); if (!c2bool(c))printf("false\n");
	GetIniKeyString("物体发光", "加速装填器", path.c_str(), c); fast_reload_glow = c2bool(c); //if (c2bool(c))printf("true\n"); if (!c2bool(c))printf("false\n");
	GetIniKeyString("物体发光", "金枪", path.c_str(), c); glow_goldgun = c2bool(c); //if (c2bool(c))printf("true\n"); if (!c2bool(c))printf("false\n");
	GetIniKeyString("物体发光", "空投枪和红甲", path.c_str(), c); glow_supply_gun = c2bool(c);// if (c2bool(c))printf("true\n"); if (!c2bool(c))printf("false\n");
	GetIniKeyString("物体发光", "紫金装备", path.c_str(), c); glow_suit = c2bool(c); //if (c2bool(c))printf("true\n"); if (!c2bool(c))printf("false\n");
	GetIniKeyString("物体发光", "紫金配件", path.c_str(), c); glow_gunPart = c2bool(c);// if (c2bool(c))printf("true\n"); if (!c2bool(c))printf("false\n");
	
	GetIniKeyString("物体发光", "金色r", path.c_str(), &gold_item_col[0],255);
	GetIniKeyString("物体发光", "金色g", path.c_str(), &gold_item_col[1],255);
	GetIniKeyString("物体发光", "金色b", path.c_str(), &gold_item_col[2],255);
		
	GetIniKeyString("物体发光", "红色r", path.c_str(), &red_item_col[0],255);
	GetIniKeyString("物体发光", "红色g", path.c_str(), &red_item_col[1], 255);
	GetIniKeyString("物体发光", "红色b", path.c_str(), &red_item_col[2], 255);
		
	GetIniKeyString("物体发光", "紫色r", path.c_str(), &purple_item_col[0], 255);
	GetIniKeyString("物体发光", "紫色g", path.c_str(), &purple_item_col[1], 255);
	GetIniKeyString("物体发光", "紫色b", path.c_str(), &purple_item_col[2], 255);
			
	GetIniKeyString("物体发光", "蓝色r", path.c_str(), &blue_item_col[0], 255);
	GetIniKeyString("物体发光", "蓝色g", path.c_str(), &blue_item_col[1], 255);
	GetIniKeyString("物体发光", "蓝色b", path.c_str(), &blue_item_col[2], 255);
			
	GetIniKeyString("物体发光", "枪械发光颜色r", path.c_str(), &gun_glow_col[0], 255);
	GetIniKeyString("物体发光", "枪械发光颜色g", path.c_str(), &gun_glow_col[1], 255);
	GetIniKeyString("物体发光", "枪械发光颜色b", path.c_str(), &gun_glow_col[2], 255);
				
	GetIniKeyString("物体发光", "瞄准镜发光颜色r", path.c_str(), &zoom_col[0], 255);
	GetIniKeyString("物体发光", "瞄准镜发光颜色g", path.c_str(), &zoom_col[1], 255);
	GetIniKeyString("物体发光", "瞄准镜发光颜色b", path.c_str(), &zoom_col[2], 255);

	GetIniKeyString("物体发光", "物品主发光类型", path.c_str(), &item_main_glow_type);
	GetIniKeyString("物体发光", "物品发光边界类型", path.c_str(), &item_border_glow_type);
	GetIniKeyString("物体发光", "物品发光距离", path.c_str(), &item_glow_distance);

	//[玩家发光]
	GetIniKeyString("玩家发光", "玩家发光", path.c_str(), c); player_glow = c2bool(c);
	GetIniKeyString("玩家发光", "玩家发光主类型", path.c_str(), &player_main_glow_type);
	GetIniKeyString("玩家发光", "玩家发光边界类型", path.c_str(), &player_border_glow_type);
	GetIniKeyString("玩家发光", "玩家发光边界尺寸", path.c_str(), &BorderSize);
	GetIniKeyString("玩家发光", "玩家发光透明度", path.c_str(), &TransparentLevel);
	GetIniKeyString("玩家发光", "玩家发光距离", path.c_str(), &player_glow_distance);

	GetIniKeyString("玩家发光", "可见敌人颜色r", path.c_str(), &playerglow1[0], 255);
	GetIniKeyString("玩家发光", "可见敌人颜色g", path.c_str(), &playerglow1[1], 255);
	GetIniKeyString("玩家发光", "可见敌人颜色b", path.c_str(), &playerglow1[2], 255);
	
	GetIniKeyString("玩家发光", "可见倒地敌人颜色r", path.c_str(), &playerglow2[0], 255);
	GetIniKeyString("玩家发光", "可见倒地敌人颜色g", path.c_str(), &playerglow2[1], 255);
	GetIniKeyString("玩家发光", "可见倒地敌人颜色b", path.c_str(), &playerglow2[2], 255);
	
	GetIniKeyString("玩家发光", "不可见敌人颜色r", path.c_str(), &playerglow3[0], 255);
	GetIniKeyString("玩家发光", "不可见敌人颜色g", path.c_str(), &playerglow3[1], 255);
	GetIniKeyString("玩家发光", "不可见敌人颜色b", path.c_str(), &playerglow3[2], 255);
	
	GetIniKeyString("玩家发光", "不可见倒地敌人颜色r", path.c_str(), &playerglow4[0], 255);
	GetIniKeyString("玩家发光", "不可见倒地敌人颜色g", path.c_str(), &playerglow4[1], 255);
	GetIniKeyString("玩家发光", "不可见倒地敌人颜色b", path.c_str(), &playerglow4[2], 255);
	/**/
	return true;
}
bool setting_to_file(std::string path) {
	path += "\\setting.ini";
	//path = ".\setting.ini";
	int ref,i=1;

	//[AIM]
	PutIniKeyString("自瞄", "自瞄按键", aim_key, path.c_str());
	PutIniKeyString("自瞄", "自瞄窗口大小", xFOV, path.c_str());
	PutIniKeyString("自瞄", "平滑度", aSmoothAmount, path.c_str());

	//[ESP]
	ref = PutIniKeyString("ESP", "ESP字体大小", esp_font_size, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("ESP", "文字菜单字体大小", text_menu_font_size, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("ESP", "名字", v.name, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("ESP", "护甲条", v.shieldbar, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("ESP", "血条", v.healthbar, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("ESP", "距离", v.distance, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("ESP", "连线", v.line, path.c_str()); //printf("%d:%d\n", i++, ref);
	/**/
	//[物体发光]
	ref = PutIniKeyString("物体发光", "物品发光最大遍历数", max_check_glow_item_num, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "涡轮", turbo_glow, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "加速装填器", fast_reload_glow, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "金枪", glow_goldgun, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "空投枪和红甲", glow_supply_gun, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "紫金装备", glow_suit, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "紫金配件", glow_gunPart, path.c_str()); //printf("%d:%d\n", i++, ref);

	ref = PutIniKeyString("物体发光", "金色r", gold_item_col[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "金色g", gold_item_col[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "金色b", gold_item_col[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);

	ref = PutIniKeyString("物体发光", "红色r", red_item_col[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "红色g", red_item_col[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "红色b", red_item_col[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);

	ref = PutIniKeyString("物体发光", "紫色r", purple_item_col[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "紫色g", purple_item_col[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "紫色b", purple_item_col[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	ref = PutIniKeyString("物体发光", "蓝色r", blue_item_col[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "蓝色g", blue_item_col[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "蓝色b", blue_item_col[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	ref = PutIniKeyString("物体发光", "枪械发光颜色r", gun_glow_col[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "枪械发光颜色g", gun_glow_col[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "枪械发光颜色b", gun_glow_col[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	ref = PutIniKeyString("物体发光", "瞄准镜发光颜色r", zoom_col[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "瞄准镜发光颜色g", zoom_col[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "瞄准镜发光颜色b", zoom_col[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	ref = PutIniKeyString("物体发光", "物品主发光类型", item_main_glow_type, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "物品发光边界类型", item_border_glow_type, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("物体发光", "物品发光距离", item_glow_distance, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	//[玩家发光]
	ref = PutIniKeyString("玩家发光", "玩家发光", player_glow, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "玩家发光主类型", player_main_glow_type, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "玩家发光边界类型", player_border_glow_type, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "玩家发光边界尺寸", BorderSize, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "玩家发光透明度", TransparentLevel, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "玩家发光距离", player_glow_distance, path.c_str()); //printf("%d:%d\n", i++, ref);

	ref = PutIniKeyString("玩家发光", "可见敌人颜色r", playerglow1[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "可见敌人颜色g", playerglow1[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "可见敌人颜色b", playerglow1[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	ref = PutIniKeyString("玩家发光", "可见倒地敌人颜色r", playerglow2[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "可见倒地敌人颜色g", playerglow2[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "可见倒地敌人颜色b", playerglow2[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	ref = PutIniKeyString("玩家发光", "不可见敌人颜色r", playerglow3[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "不可见敌人颜色g", playerglow3[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "不可见敌人颜色b", playerglow3[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	ref = PutIniKeyString("玩家发光", "不可见倒地敌人颜色r", playerglow4[0] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "不可见倒地敌人颜色g", playerglow4[1] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	ref = PutIniKeyString("玩家发光", "不可见倒地敌人颜色b", playerglow4[2] * 255, path.c_str()); //printf("%d:%d\n", i++, ref);
	
	//printf("over\n");
	return true;
}

int main(int argCount, char** argVector)
{
	srand(time(NULL));
	std::string filePath = argVector[0];
	RenameFile(filePath);

	if (verify() == -1)
		exit(0);
	else {
		oBaseAddress = init_main();
		g_Base = oBaseAddress;
		std::string path = get_path();
		file_to_setting(path);
		//while (!hwnd)
		//{
		//	hwnd = FindWindowA(NULL, ("Apex Legends"));
		//	Sleep(500);
		//}

		//while (!oPID) // get the process id
		//{
		//	oPID = GetPID("r5apex.exe");
		//	Sleep(500);
		//}

		//while (!oBaseAddress) // request the module base from driver
		//{
		//	oBaseAddress = GetModuleBaseAddress(oPID, "r5apex.exe");
		//	printf("进程id：%d\n",oPID);
		//	printf("基址：%d\n", oBaseAddress);
		//	printf(/*" [+] Driver Loader\n [+] Status Apex:Detected\n */"[+] Contact newton_miku\n [+]啊，哈哈哈哈\n [+]寄汤来喽\n");
		//	Sleep(500);
		//}
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		MessageBox(NULL, ("  程序已加载完毕\n[Insert]显示UI菜单\n[Home]显示文字菜单\n[F4]退出程序"), ("仅供内部使用"), MB_OK);
		gui(oBaseAddress);
		setting_to_file(path);
	}
}
