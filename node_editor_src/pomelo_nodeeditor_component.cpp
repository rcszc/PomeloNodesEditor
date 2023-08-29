
// PomeloNodeEditor_Component.
#include <iostream>
#include <fstream>
#include <filesystem>
#include <document.h> // RapidJSON

#include "pomelo_nodeeditor.h"

using namespace std;

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

namespace NODE_CMP {

    // ImNodes Node connect point render.
    namespace linkpoint_draw {

        inline void input_point_draw(
            pne_datadefine::node_in_connectpoint data
        ) {
            ImNodes::BeginInputAttribute(data.point_number);

            ImGui::TextColored(
                data.point_color,
                data.point_text.c_str()
            );
            ImNodes::EndInputAttribute();
        }

        // DRAW: 因为同一行, ImGui对齐计算为: 缩进(节点宽度 - 输出连接文本宽度 - 输入文本宽度)
        inline void output_point_draw(
            pne_datadefine::node_out_connectpoint data,
            float                                 textlen,
            StrText                               textin
        ) {
            ImNodes::BeginOutputAttribute(data.point_number);
            ImGui::Indent(textlen - ImGui::CalcTextSize(data.point_text.c_str()).x - ImGui::CalcTextSize(textin).x);

            ImGui::TextColored(
                data.point_color,
                data.point_text.c_str()
            );
            ImNodes::EndOutputAttribute();
        }
    }

    pne_datadefine::node_attribute NodeComponent::nodecmp_loadconfig(
        StrText jsonfile,
        bool& state
    ) {
        pne_datadefine::node_attribute return_node_cmp = {};

        ifstream read_jsonfile(jsonfile);
        if (!read_jsonfile) {

            // Failed read json file.
            state = false;
        }
        else {
            // RapidJSON read json.
            string jsonString((std::istreambuf_iterator<char>(read_jsonfile)), std::istreambuf_iterator<char>());
            read_jsonfile.close();

            // 解析 json string.
            rapidjson::Document jsondoc;
            jsondoc.Parse(jsonString.c_str());

            return_node_cmp.node_number = NULL;

            if (!jsondoc.HasParseError() && jsondoc.IsObject()) {

                // 解析 node_color "node color" 
                if (jsondoc.HasMember("node color") && jsondoc["node color"].IsArray()) {

                    const auto& node_color = jsondoc["node color"];

                    return_node_cmp.node_color.x = node_color[0].GetFloat();
                    return_node_cmp.node_color.y = node_color[1].GetFloat();
                    return_node_cmp.node_color.z = node_color[2].GetFloat();
                    return_node_cmp.node_color.w = node_color[3].GetFloat();
                }

                // 解析 node_color_click "node color click"
                if (jsondoc.HasMember("node color click") && jsondoc["node color click"].IsArray()) {

                    const auto& node_colorclick = jsondoc["node color click"];

                    return_node_cmp.node_color_click.x = node_colorclick[0].GetFloat();
                    return_node_cmp.node_color_click.y = node_colorclick[1].GetFloat();
                    return_node_cmp.node_color_click.z = node_colorclick[2].GetFloat();
                    return_node_cmp.node_color_click.w = node_colorclick[3].GetFloat();
                }

                // 解析 node_title "node title"
                if (jsondoc.HasMember("node title") && jsondoc["node title"].IsString()) {

                    const auto& node_title = jsondoc["node title"];

                    return_node_cmp.node_title = node_title.GetString();
                }

                // 解析 node_width "node width"
                if (jsondoc.HasMember("node width") && jsondoc["node width"].IsFloat()) {

                    const auto& node_width = jsondoc["node width"];

                    return_node_cmp.node_width = node_width.GetFloat();
                }

                // 解析 node_tex "node image"
                if (jsondoc.HasMember("node image") && jsondoc["node image"].IsString()) {

                    const auto& node_tex = jsondoc["node image"];

                    // 加载纹理 => OGL句柄.
                    return_node_cmp.node_tex.node_handle =
                        load_image_texture(node_tex.GetString()).texture_handle;
                }

                // 解析 node_texsz "node image dwsz" 纹理绘制尺寸.
                if (jsondoc.HasMember("node image size") && jsondoc["node image size"].IsArray()) {

                    const auto& node_texdw_size = jsondoc["node image size"];

                    return_node_cmp.node_tex.node_texdraw_size.x = node_texdw_size[0].GetFloat();
                    return_node_cmp.node_tex.node_texdraw_size.y = node_texdw_size[1].GetFloat();
                }

                // 解析 node_sample_sw "node sample switch"
                if (jsondoc.HasMember("node sample switch") && jsondoc["node sample switch"].IsBool()) {

                    const auto& node_sample_sw = jsondoc["node sample switch"];

                    return_node_cmp.node_smp.node_sample_sw = node_sample_sw.GetBool();

                    // 开启采样器: 指针指向采样值.
                    if (return_node_cmp.node_smp.node_sample_sw) {

                        return_node_cmp.node_smp.timer_tmp = chrono::steady_clock::now(); // 计时器初始化.
                       // return_node_cmp.node_smp.smp_pointer = &return_node_cmp.node_smp.node_sample_value;
                    }
                    else
                        return_node_cmp.node_smp.smp_pointer = nullptr;
                }

                // 解析 node_sample_timer "node sample timer"
                if (jsondoc.HasMember("node sample timer") && jsondoc["node sample timer"].IsFloat()) {

                    const auto& node_sample_timer = jsondoc["node sample timer"];

                    return_node_cmp.node_smp.node_sample_timer = node_sample_timer.GetFloat();
                }

                // 解析 execute_path_switch "node execute path switch"
                if (jsondoc.HasMember("node execute path switch") && jsondoc["node execute path switch"].IsBool()) {

                    const auto& execute_path_switch = jsondoc["node execute path switch"];

                    return_node_cmp.execute_path_switch = execute_path_switch.GetBool();
                }

                // 解析 in_point "input point"
                if (jsondoc.HasMember("input point") && jsondoc["input point"].IsArray()) {

                    const auto& input_point = jsondoc["input point"].GetArray();

                    for (const auto& point : input_point) {
                        pne_datadefine::node_in_connectpoint in_connect_point;

                        in_connect_point.point_number = NULL;

                        if (point.HasMember("point color") && point["point color"].IsArray()) {

                            const auto& pointColor = point["point color"].GetArray();

                            in_connect_point.point_color.x = pointColor[0].GetFloat();
                            in_connect_point.point_color.y = pointColor[1].GetFloat();
                            in_connect_point.point_color.z = pointColor[2].GetFloat();
                            in_connect_point.point_color.w = pointColor[3].GetFloat();
                        }

                        if (point.HasMember("point text") && point["point text"].IsString()) {

                            const auto& point_text = point["point text"];

                            in_connect_point.point_text = point_text.GetString();
                        }

                        if (point.HasMember("point param type") && point["point param type"].IsInt()) {

                            const auto& point_type = point["point param type"].GetInt();

                            in_connect_point.point_param_type = (PARAM_TYPE)point_type;
                        }
                        return_node_cmp.in_connect.push_back(in_connect_point);
                    }
                }

                // 解析 out_point "output point"
                if (jsondoc.HasMember("output point") && jsondoc["output point"].IsArray()) {

                    const auto& output_point = jsondoc["output point"].GetArray();

                    for (const auto& point : output_point) {
                        pne_datadefine::node_out_connectpoint out_connect_point;

                        out_connect_point.point_number = NULL;

                        if (point.HasMember("point color") && point["point color"].IsArray()) {

                            const auto& point_color = point["point color"].GetArray();

                            out_connect_point.point_color.x = point_color[0].GetFloat();
                            out_connect_point.point_color.y = point_color[1].GetFloat();
                            out_connect_point.point_color.z = point_color[2].GetFloat();
                            out_connect_point.point_color.w = point_color[3].GetFloat();
                        }

                        if (point.HasMember("point text") && point["point text"].IsString()) {

                            const auto& point_text = point["point text"];

                            out_connect_point.point_text = point_text.GetString();
                        }

                        if (point.HasMember("point param type") && point["point param type"].IsInt()) {

                            const auto& pointType = point["point param type"].GetInt();

                            out_connect_point.point_param_type = (PARAM_TYPE)pointType;
                        }
                        return_node_cmp.out_connect.push_back(out_connect_point);
                    }
                }

                // 解析 node_numtype "node type"
                if (jsondoc.HasMember("node type") &&
                    jsondoc["node type"].IsUint()
                    ) {
                    return_node_cmp.node_numtype = jsondoc["node type"].GetUint();
                }
            }
            else
                state = false;
            state = true;
        }
        return return_node_cmp;
    }

    void NodeComponent::nodecmp_render(
        vector<pne_datadefine::node_attribute>& dwnodes,
        unordered_map<int32_t, pne_datadefine::node_link_line>& lines
    ) {
        static bool             editor_no_close = false;
        static ImGuiWindowFlags editor_flags = NULL;

        if (node_window_fixed) {
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
            for (auto& dwnode : dwnodes) {
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

                    // 获取节点位置.
                    dwnode.node_position = ImNodes::GetNodeGridSpacePos(dwnode.node_number);

                    size_t difference = abs((int64_t)dwnode.in_connect.size() - (int64_t)dwnode.out_connect.size());
                    size_t count = min(dwnode.in_connect.size(), dwnode.out_connect.size());

                    for (size_t i = 0; i < count; ++i) {

                        linkpoint_draw::input_point_draw(dwnode.in_connect[i]);

                        ImGui::SameLine(); // in/out 同一行绘制.

                        linkpoint_draw::output_point_draw(
                            dwnode.out_connect[i],
                            dwnode.node_width,
                            dwnode.in_connect[i].point_text.c_str()
                        );
                    }

                    if (count == dwnode.in_connect.size()) {
                        // out > in, residue out.
                        for (size_t i = count; i < count + difference; ++i)
                            linkpoint_draw::output_point_draw(
                                dwnode.out_connect[i],
                                dwnode.node_width + 8.0f,
                                ""
                            );
                    }
                    else // out < in, residue in.
                        for (size_t i = count; i < count + difference; ++i)
                            linkpoint_draw::input_point_draw(dwnode.in_connect[i]);

                    // render texture.
                    if (dwnode.node_tex.node_handle >= 1) {
                        ImGui::Image(
                            (void*)(intptr_t)dwnode.node_tex.node_handle,
                            dwnode.node_tex.node_texdraw_size
                        );
                    }

                    // render sample.
                    // switch && timer.
                    if (dwnode.node_smp.node_sample_sw &&
                        chrono::duration_cast<chrono::milliseconds>(
                            chrono::steady_clock::now() - dwnode.node_smp.timer_tmp
                        ).count() > int64_t(dwnode.node_smp.node_sample_timer)
                        ) {
                        // sample queue.
                        for (size_t i = SMP_BUFFER_LENGTH - 1; i > 0; --i)
                            dwnode.node_smp.node_sample_buffer[i] = dwnode.node_smp.node_sample_buffer[i - 1];
                        // sample value.
                        dwnode.node_smp.node_sample_buffer[0] = dwnode.node_smp.node_sample_value;

                        ImGui::PlotLines(
                            "",
                            dwnode.node_smp.node_sample_buffer,
                            SMP_BUFFER_LENGTH,
                            NULL,
                            (const char*)0,
                            -1.0f,
                            1.0f,
                            ImVec2(dwnode.node_width + 8.0f, 100.0f)
                        );

                        dwnode.node_smp.timer_tmp = chrono::steady_clock::now(); // 计时器复位.
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
                    ImGui::ColorConvertFloat4ToU32(link.second.line_color)
                );
                // hovered.
                ImNodes::PushColorStyle(
                    ImNodesCol_LinkHovered,
                    ImGui::ColorConvertFloat4ToU32(
                        ColorAhplaAdd(link.second.line_color, -0.42f)
                    )
                );
                // selected.
                ImNodes::PushColorStyle(
                    ImNodesCol_LinkSelected,
                    ImGui::ColorConvertFloat4ToU32(
                        ColorAhplaAdd(link.second.line_color, -0.42f)
                    )
                );

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

    void NodeComponent::nodecmp_cleardcsdata() {
        // vector clear data.
        for (size_t i = 0; i < NODE_COMPONENTS_DCS.size(); ++i) {

            NODE_COMPONENTS_DCS[i].in_connect.clear();

            for (size_t j = 0; j < NODE_COMPONENTS_DCS[i].out_connect.size(); ++j)
                NODE_COMPONENTS_DCS[i].out_connect[j].link_number.clear();

            NODE_COMPONENTS_DCS[i].out_connect.clear();
        }
        NODE_COMPONENTS_DCS.clear();
        NODE_LINESLINK.clear();
    }

    void NodeComponent::nodecmp_freetexhd(std::vector<uint32_t> handle) {
        // opengl delete.
        for (size_t i = 0; i < TEXTURE_HANDLE.size(); ++i) {
            if (TEXTURE_HANDLE[i] > 0)
                glDeleteTextures(1, &TEXTURE_HANDLE[i]);
        }
    }

    namespace system {
        int32_t connection_management::uniquelink_count = 1;
        int32_t connection_management::uniquenode_count = 1;
        int32_t connection_management::uniquepoint_count = 1;

        // 删除节点.
        void connection_management::component_node_delete(
            vector<pne_datadefine::node_attribute>& nodes,
            unordered_map<int32_t, pne_datadefine::node_link_line>& connect_lines,
            uint32_t                                                nodes_id
        ) {
            // 搜索连线 => 删除节点所有连线 => 删除节点
            // min O(n) search node_id.
            for (size_t i = 0; i < nodes.size(); ++i) {
                if (nodes[i].node_number == nodes_id) {
                    // 删除相关连.
                    for (size_t j = 0; j < nodes[i].in_connect.size(); ++j) {

                        // input.
                        component_link_break(
                            nodes,
                            connect_lines,
                            nodes[i].in_connect[j].link_number
                        );
                    }
                    for (size_t j = 0; j < nodes[i].out_connect.size(); ++j) {

                        // output.
                        for (size_t d = 0; d < nodes[i].out_connect[j].link_number.size(); ++d) {
                            component_link_break(
                                nodes,
                                connect_lines,
                                nodes[i].out_connect[j].link_number[d]
                            );
                        }
                    }
                    // delete node.
                    nodes.erase(nodes.begin() + i);
                }
            }
        }

        // 删除连线.
        void connection_management::component_link_break(
            vector<pne_datadefine::node_attribute>&                 nodes,
            unordered_map<int32_t, pne_datadefine::node_link_line>& connect_lines,
            int32_t                                                 connect_id
        ) {
            // 断开连接线, Erase hash_map value.
            connect_lines.erase(connect_id);
        }

#define SRAND_NODE_POSMIN -10000
#define SRAND_NODE_POSMAX  10000
        void connection_management::component_node_init(
            pne_datadefine::node_attribute& node
        ) {
            // srand node create_pos.
            float srandpos[2] = {
                float(rand() % (SRAND_NODE_POSMAX - SRAND_NODE_POSMIN + 1) + SRAND_NODE_POSMIN) / 100.0f,
                float(rand() % (SRAND_NODE_POSMAX - SRAND_NODE_POSMIN + 1) + SRAND_NODE_POSMIN) / 100.0f
            };

            node.node_number = uniquenode_count;
            // set create pos. [srand]
            ImNodes::SetNodeGridSpacePos(
                uniquenode_count,
                ImVec2(320.0f + srandpos[0], 320.0f + srandpos[1])
            );

            uniquenode_count += CREATE_UNIQUE_STEP;

            for (size_t i = 0; i < node.in_connect.size(); ++i) {
                // input connectpoint count.
                node.in_connect[i].point_number = uniquepoint_count;

                uniquepoint_count += CREATE_UNIQUE_STEP;
            }

            for (size_t i = 0; i < node.out_connect.size(); ++i) {
                // output connectpoint count.
                node.out_connect[i].point_number = uniquepoint_count;

                uniquepoint_count += CREATE_UNIQUE_STEP;
            }
        }

        // 创建连线.
        void connection_management::CompTick_Links(
            vector<pne_datadefine::node_attribute>& nodes,
            unordered_map<int32_t, pne_datadefine::node_link_line>& connect_lines,
            bool   lk_switch,
            ImVec2 lk_beginend
        ) {
            pne_datadefine::node_link_line create_link = {};
            PARAM_TYPE                     temp_type[2] = {};
            // 0:in.i, 1:in.j, 2:out.i, 3:out.j
            size_t temp_count[4] = {};

            // IsLink => LinkLines.
            if (ImNodes::IsLinkCreated(&create_link.begin_point, &create_link.end_point) ||
                lk_switch
                ) {
                if (lk_switch) {
                    create_link.begin_point = (int32_t)lk_beginend.x;
                    create_link.end_point = (int32_t)lk_beginend.y;
                }

                for (size_t i = 0; i < nodes.size(); ++i) {
                    for (size_t j = 0; j < nodes[i].in_connect.size(); ++j) {

                        // input 连接点所在 node => 互连标识(line_id).
                        if ((nodes[i].in_connect[j].point_number == create_link.begin_point) ||
                            (nodes[i].in_connect[j].point_number == create_link.end_point))
                        {
                            temp_count[0] = i;
                            temp_count[1] = j;
                            temp_type[0] = nodes[i].in_connect[j].point_param_type;
                        }
                    }
                    for (size_t j = 0; j < nodes[i].out_connect.size(); ++j) {

                        // output 连接点所在 node => 互连标识(line_id).
                        if ((nodes[i].out_connect[j].point_number == create_link.begin_point) ||
                            (nodes[i].out_connect[j].point_number == create_link.end_point))
                        {
                            temp_count[2] = i;
                            temp_count[3] = j;
                            temp_type[1] = nodes[i].out_connect[j].point_param_type;
                        }
                    }
                }

                // Equal_types => link.
                if (temp_type[0] == temp_type[1]) {

                    // 节点为 执行路径通过时 Out 也只能允许输出一个连接.
                    if ((nodes[temp_count[2]].out_connect[NULL].link_number.size() > NULL) &&
                        (temp_count[3] == NULL) &&
                        (nodes[temp_count[2]].execute_path_switch == true))
                    {
                        for (const auto& del : nodes[temp_count[2]].out_connect[NULL].link_number)
                            component_link_break(nodes, connect_lines, del);
                    }

                    // delete old_link.
                    component_link_break(nodes, connect_lines, nodes[temp_count[0]].in_connect[temp_count[1]].link_number);

                    nodes[temp_count[0]].in_connect[temp_count[1]].link_number = uniquelink_count;
                    nodes[temp_count[2]].out_connect[temp_count[3]].link_number.push_back(uniquelink_count);

                    create_link.line_color = nodes[temp_count[0]].in_connect[temp_count[1]].point_color;

                    // IsLink => create push.
                    connect_lines.insert(make_pair(uniquelink_count, create_link));
                    uniquelink_count += CREATE_UNIQUE_STEP;
                }
            }
        }
    }
}