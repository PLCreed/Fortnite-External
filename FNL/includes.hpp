#pragma once
#define WIN32_LEAN_AND_MEAN

/*-------Windows-------*/
#include <windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <thread>
#include <random>
#include <Psapi.h>
#include <cstdint> 
#include <cmath>
#include <bcrypt.h>
#include <ntstatus.h>
#include <vector>
#include <array>
#include <mutex>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <codecvt>
#include <algorithm>
#include <xmmintrin.h>
#include <emmintrin.h>
#include <urlmon.h>
#include <mmsystem.h>

#pragma comment(lib, "urlmon.lib")

#ifdef _DEBUG
#define WRAP_IF_DEBUG(x) { x };
#else
#define WRAP_IF_DEBUG(x)
#endif
#define ERROR_MESSAGE(x) MessageBoxA(NULL, x, NULL, NULL)

#define U64 uint64_t
#define U8 uint8_t

/*-------Security-------*/
#include "xorstr.hpp"

/*-------AUTHENTICATION-*/
#include "../KeyAuth/auth.hpp"
#include "../KeyAuth/json.hpp"
#include "../KeyAuth/utils.hpp"

/*-------HyperV-------*/
#include "HyperV/CommsUtils.h"
#include "HyperV/HyperV.h"
#include "HyperV/Memory.h"

/*-------ImGui--------*/
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_internal.h"
#include <D3DX11tex.h>
#include <dwmapi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dwmapi.lib")
#pragma comment(lib,"d3dcompiler")

/*-------Misc-------*/
#include "math.hpp"

/*--------SDK----------*/
#include "settings.hpp"
#include "game.hpp"
#include "Players.hpp"
#include "Input.hpp"
class Globals {
public:
	DWORD_PTR LocalPlayer = 0;
	ImFont* icons = NULL;
	ImFont* bender = NULL;
	ImFont* title = NULL;
	ImFont* radar = NULL;
	ImFont* medium = NULL;
	ImFont* large = NULL;
	ImFont* norms2 = NULL;
	ImFont* input = NULL;
	ImFont* fut = NULL;
	ImFont* malynapis = NULL;
	ImFont* global3 = NULL;


	/*----AIM----*/
	bool LegitAim = true;
	bool StickyAim = false;
	bool Smoothing = false;
	int Smoothness = 1;
	bool IgnoreKnocked = false;
	bool ShowFOV = true;
	int FOVSize = 65;
	float fov_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
	bool Crosshair = false;
	int CrosshairSize = 1;
	float crosshair_color[4] = { 97.f, 97.f, 189.f, 255.f };
	bool Prediction = false;


	/*----ESP----*/
	bool Box = false;
	bool Lines = false;
	int Distance = 1;
	bool IsVisible = false;
	bool Name = false;
	bool DistanceText = true;
	bool Skeleton = true;
	bool Shield = false;
	bool Health = false;
	float Line_Invisible_Col[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
	float Line_Visible_Col[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
	float enm_Invisible_box_color[4] = { 180.f, 59.f, 64.f, 1.0f };
	float enm_Visible_box_color[4] = { 180.f, 59.f, 64.f, 1.0f };
	
	float Skeleton_Visible_Col[4] = { 180.f, 59.f, 64.f, 1.0f };
	float Skeleton_InVisible_Col[4] = { 180.f, 59.f, 64.f, 1.0f };
	float text_color[4] = { 255.f, 1.f, 0.f, 1.0f };

	float tnm_box_color[4] = { 0.f, 89.f, 255.f, 1.0f };
	float tnm_line_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };


	/*----MISC----*/
	bool NoRecoil = true;
	int RecoilAmount = 1;
	int subtab2 = 0;


} Globals, * _Global;

/*--------Overlay-----*/
#include "fonts.hpp"
#include "timer.hpp"
#include "overlay.hpp"