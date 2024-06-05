#pragma once
#include "Imgui/imgui.h"
#include <d3d9.h>

namespace colors
{
    inline ImVec4 accent = ImColor(180, 59, 64);

    inline ImVec4 window_background = ImColor(18, 18, 18);
    inline ImVec4 window_outline = ImColor(26, 25, 28);
    inline ImVec4 window_left_side = ImColor(16, 19, 24);
    inline ImVec4 window_right_side = ImColor(18, 18, 18);
    inline ImVec4 window_group_text = ImColor(158, 158, 160);

    inline ImVec4 child_background = ImColor(15, 15, 15);
    inline ImVec4 child_outline_hovered = ImColor(18, 18, 18);
    inline ImVec4 child_outline_default = ImColor(29, 29, 31);
    inline ImVec4 child_text_hovered = ImColor(255, 255, 255);
    inline ImVec4 child_text_default = ImColor(178, 178, 180);

    inline ImVec4 tab_text_active = ImColor(255, 255, 255);
    inline ImVec4 tab_text_hovered = ImColor(178, 178, 180);
    inline ImVec4 tab_text_default = ImColor(86, 86, 88);

    inline ImVec4 checkbox_text = ImColor(255, 255, 255);
    inline ImVec4 checkbox_circle = ImColor(255, 255, 255);
    inline ImVec4 checkbox_rect = ImColor(18, 18, 18);

    inline ImVec4 slider_text = ImColor(255, 255, 255);
    inline ImVec4 slider_value = ImColor(70, 70, 72);
    inline ImVec4 slider_circle = ImColor(158, 59, 58);
    inline ImVec4 slider_rect = ImColor(176, 52, 51);

    inline ImVec4 combo_text = ImColor(255, 255, 255);
    inline ImVec4 combo_arrow = ImColor(62, 61, 64);
    inline ImVec4 combo_outline = ImColor(33, 32, 35);
    inline ImVec4 combo_rect = ImColor(26, 25, 28);
    inline ImVec4 combo_window = ImColor(21, 20, 23);

    inline ImVec4 selectable_text_active = ImColor(255, 255, 255);
    inline ImVec4 selectable_text_default = ImColor(62, 61, 64);

    inline ImVec4 input_rect = ImColor(18, 18, 18);
    inline ImVec4 input_text = ImColor(62, 61, 64);
    inline ImVec4 input_buf = ImColor(255, 255, 255);

    inline ImVec4 button_text = ImColor(255, 255, 255);
    inline ImVec4 button_outline = ImColor(33, 32, 35);
    inline ImVec4 button_rect = ImColor(26, 25, 28);

    inline ImVec4 list_outline = ImColor(33, 32, 35);
    inline ImVec4 list_rect = ImColor(21, 20, 23);

    inline ImVec4 color_outline = ImColor(33, 32, 35);
    inline ImVec4 color_rect = ImColor(21, 20, 23);
    inline ImVec4 color_text = ImColor(255, 255, 255);

    inline ImVec4 key_text = ImColor(255, 255, 255);
    inline ImVec4 key_outline = ImColor(33, 32, 35);
    inline ImVec4 key_rect = ImColor(26, 25, 28);
    inline ImVec4 key_window = ImColor(21, 20, 23);
    inline ImVec4 key_window_outline = ImColor(33, 32, 35);
}
