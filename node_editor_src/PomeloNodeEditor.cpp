// PomeloNodeEditor.
#include <iostream>
#include <windows.h>
#include <chrono>

#include "PomeloNodeEditor.h"

using namespace std;

#define PNE_GETTIME            std::chrono::system_clock::now()
#define PNE_GETTIEM_TIME_SIZE  std::chrono::duration_cast<std::chrono::milliseconds>(PNE_GETTIME.time_since_epoch()).count()
#define PNE_TIMESEED_SRAND     srand((uint32_t)PNE_GETTIEM_TIME_SIZE)

// 计算节点组件大小. [20230721]
inline size_t AttributeSize(const pne_datadefine::node_attribute& node) {
    size_t return_size = NULL;

    return_size += sizeof(pne_datadefine::node_attribute);
    return_size += sizeof(pne_datadefine::node_texture);

    return_size += node.in_connect.size() * sizeof(pne_datadefine::node_in_connectpoint);
    return_size += node.out_connect.size() * sizeof(pne_datadefine::node_out_connectpoint);

    return_size += node.node_title.size();

    for (const auto& in_point : node.in_connect)
        return_size += in_point.point_text.size();

    for (const auto& out_point : node.out_connect)
        return_size += out_point.point_text.size();

    return return_size;
}

namespace PNE {

	void EdiorGUI::editor_imgui_init(GLFWwindow* winobj) {
        IMGUI_CHECKVERSION();

        // setup imgui context.
        ImGui::CreateContext();
        ImGuiIO& GuiIO = ImGui::GetIO(); (void)GuiIO;

        // 启用 键盘&手柄 交互.
        GuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard controls.
        GuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // enable gamepad controls.

        // setup imgui style.
        ImGui::StyleColorsDark();
        {
            ImGuiStyle* config_style = &ImGui::GetStyle();

            if (IMGUIPARAM.style_typenumber == STYLE_TYPE_CYAN)   guistyle_cyan(config_style->Colors);
            if (IMGUIPARAM.style_typenumber == STYLE_TYPE_ORANGE) guistyle_orange(config_style->Colors);
            if (IMGUIPARAM.style_typenumber == STYLE_TYPE_PINK)   guistyle_pink(config_style->Colors);
        }

        // init set fonts.
        auto SetFonts = ImGui::GetIO().Fonts;
        SetFonts->AddFontFromFileTTF
        (
            IMGUIPARAM.gui_fonts_path,
            IMGUIPARAM.gui_fonts_size,
            NULL,
            SetFonts->GetGlyphRangesChineseFull()
        );

        // setup platform & renderer backends.
        ImGui_ImplGlfw_InitForOpenGL(winobj, true);
        ImGui_ImplOpenGL3_Init(IMGUIPARAM.gui_shader_version);
	}

    void EdiorGUI::editor_imnodes_init() {
        PNE_TIMESEED_SRAND; // set srand seed.

        // setup ImNodes context.
        ImNodes::CreateContext();
        ImNodes::StyleColorsDark();
    }

	EdiorGUI::EdiorGUI(
        GLFWwindow*                         win_object,
        pne_datadefine::pne_init::imguiinit init_param
    ) {
        IMGUIPARAM = init_param;

        editor_imgui_init(win_object);
        editor_imnodes_init();
	}

    void EdiorGUI::RenderTick(function<void()> render_gui) {
        // start imgui frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // RenderEditor 
        // render node state False: Not Draw&Compute.
        if (RenderEditor)
        {
            CompTick_Links(NODE_COMPONENTS_DCS, NODE_LINESLINK, false, ImVec2());
            nodecmp_render(NODE_COMPONENTS_DCS, NODE_LINESLINK);
        }
        render_gui(); // draw imgui function.

        // render imgui.
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void EdiorGUI::EndClose() {
        // End Close ImGui.
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        // Free Context.
        ImNodes::DestroyContext();

        // Free Texture Data.
        nodecmp_freetexhd(TEXTURE_HANDLE);
    }

    bool EdiorGUI::PushNodeComponent(StrText json_path) {
        bool rteurn_state = true;

        NODE_COMPONENTS.push_back(
            nodecmp_loadconfig(
            json_path, 
            rteurn_state
            )
        );
        return rteurn_state;
    }

    void EdiorGUI::SystemNodeMenu() {

        static bool window_create  = false;
        static bool window_hovered = false;

        static int32_t break_linkline_id = -1;
        static int32_t typenode_id       = -1;

        // open close menu.
        if ((ImGui::IsMouseClicked(1) == true) && (window_create == false)) {
            // create window => mouse_pos.
            ImGui::SetNextWindowPos(ImGui::GetMousePos());

            // 获取最后点击 节点&连线 ID
            ImNodes::IsLinkHovered(&break_linkline_id);
            ImNodes::IsNodeHovered(&typenode_id);

            window_create = true;
        }
        else {
            if (((ImGui::IsMouseClicked(0) == true) || (ImGui::IsMouseClicked(1) == true))
                && (window_create == true)
                && (window_hovered == false))
            {
                window_create  = false;
                window_hovered = false;

                break_linkline_id = -1;
                typenode_id       = -1;
            }
        }

        if (window_create) {
            // rounded window.
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 7.2f);
            ImGui::SetNextWindowSize(ImVec2(128.0f, 192.0f));

            ImGui::Begin(
                "Menu Window", nullptr,
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoResize | 
                ImGuiWindowFlags_NoMove
            );
            ImGui::Text(u8"节点菜单");

            if (ImGui::Button(u8"断开连接", ImVec2(112.0f, 32.0f))
                && break_linkline_id > 0
            ) {
                component_link_break(NODE_LINESLINK, break_linkline_id);

                window_create  = false;
                window_hovered = false;
            }

            if (ImGui::Button(u8"删除节点", ImVec2(112.0f, 32.0f))
                && typenode_id > 0
            ) {
                component_node_delete(NODE_COMPONENTS_DCS, NODE_LINESLINK, typenode_id);

                window_create  = false;
                window_hovered = false;
            }

            if (ImGui::Button(u8"复制节点", ImVec2(112.0f, 32.0f))
                && typenode_id > 0
                ) {
                NODE_TYPE crttype = NULL;

                // copy node type.
                for (const auto& NODE : NODE_COMPONENTS_DCS)
                    if (NODE.node_number == typenode_id)
                        crttype = NODE.node_numtype;

                for (const auto& NODE_COMPONENT : NODE_COMPONENTS) {
                    // create component.
                    if (NODE_COMPONENT.node_numtype == crttype) {
                        // create copy node_attribute.
                        pne_datadefine::node_attribute create_node = NODE_COMPONENT;
                        component_node_init(create_node);

                        ImNodes::SetNodeGridSpacePos(create_node.node_number, ImGui::GetMousePos());

                        NODE_COMPONENTS_DCS.push_back(create_node);
                    }
                }

                window_create = false;
                window_hovered = false;
            }

            window_hovered = ImGui::IsWindowHovered();
            ImGui::End();
            ImGui::PopStyleVar();
        }
    }

    void EdiorGUI::SystemCompMenu() {

        static ImGuiWindowFlags window_flags    = NULL;
        static bool             window_no_close = false;
        static bool             WindowNoMove    = true;

        if (WindowNoMove) {
            window_flags = ImGuiWindowFlags_NoMove;    // 固定位置.
            window_flags |= ImGuiWindowFlags_NoResize; // 固定大小.
            window_flags |= ImGuiWindowFlags_NoCollapse;
        }
        else
            window_flags = NULL;

        ImGui::Begin("Node Components List", &window_no_close, window_flags);
        {
            ImGui::Checkbox("Fixed Panel", &WindowNoMove);
            ImGui::Checkbox("Fixed Editor", &node_window_fixed);

            ImGui::Text("Render ImGui: %.2f fps", ImGui::GetIO().Framerate);
            ImGui::Text("");

            // Style FrameRounding.
            ImGui::GetStyle().FrameRounding = 7.2f;
            
            // Components List.
            for (const auto& NODE_COMPONENT : NODE_COMPONENTS) {
                // create components.
                if (ImGui::Button(NODE_COMPONENT.node_title.c_str(), ImVec2(220.0f, 32.0f))) {
                    // create copy node_attribute.
                    pne_datadefine::node_attribute create_node = NODE_COMPONENT;

                    // craete initialization.
                    component_node_init(create_node);

                    // load => NODE_COMPONENTS => NODE_COMPONENTS_DCS
                    NODE_COMPONENTS_DCS.push_back(create_node);
                }
            }

            ImGui::Text(u8"连接数量: %u lines", NODE_LINESLINK.size());
            ImGui::Text(u8"节点数量: %u nodes", NODE_COMPONENTS_DCS.size());

            if (ImGui::Button(u8"清空所有节点")) {

                int result = 
                    MessageBox(NULL, L"确定删除所有节点?", L"Warring", MB_YESNOCANCEL | MB_ICONQUESTION);
                if (result == IDYES) {
                    // "YES" delete.
                    nodecmp_cleardcsdata();
                }
            }
            ImGui::Text("");

            // color set.
            if (ImGui::CollapsingHeader(u8"color config")) {

                StyleConfig_panel();
            }
        }
        ImGui::End();
    }

    IODATA::NodesLines EdiorGUI::GetNodesLinesData() {
        IODATA::NodesLines return_data = {};

        return_data.nodes = NODE_COMPONENTS_DCS;
        return_data.lines = NODE_LINESLINK;

        // calc size.
        for (const auto& nodesz : NODE_COMPONENTS_DCS)
            return_data.NodesLines_datasize += AttributeSize(nodesz);

        for (const auto& lines : NODE_LINESLINK) {
            return_data.NodesLines_datasize += sizeof(lines.first);
            return_data.NodesLines_datasize += sizeof(lines.second);
        }

        return return_data;
    }

    void EdiorGUI::ReadNodeLinesData(
        std::vector<IODATA::SaveNodeData>& nodes,
        std::vector<IODATA::SaveLineData>& lines
    ) {
        int32_t MaxNodeUnique = -1;
        int32_t MaxConnUnique = -1;

        // 清理源数据 => 加载新 节点&连线 数据
        nodecmp_cleardcsdata();

        for (const auto& crtnode : nodes) {

            for (const auto& NODE_COMPONENT : NODE_COMPONENTS) {
                // create component.
                if (NODE_COMPONENT.node_numtype == crtnode.node_types) {
                    // create copy node_attribute.
                    pne_datadefine::node_attribute create_node = NODE_COMPONENT;

                    create_node.node_number = crtnode.node_unique;

                    if (MaxNodeUnique < create_node.node_number)
                        MaxNodeUnique = create_node.node_number;
                    // 赋值连接点标识符.
                    {
                        for (size_t i = 0; i < create_node.in_connect.size(); ++i) {
                            create_node.in_connect[i].point_number = crtnode.in_connect_unique[i];

                            if (MaxConnUnique < crtnode.in_connect_unique[i])
                                MaxConnUnique = crtnode.in_connect_unique[i];
                        }

                        for (size_t i = 0; i < create_node.out_connect.size(); ++i) {
                            create_node.out_connect[i].point_number = crtnode.out_connect_unique[i];

                            if (MaxConnUnique < crtnode.out_connect_unique[i])
                                MaxConnUnique = crtnode.out_connect_unique[i];
                        }
                    }
                    ImNodes::SetNodeGridSpacePos(create_node.node_number, crtnode.node_position);

                    NODE_COMPONENTS_DCS.push_back(create_node);
                }
            }
        }

        // 重置节点标识计数器.
        uniquenode_count = MaxNodeUnique + 1;
        uniquepoint_count = MaxConnUnique + 1;

        // 创建连接数据.
        for (const auto& crtline : lines) {

            CompTick_Links(
                NODE_COMPONENTS_DCS,
                NODE_LINESLINK,
                true,
                ImVec2((float)crtline.begin_num, (float)crtline.end_num)
            );
        }
    }
}