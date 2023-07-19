// pomelo_node_editor.
#include <fstream>
#include <chrono>
#include <document.h> // RapidJSON

#include "pomelo_node_editor.h"

using namespace std;

#define PNE_GETTIME            chrono::system_clock::now()
#define PNE_GETTIEM_TIME_SIZE  chrono::duration_cast<chrono::milliseconds>(PNE_GETTIME.time_since_epoch()).count()
#define PNE_TIMESEED_SRAND     srand((uint32_t)PNE_GETTIEM_TIME_SIZE)

namespace PNEinit {

	void initialization_imgui(imguiinit initparam, GLFWwindow* wobj) {
        IMGUI_CHECKVERSION();

        // Setup ImGui Context.
        ImGui::CreateContext();
        ImGuiIO& GuiIO = ImGui::GetIO(); (void)GuiIO;

        // 键盘, 手柄 交互.
        GuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard controls.
        GuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // enable gamepad controls.

        // setup dear imgui style.
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        {
            ImGuiStyle* config_style = &ImGui::GetStyle();
            
            pne_system_style::style_preset_type(
                initparam.style_typenumber, // style type.
                config_style->Colors        // control draw colors.
            );
        }

        // init set fonts.
        auto SetFonts = ImGui::GetIO().Fonts;
        SetFonts->AddFontFromFileTTF
        (
            initparam.gui_fonts_path,
            initparam.gui_fonts_size,
            NULL,
            SetFonts->GetGlyphRangesChineseFull()
        );

        // setup platform & renderer backends.
        ImGui_ImplGlfw_InitForOpenGL(wobj, true);
        ImGui_ImplOpenGL3_Init(initparam.gui_shader_version);
	}

    void initialization_imnodes() {
        PNE_TIMESEED_SRAND; // time => srabnd seed.

        // setup ImNodes context.
        ImNodes::CreateContext();
        ImNodes::StyleColorsDark();
    }
}

namespace PNErender {
    
    void rendertick_gui(
        bool&            render_flag, 
        function<void()> render_gui
    ) {
        // start the Dear ImGui frame.
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
       
        // draw flag. 
        // render node state False: Not Draw&Compute.
        if (render_flag) 
        {
            pne_node_component::component_pcs_tick();
        }
        render_gui(); // draw imgui function.

        // render imgui.
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}

namespace PNEclose {

    void close_imgui() {
        // End Close ImGui.
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void close_imnodes() {
        // Free Context.
        ImNodes::DestroyContext();
    }
}

extern vector<pne_node_component::node_attribute> __NODE_COMPONENTS;
extern vector<pne_node_component::node_attribute> __NODE_COMPONENTS_DCS;
extern unordered_map<int32_t, pne_node_component::node_link_line> __NODE_LINESLINK;

namespace PNEpreset {

    bool             window_no_close = false;
    ImGuiWindowFlags window_flags = NULL;

    // 预设组件面板.
    void component_guipanel() {
        static bool WindowNoMove = true;

        if (WindowNoMove) {
            window_flags  = ImGuiWindowFlags_NoMove;   // 固定位置.
            window_flags |= ImGuiWindowFlags_NoResize; // 固定大小.
            window_flags |= ImGuiWindowFlags_NoCollapse;
        }
        else
            window_flags = NULL;

        ImGui::Begin("Node Components List", &window_no_close, window_flags);
        {
            ImGui::Checkbox("Fixed Panel", &WindowNoMove);
            ImGui::Checkbox("Fixed Editor", &pne_node_component::editor_fixed_move);

            ImGui::Text("Render ImGui: %.2f fps", ImGui::GetIO().Framerate);
            ImGui::Text("");

            // Style FrameRounding.
            ImGui::GetStyle().FrameRounding = 7.2f;

            // Components List.
            for (const auto& __NODE_COMPONENT : __NODE_COMPONENTS) {
                // create components.
                if (ImGui::Button(__NODE_COMPONENT.node_title.c_str(), ImVec2(220.0f, 32.0f))) {
                    // create copy node_attribute.
                    pne_node_component::node_attribute create_node = __NODE_COMPONENT;

                    // 分配 输入输出 连接唯一属性.
                    pne_node_component::component_allocation(create_node);

                    // load => __NODE_COMPONENTS => __NODE_COMPONENTS_DCS
                    __NODE_COMPONENTS_DCS.push_back(create_node);
                }
            }

            ImGui::Text(u8"连接数量: %u lines", __NODE_LINESLINK.size());
            ImGui::Text(u8"节点数量: %u nodes", __NODE_COMPONENTS_DCS.size());
        }
        ImGui::End();
    }

    bool component_load(StrText nodecmp_jsonpath, LdFuncPtr texfunction) {
        bool rteurn_flag = true;

        // load config json => push component.
        __NODE_COMPONENTS.push_back(
            pne_node_configfile::load_component_json
            (
                nodecmp_jsonpath, 
                rteurn_flag,
                texfunction
            )
        );
        return rteurn_flag;
    }
}

namespace PNEtexture {

    NodeTextureIO::NodeTextureIO(uint32_t node_id) {
        // node id => vector pos
        for (size_t i = 0; i < __NODE_COMPONENTS_DCS.size(); ++i)
            if (__NODE_COMPONENTS_DCS[i].node_number == node_id)
                vector_node_pos = i;
    }

    uint32_t NodeTextureIO::get_texture_handle() {
        if (vector_node_pos == -1)
            return NULL;
        else
            return __NODE_COMPONENTS_DCS[vector_node_pos].node_tex.node_handle;
    }

    void NodeTextureIO::set_texture_handle(uint32_t hd) {
        if (hd > 0)
            __NODE_COMPONENTS_DCS[vector_node_pos].node_tex.node_handle = hd;
    }
}

using namespace pne_node_component;
namespace pne_node_configfile {

    node_attribute load_component_json(
        StrText   jsonpath, 
        bool&     state,
        LdFuncPtr texfunc
    ) {
        node_attribute return_node_cmp = {};

        ifstream read_jsonfile(jsonpath);
        if (!read_jsonfile) {

            // Failed read json.
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

                    const auto& nodeColor = jsondoc["node color"];
                    
                    return_node_cmp.node_color.x = nodeColor[0].GetFloat();
                    return_node_cmp.node_color.y = nodeColor[1].GetFloat();
                    return_node_cmp.node_color.z = nodeColor[2].GetFloat();
                    return_node_cmp.node_color.w = nodeColor[3].GetFloat();
                }

                // 解析 node_color_click "node color click"
                if (jsondoc.HasMember("node color click") && jsondoc["node color click"].IsArray()) {

                    const auto& nodeColorClick = jsondoc["node color click"];

                    return_node_cmp.node_color_click.x = nodeColorClick[0].GetFloat();
                    return_node_cmp.node_color_click.y = nodeColorClick[1].GetFloat();
                    return_node_cmp.node_color_click.z = nodeColorClick[2].GetFloat();
                    return_node_cmp.node_color_click.w = nodeColorClick[3].GetFloat();
                }

                // 解析 node_title "node title"
                if (jsondoc.HasMember("node title") && jsondoc["node title"].IsString()) {

                    const auto& nodeTitle = jsondoc["node title"];

                    return_node_cmp.node_title = nodeTitle.GetString();
                }

                // 解析 node_tex "node image"
                if (jsondoc.HasMember("node image") && jsondoc["node image"].IsString()) {

                    const auto& nodeTitle = jsondoc["node image"];

                    // 加载纹理句柄.
                    if (texfunc != nullptr)
                        return_node_cmp.node_tex.node_handle = texfunc(nodeTitle.GetString()).texture_handle;
                }

                // 解析 node_tex "node image dwsz" 纹理绘制尺寸.
                if (jsondoc.HasMember("node image dwsz") && jsondoc["node image dwsz"].IsArray()) {

                    const auto& nodeTexDwSize = jsondoc["node image dwsz"];

                    return_node_cmp.node_tex.node_texdraw_size.x = nodeTexDwSize[0].GetFloat();
                    return_node_cmp.node_tex.node_texdraw_size.y = nodeTexDwSize[1].GetFloat();
                }

                // 解析 in_point "input point"
                if (jsondoc.HasMember("input point") && jsondoc["input point"].IsArray()) {

                    const auto& inPoint = jsondoc["input point"].GetArray();

                    for (const auto& point : inPoint) {
                        node_in_connectpoint inConnectPoint;

                        inConnectPoint.point_number = NULL;

                        if (point.HasMember("point color") && point["point color"].IsArray()) {

                            const auto& pointColor = point["point color"].GetArray();

                            inConnectPoint.point_color.x = pointColor[0].GetFloat();
                            inConnectPoint.point_color.y = pointColor[1].GetFloat();
                            inConnectPoint.point_color.z = pointColor[2].GetFloat();
                            inConnectPoint.point_color.w = pointColor[3].GetFloat();
                        }

                        if (point.HasMember("point text") && point["point text"].IsString()) {

                            const auto& pointText = point["point text"];

                            inConnectPoint.point_text = pointText.GetString();
                        }

                        if (point.HasMember("point param") && point["point param"].IsInt()) {

                            const auto& pointType = point["point param"].GetInt();
                            inConnectPoint.point_param_type = (PARAMTYPE)pointType;
                        }
                        return_node_cmp.in_connect.push_back(inConnectPoint);
                    }
                }

                // 解析 out_point "output point"
                if (jsondoc.HasMember("output point") && jsondoc["output point"].IsArray()) {

                    const auto& outPoint = jsondoc["output point"].GetArray();

                    for (const auto& point : outPoint) {
                        node_out_connectpoint outConnectPoint;

                        outConnectPoint.point_number = NULL;

                        if (point.HasMember("point color") && point["point color"].IsArray()) {

                            const auto& pointColor = point["point color"].GetArray();

                            outConnectPoint.point_color.x = pointColor[0].GetFloat();
                            outConnectPoint.point_color.y = pointColor[1].GetFloat();
                            outConnectPoint.point_color.z = pointColor[2].GetFloat();
                            outConnectPoint.point_color.w = pointColor[3].GetFloat();
                        }

                        if (point.HasMember("point text") && point["point text"].IsString()) {

                            const auto& pointText = point["point text"];

                            outConnectPoint.point_text = pointText.GetString();
                        }

                        if (point.HasMember("point param") && point["point param"].IsInt()) {

                            const auto& pointType = point["point param"].GetInt();
                            outConnectPoint.point_param_type = (PARAMTYPE)pointType;
                        }
                        return_node_cmp.out_connect.push_back(outConnectPoint);
                    }
                }

                // 解析 node_numtype "node type"
                if (jsondoc.HasMember("node type") && jsondoc["node type"].IsUint()) {

                    return_node_cmp.node_numtype = jsondoc["node type"].GetUint();
                }
            }
            else
                state = false;
            state = true;
        }

        return return_node_cmp;
    }
}