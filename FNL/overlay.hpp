#pragma once
#include "math.hpp"
#include "colors.hpp"

ImFont* font1;
ImFont* fontSmall;
ImFont* fontHead;
ImFont* fontHuge;

constexpr int menu_anim_time = 350;
constexpr int breath_anim_time = 1000;

#define toImVec2(x) *reinterpret_cast<ImVec2*>(&x)
ImVec4 clipCoords;
#define ABS(x) ((x < 0) ? (-x) : (x))

#define W2S(x, y) game::WorldToScreen(x, y)

#define TORAD(x) ((x)	*	0.01745329252)

namespace Setup
{
	ImColor Lines = ImColor(26, 26, 26);
	ImColor NeverMissText = ImColor(157, 176, 242);
	int ChildBorderj = 1;
}

static const char* aimbotMethod[]{ (" Memory Aimbot"), (" Silent Aimbot"), (" Mouse Aimbot") };
static const char* aimbotBones[]{ (" Head"),(" Neck"),(" Chest"),(" Stomach"),(" Left Arm"),(" Left Leg"),(" Right Arm"),(" Right Leg") };

class c_overlay {
private:
	HWND								window_handle;
	WNDCLASSEX							window_class;
	ID3D11Device* d3d_device;
	ID3D11DeviceContext* device_context;
	IDXGISwapChain* swap_chain;
	ID3D11RenderTargetView* render_target_view;
	ImDrawList* draw_list;
	DWORD								input_handler_thread;
	c_timer								breath_timer, menu_timer;

	struct s_font {
		ImFont* im_font;
		FLOAT font_size;
	};
private:
	template <typename T>
	inline VOID safe_release(T*& p) {
		if (NULL != p) {
			p->Release();
			p = NULL;
		}
	}

	BOOL init_device() {
		DXGI_SWAP_CHAIN_DESC vSwapChainDesc;
		ZeroMemory(&vSwapChainDesc, sizeof(vSwapChainDesc));
		vSwapChainDesc.BufferCount = 2;
		vSwapChainDesc.BufferDesc.Width = 0;
		vSwapChainDesc.BufferDesc.Height = 0;
		vSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		vSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		vSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		vSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		vSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		vSwapChainDesc.OutputWindow = window_handle;
		vSwapChainDesc.SampleDesc.Count = 1;
		vSwapChainDesc.SampleDesc.Quality = 0;
		vSwapChainDesc.Windowed = TRUE;
		vSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		D3D_FEATURE_LEVEL vFeatureLevel;
		D3D_FEATURE_LEVEL vFeatureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

		if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, vFeatureLevelArray, 2, D3D11_SDK_VERSION, &vSwapChainDesc, &swap_chain, &d3d_device, &vFeatureLevel, &device_context) != S_OK)
			return false;

		init_render_target();

		ShowWindow(window_handle, SW_SHOWNORMAL);
		UpdateWindow(window_handle);
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		char windows_directory[MAX_PATH];
		GetWindowsDirectory(windows_directory, MAX_PATH);
		std::string segoeui_dir = (std::string)windows_directory + "\\Fonts\\segoeui.ttf";

		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;

		//io.Fonts->AddFontFromMemoryTTF(&futura, sizeof futura, 19);
		//Globals.norms2 = io.Fonts->AddFontFromMemoryTTF(&norms, sizeof  norms, 18);
		//Globals.input = io.Fonts->AddFontFromMemoryTTF(&futura, sizeof futura, 19);
		//Globals.fut = io.Fonts->AddFontFromMemoryTTF(&futura, sizeof  futura, 19);
		//Globals.malynapis = io.Fonts->AddFontFromMemoryTTF(&futura, sizeof futura, 17);
		//Globals.global3 = io.Fonts->AddFontFromFileTTF(segoeui_dir.c_str(), 17.f);

		return true;
	}
	VOID dest_device() {
		dest_render_target();
		safe_release(swap_chain);
		safe_release(device_context);
		safe_release(d3d_device);
	}

	VOID init_imgui() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();


		//Color
		//ImVec4* colors = ImGui::GetStyle().Colors;
		//colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.14f, 1.00f);
		//colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);
		//colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.14f, 1.00f);
		//colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.19f, 0.94f);
		//colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.30f, 0.30f, 0.40f);
		//colors[ImGuiCol_ButtonHovered] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);
		//colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);
		//colors[ImGuiCol_ButtonActive] = ImVec4(0.95f, 0.39f, 0.39f, 1.00f);
		//colors[ImGuiCol_FrameBgActive] = ImVec4(0.95f, 0.39f, 0.39f, 1.00f);
		//colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.27f, 1.00f);

		//colors[ImGuiCol_SliderGrabActive] = ImVec4(0.18f, 0.18f, 0.19f, 0.94f);
		//colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.30f, 0.30f, 1.00f);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		char windows_directory[MAX_PATH];
		GetWindowsDirectory(windows_directory, MAX_PATH);
		std::string segoeui_dir = (std::string)windows_directory + _("\\Fonts\\segoeui.ttf");

		font1 = io.Fonts->AddFontFromFileTTF("assets/Roboto-Regular.ttf", 16.0f);

		ImFontConfig config;
		config.MergeMode = true;
		config.GlyphMinAdvanceX = 0.0f;


		fontSmall = io.Fonts->AddFontFromFileTTF("assets/Roboto-Regular.ttf", 13.0f);

		fontHead = io.Fonts->AddFontFromFileTTF("assets/Roboto-Regular.ttf", 28.0f);

		config.MergeMode = true;
		config.PixelSnapH = true;
		config.GlyphMinAdvanceX = 10.0f;

		fontHuge = io.Fonts->AddFontFromFileTTF("assets/Roboto-Regular.ttf", 40.0f);

		io.Fonts->AddFontFromMemoryTTF(&futura, sizeof futura, 19);
		ImFont* norms2 = io.Fonts->AddFontFromMemoryTTF(&norms, sizeof  norms, 18);
		ImFont* input = io.Fonts->AddFontFromMemoryTTF(&futura, sizeof futura, 19);
		ImFont* fut = io.Fonts->AddFontFromMemoryTTF(&futura, sizeof  futura, 19);
		ImFont* malynapis = io.Fonts->AddFontFromMemoryTTF(&futura, sizeof futura, 17);
		ImFont* global3 = io.Fonts->AddFontFromFileTTF(segoeui_dir.c_str(), 17.f);
		ImVec4 clear_color = ImColor(255, 255, 255);

		ImGui_ImplWin32_Init(window_handle);
		ImGui_ImplDX11_Init(d3d_device, device_context);
	}
	VOID dest_imgui() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	VOID init_render_target() {
		ID3D11Texture2D* back_buffer = nullptr;
		swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
		if (back_buffer)
			d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
		// todo: error here if it can't get the backbuffer of the render target
		back_buffer->Release();
	}
	VOID dest_render_target() {
		if (!render_target_view)
			return;

		render_target_view->Release();
		render_target_view = NULL;
	}


	void StyleUpdater() {


		ImGuiStyle* Style = &ImGui::GetStyle();
		ImVec4* Colors = Style->Colors;


		Style->Colors[ImGuiCol_WindowBg] = ImColor(10, 10, 10);
		//Style->Colors[ImGuiCol_ChildBg] = ImColor(10, 10, 10);
		Style->Colors[ImGuiCol_FrameBg] = ImColor(15, 15, 15);
		Style->Colors[ImGuiCol_FrameBgHovered] = ImColor(15, 15, 15);
		Style->Colors[ImGuiCol_FrameBgActive] = ImColor(15, 15, 15);
		Style->Colors[ImGuiCol_PopupBg] = ImColor(12, 12, 12);
		Style->Colors[ImGuiCol_Border] = ImColor(26, 26, 26);
		Style->Colors[ImGuiCol_TextDisabled] = ImColor(157, 176, 242);
		Style->Colors[ImGuiCol_ButtonActive] = ImColor(100, 100, 100);
		Style->Colors[ImGuiCol_ScrollbarBg] = ImColor(157, 176, 242);
		Style->Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(157, 176, 242);
		Style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(157, 176, 242);
		Style->WindowRounding = 0;
		Style->FrameRounding = 2;
		Style->FrameBorderSize = 1;
		Style->ChildBorderSize = 1;
	}

	
	int sub_menu = 0;
	int tab_count = 0;
	IDirect3DTexture9* general_texture = nullptr;
	IDirect3DTexture9* projectile_texture = nullptr;
	IDirect3DTexture9* settings_texture = nullptr;
	IDirect3DTexture9* players_texture = nullptr;
	IDirect3DTexture9* world_texture = nullptr;
	IDirect3DTexture9* eye_texture = nullptr;
	IDirect3DTexture9* movement_texture = nullptr;
	IDirect3DTexture9* exploits_texture = nullptr;
	void menu() {
		DWORD dwFlag = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus;
		DWORD color_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop;
		DWORD color_flags2 = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop;
		StyleUpdater();
		ImDrawList* draw;

		static bool open = true;
		ImGui::SetNextWindowSize(ImVec2({ 600,365 }));
		ImGui::SetNextWindowBgAlpha(1.0f);
		ImGui::Begin(_("Menu"), nullptr, dwFlag);
		{

			ImVec2 pos;
			auto p = ImGui::GetWindowPos();
			pos = ImGui::GetWindowPos();
			draw = ImGui::GetWindowDrawList();
			ImGui::PushFont(fontSmall);

			draw->AddRectFilled(ImVec2(p.x + 0, p.y + 20), ImVec2(p.x + 664, p.y + 340), ImColor(8, 8, 8), 5);
			draw->AddLine(ImVec2(p.x + 0, p.y + 20), ImVec2(p.x + 601, p.y + 20), Setup::Lines);
			draw->AddLine(ImVec2(p.x + 0, p.y + 340), ImVec2(p.x + 601, p.y + 340), Setup::Lines);
			draw->AddRectFilledMultiColor(ImVec2(0, p.y + 21), ImVec2(p.x + 235, p.y + 20), ImColor(157, 176, 242, 180), ImColor(157, 176, 242, 0), ImColor(157, 176, 242, 0), ImColor(157, 176, 242, 180));
			draw->AddText(ImVec2(pos.x + 10, pos.y + 344), ImColor(100, 100, 100), _("Game: Fortnite"));
			//draw->AddText(ImVec2(pos.x + 457, pos.y + 344), Setup::NeverMissText, (("Tarkov")));
			//draw->AddText(ImVec2(pos.x + 530, pos.y + 344), ImColor(100, 100, 100), (("for EFT")));
			ImGui::PopFont();

			ImGui::SetCursorPos(ImVec2(20, 35));
			ImGui::BeginChild(_("TABS"), ImVec2(130, 290));//Tabs Page
			{
				ImGui::PushFont(fontSmall);
				ImGui::SetCursorPos({ 5, 5 });
				if (ImGui::Tab(_("Aimbot"), "", !Globals.subtab2)) Globals.subtab2 = 0;

				ImGui::SetCursorPos({ 5, 35 });
				if (ImGui::Tab(_("Visuals"), "", Globals.subtab2 == 1)) Globals.subtab2 = 1;

				ImGui::SetCursorPos({ 5, 65 });
				if (ImGui::Tab(_("Misc"), "", Globals.subtab2 == 2)) Globals.subtab2 = 2;

				ImGui::SetCursorPos({ 5, 95 });
				if (ImGui::Tab(_("Config"), "", Globals.subtab2 == 3)) Globals.subtab2 = 3;

				ImGui::PopFont();
			}
			ImGui::EndChild();

			if (Globals.subtab2 == 0) {
				ImGui::PushFont(fontSmall);
				ImGui::SetCursorPos({ 160, 34 });
				ImGui::MenuChild(("Aimbot"), ImVec2(200, 274));
				{
					ImGui::SetCursorPos({ 15, 15 });
					ImGui::Checkbox(_("Aimbot"), &Globals.LegitAim);
					ImGui::SetCursorPosX({ 15 });
					ImGui::Checkbox(_("Draw FOV"), &Globals.ShowFOV);
					ImGui::SetCursorPosX({ 15 });
					ImGui::Checkbox(_("Draw Crosshair"), &Globals.Crosshair);
					ImGui::SetCursorPosX({ 15 });
					ImGui::Checkbox(_("Smoothing"), &Globals.Smoothing);
				}
				ImGui::EndChild();
				ImGui::PopFont();
				ImGui::PushFont(fontSmall);
				ImGui::SetCursorPos({ 380, 34 });
				ImGui::MenuChild(("Options"), ImVec2(200, 274));
				{
					ImGui::SetCursorPos({ 15, 15 });
					ImGui::SliderInt("FOV Size", &Globals.FOVSize, 1, 999);//needs to be a float
					ImGui::SetCursorPosX({ 15 });
					ImGui::SliderInt("Crosshair Size", &Globals.CrosshairSize, 1, 10);//needs to be a float
					ImGui::SetCursorPosX({ 15 });
					ImGui::SliderInt(_("Smoothing Amount"), &Globals.Smoothness, 1, 10);
				}
				ImGui::EndChild();
				ImGui::PopFont();
			}
			if (Globals.subtab2 == 1) {
				ImGui::PushFont(fontSmall);
				ImGui::SetCursorPos({ 160, 34 });
				ImGui::MenuChild(("Visuals"), ImVec2(200, 274));
				{
					ImGui::SetCursorPos({ 15, 15 });
					ImGui::Checkbox(_("Box"), &Globals.Box);
					ImGui::SetCursorPosX({ 15 });
					ImGui::Checkbox(_("Skeleton"), &Globals.Skeleton);
					ImGui::SetCursorPosX({ 15 });
					ImGui::Checkbox(_("Lines"), &Globals.Lines);
					ImGui::SetCursorPosX({ 15 });
					ImGui::Checkbox(_("Distance Text"), &Globals.DistanceText);
				}
				ImGui::EndChild();
				ImGui::PopFont();
			}
			if (Globals.subtab2 == 2) {
				ImGui::PushFont(fontSmall);
				ImGui::SetCursorPos({ 160, 34 });
				ImGui::MenuChild(("Misc"), ImVec2(200, 274));
				{
					ImGui::SetCursorPos({ 15, 15 });
					ImGui::Checkbox(_("No Recoil"), &Globals.NoRecoil);
				}
				ImGui::EndChild();
				ImGui::PopFont();
			}
			if (Globals.subtab2 == 3) {
				ImGui::PushFont(fontSmall);
				ImGui::SetCursorPos({ 160, 34 });
				ImGui::MenuChild(("Configs"), ImVec2(200, 274));
				{
					ImGui::SetCursorPos({ 15, 15 });
					ImGui::ColorEdit4(_("Visible Box"), Globals.enm_Visible_box_color, color_flags);
					ImGui::SetCursorPosX({ 15 });
					ImGui::ColorEdit4(_("InVisible Box"), Globals.enm_Invisible_box_color, color_flags);
					ImGui::SetCursorPosX({ 15 });
					ImGui::ColorEdit4(_("Visible Line"), Globals.Line_Visible_Col, color_flags);
					ImGui::SetCursorPosX({ 15 });
					ImGui::ColorEdit4(_("InVisible Line"), Globals.Line_Invisible_Col, color_flags);
					ImGui::SetCursorPosX({ 15 });
					ImGui::ColorEdit4(_("Visible Skeleton"), Globals.Skeleton_Visible_Col, color_flags);
					ImGui::SetCursorPosX({ 15 });
					ImGui::ColorEdit4(_("InVisible Skeleton"), Globals.Skeleton_InVisible_Col, color_flags);
					ImGui::SetCursorPosX({ 15 });
					ImGui::ColorEdit4(_("Text Color"), Globals.text_color, color_flags);
				}
				ImGui::EndChild();
				ImGui::PopFont();
			}
		}
		ImGui::End();

	}

	void Menu2() {
		DWORD dwFlag = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus;
		DWORD color_flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop;
		DWORD color_flags2 = ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop;
		StyleUpdater();

		static bool open = true;
		ImGui::SetNextWindowSize(ImVec2(150, 300));
		ImGui::SetNextWindowPos(ImVec2(4, screen_height / 2));
		ImGui::Begin(("Menu"), nullptr, dwFlag);
		ImGui::Checkbox(_("Draw Box"), &Globals.Box);
		ImGui::Checkbox(_("Draw Lines"), &Globals.Lines);
		ImGui::Checkbox(_("Draw Skeleton"), &Globals.Skeleton);
		ImGui::Checkbox(_("Draw Distance"), &Globals.DistanceText);
		ImGui::Checkbox(_("No Recoil (Risk)"), &Globals.NoRecoil);
		ImGui::End();
	}

	bool OnScreen(ImVec2 coords)
	{
		if (coords.x < 0.1f || coords.y < 0.1 || coords.x > screen_width || coords.y > screen_height)
		{
			return false;
		}
		else {
			return true;
		}
	}

	struct BoneConnection {
		Vector3 from;
		Vector3 to;
	};

	auto GetDistance(Vector3 point1, Vector3 point2) -> float
	{
		float distance = sqrt(
			(point1.x - point2.x) * (point1.x - point2.x) +
			(point1.y - point2.y) * (point1.y - point2.y) +
			(point1.z - point2.z) * (point1.z - point2.z)
		);

		auto accuratedistance = distance / 5;
		return accuratedistance;
	}

	const VOID DrawBones(Vector3 from, Vector3 to, Vector3 m_location, ImU32 col)
	{
		auto distance_vector = [](Vector3 point1, Vector3 point2) -> float
			{
				float distance = sqrt((point1.x - point2.x) * (point1.x - point2.x) +
					(point1.y - point2.y) * (point1.y - point2.y) +
					(point1.z - point2.z) * (point1.z - point2.z));
				return distance;
			};


		if (distance_vector(m_location, from) > 118)
			return;

		if (distance_vector(m_location, to) > 118)
			return;

		if (distance_vector(from, to) > 39)
			return;

		//ImVec2 W2S_from;
		//if (!game::WorldToScreen(from, W2S_from))
		//	return;

		//ImVec2 W2S_to;
		//if (!game::WorldToScreen(to, W2S_to))
		//	return;

		//line(ImVec2(W2S_from.x, W2S_from.y), ImVec2(W2S_to.x, W2S_to.y), col, 1.0f);
	}

	const VOID Aim(uintptr_t target_pawn, uintptr_t closest_distance) {
		if (!target_pawn) return;
		uintptr_t mesh = Memory.Read<uintptr_t>(target_pawn + 0x318);
		if (!mesh)
		{
			closest_distance = FLT_MAX;
			target_pawn = NULL;
		}

		int hitbox = 67;

		Vector3 Aim3D = game::GetBoneWithRotation(mesh, hitbox);
		Vector3 Aim2D = game::ProjectWorldToScreen(Aim3D);
		double dx = Aim2D.x - (screen_width / 2);
		double dy = Aim2D.y - (screen_height / 2);
		float dist = sqrtf(dx * dx + dy * dy);
		if (dist > Globals.FOVSize or Aim2D.x == 0 or Aim2D.y == 0)
		{
			closest_distance = FLT_MAX;
			target_pawn = NULL;
		}

		Vector2 screen_center = { (double)screen_width / 2, (double)screen_height / 2 };
		Vector2 target{};
		if (game::IsPlayerVisible(mesh))
		{
			if (Aim2D.x != 0)
			{
				if (Aim2D.x > screen_center.x)
				{
					target.x = -(screen_center.x - Aim2D.x);
					target.x /= Globals.Smoothness;
					if (target.x + screen_center.x > screen_center.x * 2) target.x = 0;
				}
				if (Aim2D.x < screen_center.x)
				{
					target.x = Aim2D.x - screen_center.x;
					target.x /= Globals.Smoothness;
					if (target.x + screen_center.x < 0) target.x = 0;
				}
			}
			if (Aim2D.y != 0)
			{
				if (Aim2D.y > screen_center.y)
				{
					target.y = -(screen_center.y - Aim2D.y);
					target.y /= Globals.Smoothness;
					if (target.y + screen_center.y > screen_center.y * 2) target.y = 0;
				}
				if (Aim2D.y < screen_center.y)
				{
					target.y = Aim2D.y - screen_center.y;
					target.y /= Globals.Smoothness;
					if (target.y + screen_center.y < 0) target.y = 0;
				}
			}
			Input::MoveMouse(target.x, target.y);
		}
	}

	const VOID Esp() {
		if (Globals.ShowFOV) {
			circle(ImVec2(screen_width / 2, screen_height / 2), Globals.FOVSize, ImColor(settings::visuals::fov_color[0], settings::visuals::fov_color[1], settings::visuals::fov_color[2]));
		}
		if (Globals.Crosshair) {
			crosshair(Globals.CrosshairSize, ImColor(settings::visuals::crosshair_color[0], settings::visuals::crosshair_color[1], settings::visuals::crosshair_color[2]));
		}

		Identifiers::uworld = Memory.Read<uintptr_t>(base_address + game::offsets::UWORLD);
		//Identifiers::gobjects = Memory.Read<uintptr_t>(image_base + 0xEA52F68);
		Identifiers::game_instance = Memory.Read<uintptr_t>(Identifiers::uworld + game::offsets::Gameinstance);

		Identifiers::local_players = Memory.Read<uintptr_t>(Memory.Read<uintptr_t>(Identifiers::game_instance + game::offsets::LocalPlayers));

		Identifiers::player_controller = Memory.Read<uintptr_t>(Identifiers::local_players + game::offsets::PlayerController);
		Identifiers::local_pawn = Memory.Read<uintptr_t>(Identifiers::player_controller + game::offsets::LocalPawn);
		Identifiers::root_component = Memory.Read<uintptr_t>(Identifiers::local_pawn + game::offsets::RootComponet);
		Identifiers::player_state = Memory.Read<uintptr_t>(Identifiers::local_pawn + game::offsets::PlayerState);
		Identifiers::game_state = Memory.Read<uintptr_t>(Identifiers::uworld + game::offsets::GameState);
		Identifiers::player_array = Memory.Read<uintptr_t>(Identifiers::game_state + game::offsets::PlayerArray);
		Identifiers::local_relative_location = Memory.Read<Vector3>(Identifiers::root_component + game::offsets::relativelocation);
		Identifiers::closest_distance = FLT_MAX;
		Identifiers::closest_pawn = NULL;

		auto Count = Memory.Read<int>(Identifiers::game_state + (game::offsets::PlayerArray + sizeof(uintptr_t)));
		for (int i = 0; i < Count; i++) {

			uintptr_t CurrentPlayer = Memory.Read<uintptr_t>(Identifiers::player_array + (i * sizeof(uintptr_t)));
			if (!CurrentPlayer) continue;
			//text({ 110.f, 115.f }, "Current Player", ImColor(255, 255, 0));

			uintptr_t CurrentPlayerState = Memory.Read<uintptr_t>(CurrentPlayer + game::offsets::PlayerState);
			//int MyTeam = Memory.Read<int>(Identifiers::player_state + 0x10F1);
			//int CurrentActorTeamId = Memory.Read<int>(CurrentPlayer + 0x10F1);
			//if (MyTeam == CurrentActorTeamId) continue;
		//	text({ 110.f, 135.f }, "Team Id's", ImColor(255, 255, 0));

			uintptr_t CurrentLocalPawnPrivated = Memory.Read<uintptr_t>(CurrentPlayer + game::offsets::Private);
			if (!CurrentLocalPawnPrivated) continue;
			if (CurrentLocalPawnPrivated == Identifiers::local_pawn) continue;
			uintptr_t CurrentMesh = Memory.Read<uintptr_t>(CurrentLocalPawnPrivated + game::offsets::Mesh);
			if (!CurrentMesh) continue;

		//	text({ 110.f, 165.f }, "Mesh Passed", ImColor(255, 255, 0));


			Vector3 Head3D = game::GetBoneWithRotation(CurrentMesh, game::bones::HumanHead);
			Vector3 Head2D = game::ProjectWorldToScreen(Head3D);

			Vector3 Neck3D = game::GetBoneWithRotation(CurrentMesh, game::bones::HumanNeck);
			Vector3 Neck2D = game::ProjectWorldToScreen(Neck3D);

			Vector3 Bottom3D = game::GetBoneWithRotation(CurrentMesh, game::bones::HumanBase);
			Vector3 Feet2D = game::ProjectWorldToScreen(Bottom3D);

			auto CornerHeight = Feet2D.y - Head2D.y;
			auto CornerWidth = CornerHeight / 1.7;
			CornerHeight = abs(CornerHeight * 1.1);
			double dx = Head2D.x - (screen_width / 2);
			double dy = Head2D.y - (screen_height / 2);
			float dist = sqrtf(dx * dx + dy * dy);

			if (dist < Globals.FOVSize && dist < Identifiers::closest_distance)
			{
				Identifiers::closest_distance = dist;
				Identifiers::closest_pawn = CurrentLocalPawnPrivated;
			}

			float distance = Identifiers::local_relative_location.distance(Bottom3D) / 100.0f;
			ImColor BoxCol, LineCol, EyeCol, SkelCol, FilledBoxColor;
			int BottomOffset = 7, TopOffset = 17;

			std::string TitleString = _("");

			if (game::IsPlayerVisible(CurrentMesh)) {
				BoxCol = ImColor(Globals.enm_Visible_box_color[0], Globals.enm_Visible_box_color[1], Globals.enm_Visible_box_color[2]);//Enemy
				LineCol = ImColor(Globals.Line_Visible_Col[0], Globals.Line_Visible_Col[1], Globals.Line_Visible_Col[2]);
				SkelCol = ImColor(Globals.Skeleton_Visible_Col[0], Globals.Skeleton_Visible_Col[1], Globals.Skeleton_Visible_Col[2]);
			}
			else if (!game::IsPlayerVisible(CurrentMesh)) {
				BoxCol = ImColor(settings::visuals::enm_box_color[0], settings::visuals::enm_box_color[1], settings::visuals::enm_box_color[2]);
				LineCol = ImColor(Globals.Line_Invisible_Col[0], Globals.Line_Invisible_Col[1], Globals.Line_Invisible_Col[2]);
				SkelCol = ImColor(Globals.Skeleton_InVisible_Col[0], Globals.Skeleton_InVisible_Col[1], Globals.Skeleton_InVisible_Col[2]);
			}


			if (Globals.Box) {
				//draw_cornered_box(Head2D.x, Head2D.y, CornerWidth, -CornerHeight, BoxCol, 3);
				//box(ImVec2(Head2D.x - (CornerWidth / 2), Head2D.y), CornerWidth, CornerHeight, BoxCol, 1.f);
				box(ImVec2(Feet2D.x - (CornerWidth / 2), Feet2D.y), CornerWidth, -CornerHeight, BoxCol, 1.f);
			}
			if (Globals.Lines) {
				line(ImVec2(Feet2D.x, Feet2D.y), ImVec2(screen_width / 2, screen_height), LineCol, 1.f);
			}
			if (Globals.DistanceText) {
				TitleString.append(std::to_string((int)distance) + _("m"));
				//text(ImVec2(Feet2D.x + (CornerWidth / 2) - (ImGui::CalcTextSize(DistText.c_str()).x / 2), Feet2D.y - 16), DistText.c_str(), DistanceColor, true);
			}
			if (Globals.Skeleton) {

				std::list<int> UpperPart = { game::bones::HumanHead };
				std::list<int> RightArm = { game::bones::HumanNeck, game::bones::HumanChest, game::bones::HumanRUpperarm, game::bones::HumanRForearm1, game::bones::HumanRForearm23, game::bones::HumanRPalm };
				std::list<int> LeftArm = { game::bones::HumanHead, game::bones::HumanChest, game::bones::HumanLUpperarm, game::bones::HumanLForearm1, game::bones::HumanLForearm23, game::bones::HumanLPalm };
				std::list<int> Spine = { game::bones::HumanChest, game::bones::HumanSpine1, game::bones::HumanSpine2, game::bones::HumanSpine3 };

				std::list<int> LowerRight = { game::bones::HumanPelvis, game::bones::HumanRThigh1, game::bones::HumanRThigh2, game::bones::HumanRThigh3, game::bones::HumanRCalf, game::bones::HumanRFoot };
				std::list<int> LowerLeft = { game::bones::HumanPelvis, game::bones::HumanLThigh1, game::bones::HumanLThigh2, game::bones::HumanLThigh3, game::bones::HumanLCalf, game::bones::HumanLFoot };
				std::list<std::list<int>> Skel = { UpperPart, RightArm, LeftArm, Spine, LowerRight, LowerLeft };

				Vector3 Previous(0, 0, 0);
				Vector3 Current;

				auto PelvPos = game::GetBoneWithRotation(CurrentMesh, game::bones::HumanPelvis);

				for (auto a : Skel) {
					Previous = Vector3(0, 0, 0);
					for (int bone : a) {

						Current = Current = bone == game::bones::HumanNeck ? Neck3D : (bone == game::bones::HumanPelvis ? PelvPos : game::GetBoneWithRotation(CurrentMesh, bone));
						if (Previous.x == 0.f) {
							Previous = Current;
							continue;
						}
						Vector3 P1 = game::ProjectWorldToScreen(Previous);
						Vector3 C1 = game::ProjectWorldToScreen(Current);

						line(ImVec2(P1.x, P1.y), ImVec2(C1.x, C1.y), SkelCol, 1.f);
						Previous = Current;
					}
				}

			}
			text(ImVec2(Feet2D.x - (ImGui::CalcTextSize(TitleString.c_str()).x / 2), Feet2D.y), TitleString, ImColor(Globals.text_color[0], Globals.text_color[1], Globals.text_color[2]), true);
		}
		if (Globals.LegitAim) {
			if (GetAsyncKeyState(VK_RBUTTON)) {
				Aim(Identifiers::closest_pawn, Identifiers::closest_distance);
			}
		}
		if (Globals.NoRecoil) {
			if (GetAsyncKeyState(VK_LBUTTON)) {
				Memory.Write<float>(Identifiers::player_controller + 0x68, -0.99f);
			}
		}
	}

	inline bool in_fov(int fovX, int fovY, float fov, int x, int y)
	{
		float dist = sqrt(abs(fovX - x) * abs(fovX - x) + abs(fovY - y) * abs(fovY - y));
		return (dist < fov) ? true : false;
	}

	const VOID MemAim(uintptr_t target_pawn, uintptr_t closest_distance) {
		if (!target_pawn) return;
		uintptr_t mesh = Memory.Read<uintptr_t>(target_pawn + 0x318);
		if (!mesh)
		{
			closest_distance = FLT_MAX;
			target_pawn = NULL;
		}

		int hitbox = 67;//neck

		Vector3 Aim3D = game::GetBoneWithRotation(mesh, hitbox);
		Vector3 Aim2D = game::ProjectWorldToScreen(Aim3D);

		auto CameraPos = game::GetCamera().rotation;
		if (in_fov(screen_width / 2, screen_height, Globals.FOVSize, Aim2D.x, Aim2D.y)) {
			if (Globals.LegitAim) {
				Vector3 AimAngle = AimAngle.calc_angle(CameraPos, Aim3D);

			}
		}
	}

	const VOID render(FLOAT width, FLOAT height) {
		if (exit) {
			ready = true;
			return;
		}

		ImGuiStyle& style = ImGui::GetStyle();

		static bool l_show_fps = true;

		init_draw_list(); // we only do this once a loop
		// double value between 0 && 1 based on the anim time
		double menu_anim = sin((M_PI / 2.0) * (((menu_ticks == 1 ? 0 : 1) * pow(menu_ticks, 2)) / ((double)pow(menu_anim_time - 1, 2))));
		//	circle({ window_width / 2, window_height / 2 }, menu_anim * (window_width * 0.573674 /*Magical constant to calc radius of a 16:9 window found using some basic trig*/), ImColor(11, 11, 11, 80));
		//	style.Alpha = menu_anim;
		//	style.Colors[ImGuiCol_Border] = ImVec4(0.012f, 0.91f, 0.957f, ((breath / (float)breath_anim_time) * .7f) + .3f);

		if (l_show_fps) {
			std::string fps = std::to_string(static_cast<int>(ImGui::GetIO().Framerate));
			text({ window_width - ImGui::CalcTextSize(fps.c_str()).x, 0 }, fps);
		}

		if (GetAsyncKeyState(settings::hotkeys::menu_key) < 0)
		{
			if (settings::menu::menu_key == false)
			{
				settings::menu::menu_key = true;
			}
			else if (settings::menu::menu_key == true)
			{
				settings::menu::menu_key = false;
			}
			Sleep(200);
		}

		if (settings::menu::menu_key) {
			menu();
			ImGui::GetIO().MouseDrawCursor = 1;
		}
		else
		{
			ImGui::GetIO().MouseDrawCursor = 0;
		}

		/*---------Drawing Ents here?---------------*/
		Esp();

	}

	DWORD input_handler() {
		// timers for our animations
		breath_timer.start();
		menu_timer.start();

		for (; !exit;) {
			// for our imgui menu interaction
			ImGuiIO& io = ImGui::GetIO();

			POINT p{};
			GetCursorPos(&p);
			io.MousePos = ImVec2((float)p.x, (float)p.y);

			io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
			io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

			RECT rect{ 0 };
			POINT point{ 0 };
			GetClientRect(window_handle, &rect);
			SetWindowPos(window_handle, HWND_TOPMOST, rect.left, rect.top, rect.right, rect.bottom, SWP_NOREDRAW);

			// panic key
			exit = GetAsyncKeyState(VK_END) & 0x8000;

			// timer for our breathing animation
			int breath_tmr = breath_timer.end();
			if (breath_tmr >= breath_anim_time * 2) {
				breath = breath_anim_time;
				breath_timer.start();
			}
			else if (breath_tmr > breath_anim_time)
				breath = breath_anim_time - (breath_tmr - breath_anim_time);
			else if (breath_tmr <= breath_anim_time)
				breath = breath_tmr;

			// timer for menu circle animation & fade in
			int menu_tmr = menu_timer.end();
			if (GetAsyncKeyState(VK_INSERT) & 0x8000 && menu_tmr > menu_anim_time) {
				show_menu = !show_menu;
				menu_ticks = menu_anim_time;
				menu_timer.start();
			}
			else if (menu_tmr < menu_anim_time)
				menu_ticks = (show_menu ? menu_tmr : menu_anim_time - menu_tmr);
		}
		return 1;
	}

	static DWORD WINAPI start_thread(VOID* Param) {
		c_overlay* This = (c_overlay*)Param;
		return This->input_handler();
	}
public:
	c_overlay() :
		ready(false),
		exit(false),
		window_handle(nullptr),
		window_class({}),
		window_width((FLOAT)GetSystemMetrics(SM_CXSCREEN)),
		window_height((FLOAT)GetSystemMetrics(SM_CYSCREEN)),
		font(nullptr),
		d3d_device(nullptr),
		device_context(nullptr),
		swap_chain(nullptr),
		render_target_view(nullptr),
		draw_list(nullptr),
		input_handler_thread(NULL),
		breath(NULL),
		menu_ticks(NULL)
	{

		window_handle = FindWindowA("CEF-OSC-WIDGET", "NVIDIA GeForce Overlay"); //Name Of Window For Hijacking
		auto getInfo = GetWindowLongA(window_handle, -20);
		auto changeAttributes = SetWindowLongA(window_handle, -20, (LONG_PTR)(getInfo | 0x20));

		MARGINS margins = { -1 };
		margins.cyBottomHeight = margins.cyTopHeight = margins.cxLeftWidth = margins.cxRightWidth = -1;
		DwmExtendFrameIntoClientArea(window_handle, &margins);
		SetLayeredWindowAttributes(window_handle, 0x000000, 0xFF, 0x02);
		SetWindowPos(window_handle, HWND_TOPMOST, 0, 0, 0, 0, 0x0002 | 0x0001);

		if (!window_handle)
			return; // error grabbing nvidia overlay

		font = new s_font();

		if (!init_device())
			return; // error creating device

		init_imgui();
	}

	~c_overlay() {
		dest_imgui();
		dest_device();

		delete font;
	}

	BOOL msg_loop() {
		if (window_handle) {
			MSG msg{ 0 };
			ImVec4 clear_clr = { 0,0,0,0 };

			if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			render(window_width, window_height);

			ImGui::Render();
			device_context->OMSetRenderTargets(1, &render_target_view, NULL);
			device_context->ClearRenderTargetView(render_target_view, (float*)&clear_clr); // might break, if so then put cheat color of type ImVec4 { 0,0,0,0 }
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			swap_chain->Present(1, 0);

			return msg.message != WM_QUIT;
		}
		return false;
	}

	BOOL start_input_handler() {
		return CreateThread(NULL, 0, start_thread, (VOID*)this, 0, &input_handler_thread) != INVALID_HANDLE_VALUE;
	}

	VOID init_draw_list() {
		draw_list = ImGui::GetBackgroundDrawList();
	}

	VOID crosshair(const FLOAT aSize, ImU32 color) {
		draw_list->AddLine({ window_width / 2,window_height / 2 - (aSize + 1) }, { window_width / 2 ,window_height / 2 + (aSize + 1) }, color, .95);
		draw_list->AddLine({ window_width / 2 - (aSize + 1),window_height / 2 }, { window_width / 2 + (aSize + 1), window_height / 2 }, color, .95);
	}

	VOID box(const ImVec2& pos, const FLOAT width, const FLOAT height, ImU32 color, const FLOAT line_width = 2.f) {
		std::array<ImVec2, 4> box_lines{ pos, ImVec2{ pos.x + width,pos.y }, ImVec2{ pos.x + width,pos.y + height }, ImVec2{ pos.x,pos.y + height } };
		draw_list->AddPolyline(box_lines.data(), box_lines.size(), color, true, 2);
	}

	VOID draw_rect(int x, int y, int w, int h, const ImColor color, int thickness) {

		draw_list->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0, thickness);
	}

	VOID DrawBox3D(Vector3 Top, Vector3 Bottom, float A, int Width, int Thickness, ImColor color) {
		int Height = ABS(Top.z - Bottom.z);
		Vector3 B1, B2, B3, B4, T1, T2, T3, T4;

		B1.z = B2.z = B3.z = B4.z = Bottom.z;

		B1.x = Bottom.x + (cos(TORAD(A + 45)) * Width);
		B1.y = Bottom.y + (sin(TORAD(A + 45)) * Width);

		B2.x = Bottom.x + (cos(TORAD(A + 135)) * Width);
		B2.y = Bottom.y + (sin(TORAD(A + 135)) * Width);

		B3.x = Bottom.x + (cos(TORAD(A + 225)) * Width);
		B3.y = Bottom.y + (sin(TORAD(A + 225)) * Width);

		B4.x = Bottom.x + (cos(TORAD(A + 315)) * Width);
		B4.y = Bottom.y + (sin(TORAD(A + 315)) * Width);

		T1.x = B1.x;
		T1.y = B1.y;
		T1.z = B1.z + Height;

		T2.x = B2.x;
		T2.y = B2.y;
		T2.z = B2.z + Height;

		T3.x = B3.x;
		T3.y = B3.y;
		T3.z = B3.z + Height;

		T4.x = B4.x;
		T4.y = B4.y;
		T4.z = B4.z + Height;

		ImVec2 B1D, B2D, B3D, B4D, T1D, T2D, T3D, T4D;

		//if (game::WorldToScreen(B1, B1D) && game::WorldToScreen(B2, B2D) && game::WorldToScreen(B3, B3D) && game::WorldToScreen(B4, B4D) && game::WorldToScreen(T1, T1D) && game::WorldToScreen(T2, T2D) && game::WorldToScreen(T3, T3D) && game::WorldToScreen(T4, T4D)) {

		//	this->line(T1D, B1D, color, 1.f);
		//	this->line(T2D, B2D, color, 1.f);
		//	this->line(T3D, B3D, color, 1.f);
		//	this->line(T4D, B4D, color, 1.f);


		//	this->line(T1D, T2D, color, 1.f);
		//	this->line(T2D, T3D, color, 1.f);
		//	this->line(T3D, T4D, color, 1.f);
		//	this->line(T3D, T4D, color, 1.f);
		//	this->line(T4D, T1D, color, 1.f);


		//	this->line(B1D, B2D, color, 1.f);
		//	this->line(B2D, B3D, color, 1.f);
		//	this->line(B3D, B4D, color, 1.f);
		//	this->line(B4D, B1D, color, 1.f);
		//}
	}

	VOID CornerBox(float x, float y, float x2, float y2, ImColor color, float thickness) {
		float box_width = x2 - x;
		float box_height = y2 - y;

		// top
		this->line(ImVec2(x, y), ImVec2(x + box_width / 4, y), color, thickness);
		this->line(ImVec2(x2, y), ImVec2(x2 - box_width / 4, y), color, thickness);

		// left
		this->line(ImVec2(x, y), ImVec2(x, y + box_height / 3), color, thickness);
		this->line(ImVec2(x, y2), ImVec2(x, y2 - box_height / 3), color, thickness);

		// right
		this->line(ImVec2(x2, y), ImVec2(x2, y + box_height / 3), color, thickness);
		this->line(ImVec2(x2, y2), ImVec2(x2, y2 - box_height / 3), color, thickness);

		// bottom
		this->line(ImVec2(x, y2), ImVec2(x + box_width / 4, y2), color, thickness);
		this->line(ImVec2(x2, y2), ImVec2(x2 - box_width / 4, y2), color, thickness);

		return;
	}

	VOID draw_cornered_box(int x, int y, int w, int h, const ImColor color, int thickness)
	{
		this->line(ImVec2(x, y), ImVec2(x, y + (h / 3)), color, thickness);
		this->line(ImVec2(x, y), ImVec2(x + (w / 3), y), color, thickness);
		this->line(ImVec2(x + w - (w / 3), y), ImVec2(x + w, y), color, thickness);
		this->line(ImVec2(x + w, y), ImVec2(x + w, y + (h / 3)), color, thickness);
		this->line(ImVec2(x, y + h - (h / 3)), ImVec2(x, y + h), color, thickness);
		this->line(ImVec2(x, y + h), ImVec2(x + (w / 3), y + h), color, thickness);
		this->line(ImVec2(x + w - (w / 3), y + h), ImVec2(x + w, y + h), color, thickness);
		this->line(ImVec2(x + w, y + h - (h / 3)), ImVec2(x + w, y + h), color, thickness);
	}


	VOID line(const ImVec2& point1, const ImVec2 point2, ImU32 color, const FLOAT line_width = 2.f) {
		draw_list->AddLine(point1, point2, color, line_width);
	}

	VOID circle(const ImVec2& point, const FLOAT radius, ImU32 color) {
		draw_list->AddCircle(point, radius, color, 200); // num segments is how circular the circle actually is
	}

	void DrawHealthBox(int value, ImVec2 position, float width, float height) {
		float normalizedValue = static_cast<float>(value) / 100.0f;
		float redComponent, greenComponent, blueComponent;

		if (normalizedValue >= 0.9f) {
			greenComponent = 1.0f;
			redComponent = 0.0f;
		}
		else if (normalizedValue >= 0.75f) {
			greenComponent = 1.0f;
			redComponent = (normalizedValue - 0.75f) / 0.15f;
		}
		else if (normalizedValue >= 0.45f) {
			greenComponent = normalizedValue / 0.75f;
			redComponent = 1.0f;
		}
		else {
			greenComponent = 0.0f;
			redComponent = 1.0f;
		}

		blueComponent = 0.0f;

		ImVec4 color = ImVec4(redComponent, greenComponent, blueComponent, 1.0f);

		float coloredBoxHeight = height * normalizedValue;

		ImVec2 whiteBoxTopPosition = ImVec2(position.x, position.y);
		ImVec2 whiteBoxBottomPosition = ImVec2(position.x + width, position.y + height);

		ImVec2 coloredBoxTopPosition = ImVec2(position.x, position.y + height - coloredBoxHeight);
		ImVec2 coloredBoxBottomPosition = ImVec2(position.x + width, position.y + height);

		draw_list->AddRect(whiteBoxTopPosition, whiteBoxBottomPosition, IM_COL32(255, 255, 255, 255));
		draw_list->AddRectFilled(coloredBoxTopPosition, coloredBoxBottomPosition, IM_COL32(color.x * 255, color.y * 255, color.z * 255, color.w * 255));
	}

	VOID text(const ImVec2& pos, const std::string& text, ImU32 color = ImColor(240, 248, 255), bool outline = true) {
		if (outline) {
			draw_list->AddText(font->im_font, 13.f, { pos.x + 1, pos.y }, IM_COL32_BLACK, text.c_str());
			draw_list->AddText(font->im_font, 13.f, { pos.x - 1, pos.y }, IM_COL32_BLACK, text.c_str());
			draw_list->AddText(font->im_font, 13.f, { pos.x, pos.y + 1 }, IM_COL32_BLACK, text.c_str());
			draw_list->AddText(font->im_font, 13.f, { pos.x, pos.y - 1 }, IM_COL32_BLACK, text.c_str());
		}
		draw_list->AddText(font->im_font, 13.f, pos, color, text.c_str());
	}

	VOID radial_gradient(const ImVec2& center, float radius, ImU32 col_in, ImU32 col_out) {
		if (((col_in | col_out) & IM_COL32_A_MASK) == 0 || radius < 0.5f)
			return;

		draw_list->_PathArcToFastEx(center, radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
		const int count = draw_list->_Path.Size - 1;

		unsigned int vtx_base = draw_list->_VtxCurrentIdx;
		draw_list->PrimReserve(count * 3, count + 1);

		const ImVec2 uv = draw_list->_Data->TexUvWhitePixel;
		draw_list->PrimWriteVtx(center, uv, col_in);
		for (int n = 0; n < count; n++)
			draw_list->PrimWriteVtx(draw_list->_Path[n], uv, col_out);

		for (int n = 0; n < count; n++) {
			draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base));
			draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + n));
			draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + ((n + 1) % count)));
		}
		draw_list->_Path.Size = 0;
	}

	BOOL in_screen(const ImVec2& pos) {
		return !(pos.x > window_width || pos.x<0 || pos.y>window_height || pos.y < 0);
	}

public:
	FLOAT window_width, window_height;
	bool exit, ready, show_menu = true;
	INT breath, menu_ticks;
	s_font* font;
};




