// pomelo_node_editor.

#ifndef _POMELO_NODE_EDITOR_H
#define _POMELO_NODE_EDITOR_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <any> // type.
#include <functional>
#include <GLFW/glfw3.h>

#include "imnodes.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(_MSV_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib,"legacy_stdio_definitons.lib")
#endif

#define CREATE_UNIQUE_STEP 1

typedef unsigned short     StyleType;
typedef const char*        StrText;

typedef unsigned long long NODETYPE;
typedef unsigned short     PARAMTYPE;

namespace PNEinit {
	// glfw imgui initparam. 
	struct imguiinit {

		// #version 330, #version 460.
		StrText gui_shader_version;
		StrText gui_fonts_path;
		float   gui_fonts_size;
		// imgui group type.
		StyleType style_typenumber;

		constexpr imguiinit(StrText ver, StrText pth, float sze, StyleType tpe) :
			gui_shader_version (ver),
			gui_fonts_path     (pth),
			gui_fonts_size     (sze),
			style_typenumber   (tpe) 
		{}
	};

	/*
	* @param  imguiinit, GLFWwindow*
	* @return void
	*/
	void initialization_imgui(imguiinit initparam, GLFWwindow* wobj);
	void initialization_imnodes();
}

namespace PNErender {
	/*
	* @param  bool (GUI绘制标志)
	* @return void
	*/
	void rendertick_gui(bool& render_flag, std::function<void()> render_gui);
}

namespace PNEclose {

	void close_imgui();
	void close_imnodes();
}

namespace PNEpreset {
	// render panel.
	void component_guipanel();
	/*
	* @param  const char* (json config path)
	* @return bool        (flag)
	*/
	bool component_load(StrText nodecmp_jsonpath);
}

namespace pne_system_style {

#define STYLE_TYPE_CYAN   0x00
#define STYLE_TYPE_ORANGE 0x01
#define STYLE_TYPE_PINK   0x02
	
	// style type number.
	void style_preset_type(StyleType& number, ImVec4* colors);
}

namespace pne_node_component {

	// node: link vector.
	struct node_link_line {

		int32_t begin_point;  // linkline begin point 
		int32_t end_point;    // linkline end point

		ImVec4 link_line_color; // line color RGBA.
	};

	// node: input connect point.
	struct node_in_connectpoint {

		uint32_t    point_number;  // connect point unique
 		ImVec4      point_color;   // connect point rgba
		std::string point_text;    // connect point str text

		PARAMTYPE   point_param_type;
		std::any    point_param;   // connect point param

		std::vector<int32_t> linkline_number;   // 互联标识 (LineId).
	};

	// node: output connect point.
	struct node_out_connectpoint {

		uint32_t    point_number;  // connect point unique
		ImVec4      point_color;   // connect point rgba
		std::string point_text;    // connect point str text

		PARAMTYPE   point_param_type;
		std::any    point_param;   // connect point param

		std::vector<int32_t> linkline_number;   // 互联标识 (LineId).
	};

	struct node_attribute {

		uint32_t    node_number;      // node unique
		ImVec4      node_color;       // node rgba
		ImVec4      node_color_click; // node click rgba
		std::string node_title;       // node str title	

		std::vector<node_in_connectpoint>  in_connect;
		std::vector<node_out_connectpoint> out_connect;

		NODETYPE node_numtype; // node types
	};

	/*
	* @param  node_attribute&
	* @return void
	*/
	void component_allocation(node_attribute& node);

	// editor window fixed flag.
	extern bool editor_fixed_move;

	void component_render(
		std::vector<node_attribute>& dwnodes,
		std::unordered_map<int32_t, pne_node_component::node_link_line>& lines
	);

	// component process tick.
	void component_pcs_tick();
}

// source components list. (未分配属性值)
extern std::vector<pne_node_component::node_attribute> __NODE_COMPONENTS;
// dynamics components list. (已分配属性值,动态组件)
extern std::vector<pne_node_component::node_attribute> __NODE_COMPONENTS_DCS;
extern std::unordered_map<int32_t, pne_node_component::node_link_line> __NODE_LINESLINK;

namespace pne_node_configfile {
	/*
	* @param  const char* (json配置)
	* @return _pne_node_component::node_attribute
	*/
	pne_node_component::node_attribute load_component_json(
		StrText jsonpath, 
		bool&   state
	);
}

/*
* 获取节点编辑器数据.
* @param  std::vector<pne_node_component::node_attribute>&                 (节点数据)
* @param  std::unordered_map<int32_t, pne_node_component::node_link_line>& (连接数据)
* @return void
*/
void PNEGET_node_link_data(
	std::vector<pne_node_component::node_attribute>&                 node_data,
	std::unordered_map<int32_t, pne_node_component::node_link_line>& link_data
);

#endif