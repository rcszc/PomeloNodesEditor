// pomelo_node_editor_style.
#include "pomelo_node_editor.h"

using namespace std;

namespace pne_system_style {

	void style_preset_type(StyleType& number, ImVec4* colors) {

		if (number == STYLE_TYPE_CYAN) {
			// set colors.
			colors[ImGuiCol_Text] =           ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			colors[ImGuiCol_WindowBg] =       ImVec4(0.0f, 0.24f, 0.32f, 1.0f);
			colors[ImGuiCol_TitleBg] =        ImVec4(0.0f, 1.0f, 1.0f, 0.0f);  // progress_bar 0.
			colors[ImGuiCol_TitleBgActive] =  ImVec4(0.0f, 1.0f, 1.0f, 0.0f);  // progress_bar 1.
			colors[ImGuiCol_Button] =         ImVec4(0.0f, 1.0f, 1.0f, 0.24f); // button 0.
			colors[ImGuiCol_ButtonHovered] =  ImVec4(0.0f, 1.0f, 1.0f, 0.58f); // button H
			colors[ImGuiCol_ButtonActive] =   ImVec4(0.0f, 1.0f, 1.0f, 0.76f); // button 1.
			colors[ImGuiCol_CheckMark] =      ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0f, 1.0f, 1.0f, 0.1f);  // ÐüÍ£ÑÕÉ«.
		}

		if (number == STYLE_TYPE_ORANGE) {
			// set colors.
			colors[ImGuiCol_Text] =           ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
			colors[ImGuiCol_WindowBg] =       ImVec4(0.2f, 0.1f, 0.0f, 1.0f);
			colors[ImGuiCol_TitleBg] =        ImVec4(1.0f, 0.5f, 0.0f, 0.0f);  // progress_bar 0.
			colors[ImGuiCol_TitleBgActive] =  ImVec4(1.0f, 0.5f, 0.0f, 0.0f);  // progress_bar 1.
			colors[ImGuiCol_Button] =         ImVec4(1.0f, 0.5f, 0.0f, 0.24f); // button 0.
			colors[ImGuiCol_ButtonHovered] =  ImVec4(1.0f, 0.5f, 0.0f, 0.58f); // button H
			colors[ImGuiCol_ButtonActive] =   ImVec4(1.0f, 0.5f, 0.0f, 0.76f); // button 1.
			colors[ImGuiCol_CheckMark] =      ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 0.5f, 0.0f, 0.1f);  // ÐüÍ£ÑÕÉ«.
		}

		if (number == STYLE_TYPE_PINK) {
			// set colors.
			colors[ImGuiCol_Text] =           ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
			colors[ImGuiCol_WindowBg] =       ImVec4(0.2f, 0.0f, 0.17f, 1.0f);
			colors[ImGuiCol_TitleBg] =        ImVec4(0.7f, 0.0f, 1.0f, 0.0f);  // progress_bar 0.
			colors[ImGuiCol_TitleBgActive] =  ImVec4(0.7f, 0.0f, 1.0f, 0.0f);  // progress_bar 1.
			colors[ImGuiCol_Button] =         ImVec4(0.7f, 0.0f, 1.0f, 0.24f); // button 0.
			colors[ImGuiCol_ButtonHovered] =  ImVec4(0.7f, 0.0f, 1.0f, 0.58f); // button H
			colors[ImGuiCol_ButtonActive] =   ImVec4(0.7f, 0.0f, 1.0f, 0.76f); // button 1.
			colors[ImGuiCol_CheckMark] =      ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.7f, 0.0f, 1.0f, 0.1f);  // ÐüÍ£ÑÕÉ«.
		}
	}
}