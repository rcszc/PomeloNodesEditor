// PomeloNodeEditor_DataDefine. RCSZ.

#ifndef _POMELONODEEDITOR_DATADEFINE
#define _POMELONODEEDITOR_DATADEFINE

#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>
#include <any>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(_MSV_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib,"legacy_stdio_definitons.lib")
#endif

typedef unsigned short StyleType;
typedef const char*    StrText;

typedef unsigned long long NODE_TYPE;
typedef unsigned short     PARAM_TYPE;

// 节点源数据.
namespace pne_datadefine {
	namespace pne_init {

		// glfw imgui initparam. 
		struct imguiinit {

			// #version 330, #version 460.
			StrText gui_shader_version;
			StrText gui_fonts_path;
			float   gui_fonts_size;

			StyleType style_typenumber;

			constexpr imguiinit() :
				gui_shader_version ("#version 330"),
				gui_fonts_path     ("null"),
				gui_fonts_size     (18.5f),
				style_typenumber   (NULL)
			{}
			constexpr imguiinit(StrText ver, StrText pth, float sze, StyleType tpe) :
				gui_shader_version (ver),
				gui_fonts_path     (pth),
				gui_fonts_size     (sze),
				style_typenumber   (tpe)
			{}
		};
	}

	// node: link vector.
	struct node_link_line {

		int32_t begin_point; // linkline begin point 
		int32_t end_point;   // linkline end point

		ImVec4 line_color;   // line color RGBA.
	};

	// node: input connect point.
	struct node_in_connectpoint {

		int32_t     point_number;  // connect point unique
		ImVec4      point_color;   // connect point rgba
		std::string point_text;    // connect point str text

		PARAM_TYPE  point_param_type;
		std::any    point_param;   // connect point param

		int32_t link_number;       // line id, unique (输入只能有单连接)
	};

	// node: output connect point.
	struct node_out_connectpoint {

		int32_t     point_number; // connect point unique
		ImVec4      point_color;  // connect point rgba
		std::string point_text;   // connect point str text

		PARAM_TYPE  point_param_type;
		std::any    point_param;  // connect point param

		std::vector<int32_t> link_number; // line id, multiple (输出可以有多连接)
	};

	// 节点贴图.
	struct node_texture {

		uint32_t node_handle;       // node texture handle
		ImVec2   node_texdraw_size; // node texture render size xy
	};

	// 节点数据采样器.
#define SMP_BUFFER_LENGTH 256
	struct node_sample {

		bool   node_sample_sw;     // sample switch
		float  node_sample_buffer[SMP_BUFFER_LENGTH];

		float  node_sample_value;  // limit -1.0f ~ 1.0f
		float  node_sample_timer;  // sample time[ms]
		float* smp_pointer;

		std::chrono::steady_clock::time_point timer_tmp;
	};

	struct node_attribute {

		int32_t     node_number;      // node unique
		ImVec4      node_color;       // node rgba
		ImVec4      node_color_click; // node click rgba
		std::string node_title;       // node str title	
		ImVec2      node_position;    // node pos
		float       node_width;       // node render width

		bool execute_path_switch;

		node_texture node_tex; // texture
		node_sample  node_smp; // sample

		std::vector<node_in_connectpoint>  in_connect;
		std::vector<node_out_connectpoint> out_connect;

		NODE_TYPE node_numtype; // node types
	};
}

// 节点编辑器 io 数据结构.
namespace IODATA {
	struct NodesExport {

		std::vector<pne_datadefine::node_attribute> nodes;
		std::unordered_map<
			int32_t,
			pne_datadefine::node_link_line
		> lines;

		size_t data_size;
	};

	// node save compress data.
	struct SaveNodeData {

		NODE_TYPE node_types;

		int32_t node_unique;
		ImVec2  node_position;

		std::vector<int32_t> in_connect_unique;
		std::vector<int32_t> out_connect_unique;
	};

	struct SaveLineData {

		int32_t begin_num;
		int32_t end_num;
	};
}

#endif
