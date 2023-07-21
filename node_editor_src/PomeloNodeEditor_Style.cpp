// PomeloNodeEditor_Style.
#include "PomeloNodeEditor.h"

using namespace std;

namespace PNE {
	namespace system {
		void style_config::guistyle_cyan(ImVec4* colors) {
			colors[ImGuiCol_Text] =           ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			colors[ImGuiCol_WindowBg] =       ImVec4(0.0f, 0.24f, 0.32f, 1.0f);
			colors[ImGuiCol_TitleBg] =        ImVec4(0.0f, 1.0f, 1.0f, 0.0f);  // progress_bar 0.
			colors[ImGuiCol_TitleBgActive] =  ImVec4(0.0f, 1.0f, 1.0f, 0.0f);  // progress_bar 1.
			colors[ImGuiCol_Button] =         ImVec4(0.0f, 1.0f, 1.0f, 0.24f); // button 0.
			colors[ImGuiCol_ButtonHovered] =  ImVec4(0.0f, 1.0f, 1.0f, 0.58f); // button H
			colors[ImGuiCol_ButtonActive] =   ImVec4(0.0f, 1.0f, 1.0f, 0.76f); // button 1.
			colors[ImGuiCol_CheckMark] =      ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0f, 1.0f, 1.0f, 0.1f);  // 悬停颜色.
		}

		void style_config::guistyle_orange(ImVec4* colors) {
			colors[ImGuiCol_Text] =           ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
			colors[ImGuiCol_WindowBg] =       ImVec4(0.2f, 0.1f, 0.0f, 1.0f);
			colors[ImGuiCol_TitleBg] =        ImVec4(1.0f, 0.5f, 0.0f, 0.0f);  // progress_bar 0.
			colors[ImGuiCol_TitleBgActive] =  ImVec4(1.0f, 0.5f, 0.0f, 0.0f);  // progress_bar 1.
			colors[ImGuiCol_Button] =         ImVec4(1.0f, 0.5f, 0.0f, 0.24f); // button 0.
			colors[ImGuiCol_ButtonHovered] =  ImVec4(1.0f, 0.5f, 0.0f, 0.58f); // button H
			colors[ImGuiCol_ButtonActive] =   ImVec4(1.0f, 0.5f, 0.0f, 0.76f); // button 1.
			colors[ImGuiCol_CheckMark] =      ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 0.5f, 0.0f, 0.1f);  // 悬停颜色.
		}

		void style_config::guistyle_pink(ImVec4* colors) {
			colors[ImGuiCol_Text] =           ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
			colors[ImGuiCol_WindowBg] =       ImVec4(0.2f, 0.0f, 0.17f, 1.0f);
			colors[ImGuiCol_TitleBg] =        ImVec4(0.7f, 0.0f, 1.0f, 0.0f);  // progress_bar 0.
			colors[ImGuiCol_TitleBgActive] =  ImVec4(0.7f, 0.0f, 1.0f, 0.0f);  // progress_bar 1.
			colors[ImGuiCol_Button] =         ImVec4(0.7f, 0.0f, 1.0f, 0.24f); // button 0.
			colors[ImGuiCol_ButtonHovered] =  ImVec4(0.7f, 0.0f, 1.0f, 0.58f); // button H
			colors[ImGuiCol_ButtonActive] =   ImVec4(0.7f, 0.0f, 1.0f, 0.76f); // button 1.
			colors[ImGuiCol_CheckMark] =      ImVec4(0.7f, 0.0f, 1.0f, 1.0f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.7f, 0.0f, 1.0f, 0.1f);  // 悬停颜色.
		}

		void style_config::StyleConfig_panel() {

			ImGuiStyle* config_style = &ImGui::GetStyle();
			ImVec4*     config_colors = config_style->Colors;

			ImGui::ColorEdit4(u8"文本颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_Text]));
			ImGui::ColorEdit4(u8"窗口背景颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_WindowBg]));
			ImGui::ColorEdit4(u8"标题栏颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_TitleBg]));
			ImGui::ColorEdit4(u8"活动标题栏颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_TitleBgActive]));
			ImGui::ColorEdit4(u8"按钮颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_Button]));
			ImGui::ColorEdit4(u8"悬停按钮颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_ButtonHovered]));
			ImGui::ColorEdit4(u8"按钮按下颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_ButtonActive]));
			ImGui::ColorEdit4(u8"选择标记颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_CheckMark]));
			ImGui::ColorEdit4(u8"悬停框背景颜色", reinterpret_cast<float*>(&config_colors[ImGuiCol_FrameBgHovered]));
		}
	}
}