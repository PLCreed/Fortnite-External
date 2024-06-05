#pragma once
namespace settings
{
	namespace aimbot
	{
		bool aimbot_ViewAngles = false;
		bool aimbot_Mouse = false;
		bool smooth = true;
		bool skip_knocked = false;

		bool line_aim = false;

		float smooth_value = 2.0f;
		bool no_recoil = true;
		int recoil_value = 91;


		bool crosshair = true;
		float crosshair_size = 6.0f;

		bool check_fov = true;
		float fov = 65;

		float aim_distance = 45;

		int hitbox = 0;
		static const char* hitboxes[]{ "Head", "Neck", "Chest" };

		bool check_team = true;
		bool check_visibility = false;
		bool check_health = false;
		bool aimbot_lock = false;
		bool predict_player_pos = false;
	}

	namespace visuals
	{
		bool spectator_count = false;
		bool Debugger = false;

		namespace Team {
			bool enabled = false;
			bool box = false;
			bool Box3D = false;
			bool skel = false;
			bool health = false;
			bool shield = false;
			bool lines = false;
			bool EyeSight = false;
			bool team = false;
			bool show_health = false;
			bool show_name = false;
			int hitbox_radius = 5;
			bool fov_circle = false;
			bool show_distance = false;
		}

		namespace Enemy {
			bool enabled = false;
			bool box = false;
			bool Box3D = false;
			bool skel = false;
			bool health = false;
			bool shield = false;
			bool lines = false;
			bool EyeSight = false;
			bool team = false;
			bool show_health = false;
			bool show_name = false;
			int hitbox_radius = 5;
			bool fov_circle = false;
			bool show_distance = false;
		}

		bool tam_glow = false;
		bool enm_glow = false;

		int boxint = 0;
		static const char* bot_type[]{ "2D-Boxes", "3D-Boxes", "Cornered Boxes", "Filled Boxes" };

		int max_distance = 250;
		int box_type = 0;
		//float enm_box_color[4] = { 180.f, 59.f, 64.f, 1.f };
		float enm_box_color[4] = { 180.f, 59.f, 64.f, 1.0f };

		float enm_glow_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float enm_line_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float enm_eyesight_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float enm_skel_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };


		float tnm_box_color[4] = { 0.f, 89.f, 255.f, 1.0f };
		float tnm_line_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float tnm_glow_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float tnm_eyesight_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float tnm_skel_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };

		float hitbox_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float crosshair_color[4] = { 97.f, 97.f, 189.f, 255.f };
		float fov_color[4] = { 1.0f , 1.0f , 1.0f, 1.0f };
		float pred_color[4] = { 180.f, 59.f, 64.f, 1.0f };

		int color_type = 0;

		float PDistanceAmount = 75.f;
		float LDistanceAmount = 50.f;
		float ADistanceAmount = 1000.f;
	}

	namespace loot_visuals {
		bool enabled = false;
		bool box = false;
		bool distance = false;
		bool snaplines = false;
		bool title = false;

		int DistAmt = 1;
		int Rarint = 0;
		static const char* RarityType[]{ "Common", "Rare", "Epic", "Legendary" };
	}

	namespace misc
	{
		float closest_player_distance = 1.f;
	}

	namespace menu
	{
		bool menu_key = false;
		bool overlay_fps = true;
		bool debug = false;
		int box_thick = 1;
		float rainbow_speed = 0.25f;
		float crosshair_size = 1.f;
	}

	namespace hotkeys
	{
		int aim_key = VK_XBUTTON1;
		int menu_key = VK_INSERT;
	}


}
