// pomelo_node_editor_cmp.
#include "pomelo_node_editor.h"

using namespace std;

vector<pne_node_component::node_attribute> __NODE_COMPONENTS     = {};
vector<pne_node_component::node_attribute> __NODE_COMPONENTS_DCS = {};

// Link management HashMap.
unordered_map<
    int32_t, 
    pne_node_component::node_link_line
> __NODE_LINESLINK = {};

/*
* @param  ImVec4 (in Color, Ahpla += w)
* @return ImVec4
*/
ImVec4 ColorAhplaAdd(ImVec4 color, float w) {
    ImVec4 rteurn_color = ImVec4(
        color.x,
        color.y,
        color.z,
        color.w + w
    );
    return rteurn_color;
}

void PNEGET_node_link_data(
    vector<pne_node_component::node_attribute>&                 node_data,
    unordered_map<int32_t, pne_node_component::node_link_line>& link_data
) {
    node_data = __NODE_COMPONENTS_DCS;
    link_data = __NODE_LINESLINK;
}

namespace pne_node_component {
    // 连接管理系统.
    namespace LINKSYSTEM {

        void component_link_break(
            unordered_map<int32_t, pne_node_component::node_link_line>& linklines,
            int32_t linkline_id
        );

        void component_link_lines(
            vector<pne_node_component::node_attribute>&                 nodes,
            unordered_map<int32_t, pne_node_component::node_link_line>& linklines
        ) {
            // Link unique identification.
            static uint32_t link_count = 1;

            node_link_line create_link = {};
            PARAMTYPE      type_temp[2] = {};

            // IsLink => LinkLines.
            if (ImNodes::IsLinkCreated(&create_link.begin_point, &create_link.end_point)) {

                for (size_t i = 0; i < nodes.size(); ++i) {
                    for (size_t j = 0; j < nodes[i].in_connect.size(); ++j) {

                        // input 连接点所在 node => 互联标识(line_id).
                        if ((nodes[i].in_connect[j].point_number == create_link.begin_point) || 
                            (nodes[i].in_connect[j].point_number == create_link.end_point)) 
                        {
                            // delete old_link.
                            component_link_break(linklines, nodes[i].in_connect[j].linkline_number);
                            nodes[i].in_connect[j].linkline_number = link_count;

                            type_temp[0] = nodes[i].in_connect[j].point_param_type;

                            create_link.link_line_color = nodes[i].in_connect[j].point_color;
                        }
                    }
                    for (size_t j = 0; j < nodes[i].out_connect.size(); ++j) {

                        // output 连接点所在 node => 互联标识(line_id).
                        if ((nodes[i].out_connect[j].point_number == create_link.begin_point) ||
                            (nodes[i].out_connect[j].point_number == create_link.end_point))
                        {
                            nodes[i].out_connect[j].linkline_number.push_back(link_count);
                            type_temp[1] = nodes[i].out_connect[j].point_param_type;
                        }
                    }
                }

                // Equal_types => link.
                if (type_temp[0] == type_temp[1]) {

                    // IsLink => create push.
                    linklines.insert(make_pair(link_count, create_link));
                    link_count += CREATE_UNIQUE_STEP;
                }
            }
        }

        void component_link_break(
            unordered_map<int32_t, pne_node_component::node_link_line>& linklines,
            int32_t linkline_id
        ) {
            // 断开连接线, Erase hash_map value.
            linklines.erase(linkline_id);
        }

        void component_node_delete(
            vector<pne_node_component::node_attribute>& nodes,
            uint32_t nodes_id
        ) {
            // 删除节点, (搜索连线 => 删除节点所有连线 => 删除节点)
            // min O(n) search node_id.
            for (size_t i = 0; i < nodes.size(); ++i) {
                if (nodes[i].node_number == nodes_id) {
                    for (size_t j = 0; j < nodes[i].in_connect.size(); ++j) {

                        // input 删除相关互联.
                        component_link_break(
                            __NODE_LINESLINK,
                            nodes[i].in_connect[j].linkline_number
                        );
                    }
                    for (size_t j = 0; j < nodes[i].out_connect.size(); ++j) {

                        // output 删除相关互联.
                        for (size_t d = 0; d < nodes[i].out_connect[j].linkline_number.size(); ++d) {
                            component_link_break(
                                __NODE_LINESLINK,
                                nodes[i].out_connect[j].linkline_number[d]
                            );
                        }
                    }
                    // delete node.
                    nodes.erase(nodes.begin() + i);
                }
            }
        }

        void component_node_menu() {

            static bool window_create  = false;
            static bool window_hovered = false;

            static int32_t break_linkline_id = -1;
            static int32_t typenode_id       = -1;

            // open close menu.
            if ((ImGui::IsMouseClicked(1) == true) && (window_create == false)) {
                // create window => mouse_pos.
                ImGui::SetNextWindowPos(ImGui::GetMousePos());

                ImNodes::IsLinkHovered(&break_linkline_id);
                ImNodes::IsNodeHovered(&typenode_id);

                window_create = true;
            }
            else { 
                // 点击任意键菜单栏关闭.
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
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 7.2f); // rounded window.
                ImGui::SetNextWindowSize(ImVec2(128.0f, 192.0f));

                ImGui::Begin("Pure Window", nullptr,
                    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

                ImGui::Text(u8"节点菜单");
                if (ImGui::Button(u8"断开连接", ImVec2(112.0f, 32.0f)) 
                    && break_linkline_id > 0
                ) {
                    component_link_break(__NODE_LINESLINK, break_linkline_id);

                    window_create  = false;
                    window_hovered = false;
                }

                if (ImGui::Button(u8"删除节点", ImVec2(112.0f, 32.0f))
                    && typenode_id > 0
                ) {
                    component_node_delete(__NODE_COMPONENTS_DCS, typenode_id);
                   
                    window_create  = false;
                    window_hovered = false;
                }

                if (ImGui::Button(u8"复制节点", ImVec2(112.0f, 32.0f))
                    && typenode_id > 0
                ) {
                    NODETYPE crttype = NULL;
                    // copy node type.
                    for (const auto& __NODE : __NODE_COMPONENTS_DCS)
                        if (__NODE.node_number == typenode_id)
                            crttype = __NODE.node_numtype;

                    for (const auto& __NODE_COMPONENT : __NODE_COMPONENTS) {
                        // create component.
                        if (__NODE_COMPONENT.node_numtype == crttype) {
                            // create copy node_attribute.
                            pne_node_component::node_attribute create_node = __NODE_COMPONENT;
                            pne_node_component::component_allocation(create_node);

                            __NODE_COMPONENTS_DCS.push_back(create_node);
                        }
                    }

                    window_create  = false;
                    window_hovered = false;
                }
                
                window_hovered = ImGui::IsWindowHovered();
                ImGui::End();
                ImGui::PopStyleVar();
            }
        }
    }

    // component_pcs_tick => rendertick_gui.
    void component_pcs_tick() {

        component_render(__NODE_COMPONENTS_DCS, __NODE_LINESLINK);

        LINKSYSTEM::component_link_lines(__NODE_COMPONENTS_DCS, __NODE_LINESLINK);
        LINKSYSTEM::component_node_menu();
    }

#define SRAND_NODE_POSMIN -10000
#define SRAND_NODE_POSMAX 10000

    void component_allocation(node_attribute& node) {
        // nodes & connect unique identification.
        static uint32_t node_count = 1;
        static uint32_t connectpoint_count = 1;
        
        // srand node create_pos.
        float srandpos[2] = {
            float(rand() % (SRAND_NODE_POSMAX - SRAND_NODE_POSMIN + 1) + SRAND_NODE_POSMIN) / 100.0f,
            float(rand() % (SRAND_NODE_POSMAX - SRAND_NODE_POSMIN + 1) + SRAND_NODE_POSMIN) / 100.0f
        };

        node.node_number = node_count;
        // set create pos. [srand]
        ImNodes::SetNodeGridSpacePos(
            node_count, 
            ImVec2(320.0f + srandpos[0], 320.0f + srandpos[1])
        );

        node_count += CREATE_UNIQUE_STEP;

        for (size_t i = 0; i < node.in_connect.size(); ++i) {
            // input connectpoint count.
            node.in_connect[i].point_number = connectpoint_count;
            connectpoint_count += CREATE_UNIQUE_STEP;
        }

        for (size_t i = 0; i < node.out_connect.size(); ++i) {
            // output connectpoint count.
            node.out_connect[i].point_number = connectpoint_count;
            connectpoint_count += CREATE_UNIQUE_STEP;
        }
    }

    // ImNodes Node link_point render.
    namespace linkpoint_draw {

        void input_point_draw(node_in_connectpoint data) {
            // 输入 连接点.
            ImNodes::BeginInputAttribute(data.point_number);

            ImGui::TextColored(
                data.point_color,
                data.point_text.c_str()
            );
            ImNodes::EndInputAttribute();
        }

#define LinesTextLength 6.0f
        // "float textlen" => ImGui::Indent(textlen)
        // SameLine n.f, Line 6.5f + n,f
        void output_point_draw(node_out_connectpoint data, float textlen) { 
            // 输出 连接点.
            ImNodes::BeginOutputAttribute(data.point_number);
            ImGui::Indent(textlen - ImGui::CalcTextSize(data.point_text.c_str()).x);
            
            ImGui::TextColored(
                data.point_color,
                data.point_text.c_str()
            );
            ImNodes::EndOutputAttribute();
        }
    }

    // editor window fixed. 
    bool editor_fixed_move = true;

	void component_render(
        vector<node_attribute>& dwnodes,
        unordered_map<int32_t, pne_node_component::node_link_line>& lines
    ) {
        static bool             editor_no_close = false;
        static ImGuiWindowFlags editor_flags = NULL;

        if (editor_fixed_move) {
            editor_flags = ImGuiWindowFlags_NoMove;    // 固定位置.
            editor_flags |= ImGuiWindowFlags_NoResize; // 固定大小.
            editor_flags |= ImGuiWindowFlags_NoCollapse;
        }
        else
            editor_flags = NULL;

        ImGui::Begin("Pomelo NodesEditor", &editor_no_close, editor_flags);
        ImNodes::BeginNodeEditor();
        // 节点编辑器.
        {
            // ergodic vector<node_attribute>.
            for (const auto& dwnode : dwnodes) {
                // set colors.
                {
                    // 常态颜色. [NodeBar]
                    ImNodes::PushColorStyle(
                        ImNodesCol_TitleBar,
                        ImGui::ColorConvertFloat4ToU32(ImVec4(dwnode.node_color))
                    );
                    // 悬停颜色. [NodeBar]
                    ImNodes::PushColorStyle(
                        ImNodesCol_TitleBarHovered,
                        ImGui::ColorConvertFloat4ToU32(ImVec4(dwnode.node_color_click))
                    );
                    // 点击颜色. [NodeBar]
                    ImNodes::PushColorStyle(
                        ImNodesCol_TitleBarSelected,
                        ImGui::ColorConvertFloat4ToU32(ImVec4(dwnode.node_color_click))
                    );
                }

                ImNodes::BeginNode(dwnode.node_number);
                // 节点积木.
                {
                    ImNodes::BeginNodeTitleBar();
                    ImGui::TextUnformatted(dwnode.node_title.c_str());
                    ImNodes::EndNodeTitleBar();

                    size_t difference = abs((int64_t)dwnode.in_connect.size() - (int64_t)dwnode.out_connect.size());
                    size_t count = min(dwnode.in_connect.size(), dwnode.out_connect.size());

                    for (size_t i = 0; i < count; ++i) {

                        linkpoint_draw::input_point_draw(dwnode.in_connect[i]);

                        ImGui::SameLine(); // 同一行绘制.

                        linkpoint_draw::output_point_draw(dwnode.out_connect[i], 122.0f);
                    }

                    if (count == dwnode.in_connect.size()) {
                        // out > in, residue out.
                        for (size_t i = count; i < count + difference; ++i)
                            linkpoint_draw::output_point_draw(
                                dwnode.out_connect[i],
                                122.0f + 35.0f
                            );
                    }
                    else // out < in, residue in.
                        for (size_t i = count; i < count + difference; ++i)
                            linkpoint_draw::input_point_draw(dwnode.in_connect[i]);

                    // render txeture.
                    if (dwnode.node_tex.node_handle >= 1) {
                        ImGui::Image(
                            (void*)(intptr_t)dwnode.node_tex.node_handle,
                            dwnode.node_tex.node_texdraw_size
                        );
                    }
                }
                ImNodes::EndNode();

                ImNodes::PopColorStyle();
                ImNodes::PopColorStyle();
            }
            
            for (const auto& link : lines) {
                // link lines color = connect.point_color.
                // static.
                ImNodes::PushColorStyle(
                    ImNodesCol_Link,
                    ImGui::ColorConvertFloat4ToU32(link.second.link_line_color)
                );
                // hovered.
                ImNodes::PushColorStyle(
                    ImNodesCol_LinkHovered,
                    ImGui::ColorConvertFloat4ToU32(
                        ColorAhplaAdd(link.second.link_line_color, -0.42f)
                ));
                // selected.
                ImNodes::PushColorStyle(
                    ImNodesCol_LinkSelected,
                    ImGui::ColorConvertFloat4ToU32(
                        ColorAhplaAdd(link.second.link_line_color, -0.42f)
                ));

                // link connect point.
                ImNodes::Link(link.first, link.second.begin_point, link.second.end_point);

                // restore colors.
                {
                    ImNodes::PopColorStyle();
                    ImNodes::PopColorStyle();
                    ImNodes::PopColorStyle();
                }
            }
        }
        ImNodes::EndNodeEditor();
        ImGui::End();
	}
}