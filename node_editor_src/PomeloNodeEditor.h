// PomeloNodeEditor.

#ifndef _POMELONODEEDITOR_H
#define _POMELONODEEDITOR_H

#include <functional>
#include <GLFW/glfw3.h>

#include "imnodes.h"
#include "PomeloNodeEditor_DataDefine.h"

#define CREATE_UNIQUE_STEP 1

#define STYLE_TYPE_CYAN   0x00
#define STYLE_TYPE_ORANGE 0x01
#define STYLE_TYPE_PINK   0x02

namespace NODE_TEXTURE {
	namespace system {

#define LOAD_TXETURE_SUCC    1 // 加载成功.
#define LOAD_TEXTURE_ERRCTE -1 // 纹理创建失败.
#define LOAD_TEXTURE_ERRDAT -2 // 图片数据加载失败.
#define LOAD_TXETURE_ERRFIL -3 // 文件不存在.

		// image texture info.
		struct texture_info {

			uint32_t texture_handle; // opengl texture handle.
			int8_t   texture_state;  // load state.

			uint32_t image_width, image_height; // image size.
			uint8_t  image_channel;             // image color channel_num.
		};
	}

	class NodeTexture {
	protected:
		std::vector<uint32_t> TEXTURE_HANDLE = {};
		/*
		* @param  const char* (image path)
		* @return system::texture_info
		*/
		system::texture_info load_image_texture(StrText img);
	};
}

namespace NODE_CMP {
	namespace system {

		class connection_management {
		protected:
			static int32_t uniquelink_count;
			static int32_t uniquenode_count;
			static int32_t uniquepoint_count;
			/*
			* @param  (nodes data, links data, key_id)
			* @return void
			*/
			void component_node_delete(
				std::vector<pne_datadefine::node_attribute>&                 nodes,
				std::unordered_map<int32_t, pne_datadefine::node_link_line>& connect_lines,
				uint32_t                                                     nodes_id
			);
			/*
			* @param  (links data, key_id)
			* @return void
			*/
			void component_link_break(
				std::unordered_map<int32_t, pne_datadefine::node_link_line>& connect_lines,
				int32_t                                                      connect_id
			);
			// 初始化待创建节点.
			void component_node_init(
				pne_datadefine::node_attribute& node
			);
			/*
			* @param  (nodes data, links data)
			* @return void
			*/
			// lk_switch 为指定连接编号标志.
			void CompTick_Links(
				std::vector<pne_datadefine::node_attribute>&                 nodes,
				std::unordered_map<int32_t, pne_datadefine::node_link_line>& connect_lines,
				bool   lk_switch,
				ImVec2 lk_beginend
			);
		};
	}
	
	class NodeComponent :public NODE_TEXTURE::NodeTexture {
	protected:
		// node component attribute type_list.
		std::vector<pne_datadefine::node_attribute> NODE_COMPONENTS     = {};
		// node component attribute dynamic_data.
		std::vector<pne_datadefine::node_attribute> NODE_COMPONENTS_DCS = {};
		// node links lines_data.
		std::unordered_map<
			int32_t, 
			pne_datadefine::node_link_line
		> NODE_LINESLINK = {};

		bool node_window_fixed = true;

		/*
		* @param  const char*, bool&
		* @return pne_datadefine::node_attribute
		*/
		pne_datadefine::node_attribute nodecmp_loadconfig(StrText jsonfile, bool& state);

		void nodecmp_render(
			std::vector<pne_datadefine::node_attribute>&                 dwnodes,
			std::unordered_map<int32_t, pne_datadefine::node_link_line>& lines
		);

		// clear NODE_COMPONENTS_DCS, NODE_LINESLINK
		void nodecmp_cleardcsdata();
		// free opengl texture.
		void nodecmp_freetexhd(std::vector<uint32_t> handle);
	};
}

namespace PNE {
	namespace system {
		class style_config {
		protected:
			void guistyle_cyan   (ImVec4* colors);
			void guistyle_orange (ImVec4* colors);
			void guistyle_pink   (ImVec4* colors);

			// imgui set colors.
			void StyleConfig_panel();
		};
	}

	class EdiorGUI :
		public system::style_config,
		public NODE_CMP::NodeComponent,
		public NODE_CMP::system::connection_management
	{
	protected:
		pne_datadefine::pne_init::imguiinit IMGUIPARAM = {};

		void editor_imgui_init(GLFWwindow* winobj);
		void editor_imnodes_init();

	public:
		// initialization.
		EdiorGUI(
			GLFWwindow* win_object,
			pne_datadefine::pne_init::imguiinit init_param = {}
		);
		~EdiorGUI() {};

		bool RenderEditor = true;

		// event loop tick.
		void RenderTick(std::function<void()> render_gui);
		// end free.
		void EndClose();

		bool PushNodeComponent(StrText json_path);

		// 编辑器节点操作菜单 GUI.
		void SystemNodeMenu();
		// 编辑器组件菜单 GUI.
		void SystemCompMenu();

		IODATA::NodesLines GetNodesLinesData();

		// 加载 文件读取格式 节点&连线 数据
		void ReadNodeLinesData(
			std::vector<IODATA::SaveNodeData>& nodes,
			std::vector<IODATA::SaveLineData>& lines
		);
	};
}

#endif