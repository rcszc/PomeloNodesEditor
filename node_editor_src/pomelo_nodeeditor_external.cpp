// PomeloNodeEditor_External.
#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>

#include "pomelo_nodeeditor_external.h"

using namespace std;

#define FILE_LABLE_NODE "NodeGourpEnd"
#define FTLE_LABLE_TYPE "type"
#define FILE_LABLE_ENUM "unique"
#define FILE_LABLE_EPOS "pos"

#define FILE_LABLE_INCON  "in_connect"
#define FILE_LABLE_OUTCON "out_connect"

#define FILE_LABLE_LINES "line"

namespace Ext_EditorFile {
	namespace system {
		/*
		* @param  const char*
		* @return LPCWSTR
		*/
		LPWSTR ConstCharToLPCWSTR(const char* szString) {
			int dwLen = (int)strlen(szString) + 1; // ADD "\0"
			int nwLen = MultiByteToWideChar(CP_ACP, 0, szString, dwLen, NULL, 0);

			LPWSTR lpszPath = new WCHAR[dwLen];
			MultiByteToWideChar(CP_ACP, 0, szString, dwLen, lpszPath, nwLen);

			return lpszPath;
		}
#define CCTLW ConstCharToLPCWSTR

		/*
		* @param  LPCWSTR
		* @return string
		*/
		string LPCWSTRtoString(LPCWSTR wideString) {
			int buffer_size = 
				WideCharToMultiByte(CP_UTF8, 0, wideString, -1, NULL, 0, NULL, NULL);

			vector<char> buffer(buffer_size);
			WideCharToMultiByte(CP_UTF8, 0, wideString, -1, buffer.data(), buffer_size, NULL, NULL);

			return string(buffer.data());
		}
#define LWTSTR LPCWSTRtoString

		string OpenFileTypeWindow() {
			OPENFILENAME open_file;
			wchar_t SZFILE[MAX_PATH] = {};

			ZeroMemory(&open_file, sizeof(open_file));
			open_file.lStructSize = sizeof(open_file);
			open_file.hwndOwner   = NULL;
			open_file.lpstrFilter = L"NodesDat Files\0*.pnedat\0";
			open_file.lpstrFile   = SZFILE;
			open_file.nMaxFile    = MAX_PATH;
			open_file.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

			if (GetOpenFileName(&open_file))
				return LWTSTR(open_file.lpstrFile);
			else
				return "";
		}
#define OFNWIN OpenFileTypeWindow
	}
	
	bool FileProcessing::Save_NodesDataFile(
		StrText            File, 
		IODATA::NodesExport Source
	) {
		bool     return_state = true;
		ofstream save_file(File);

		if (save_file) {
			// 节点数据.
			for (size_t i = 0; i < Source.nodes.size(); ++i) {

				save_file << FTLE_LABLE_TYPE << " " << Source.nodes[i].node_numtype << endl;
				save_file << FILE_LABLE_ENUM << " " << Source.nodes[i].node_number << endl;
				save_file << FILE_LABLE_EPOS << " " << Source.nodes[i].node_position.x << " " << Source.nodes[i].node_position.y << endl;

				for(size_t j = 0; j < Source.nodes[i].in_connect.size(); ++j)
					save_file << FILE_LABLE_INCON << " " << Source.nodes[i].in_connect[j].point_number << endl;

				for (size_t j = 0; j < Source.nodes[i].out_connect.size(); ++j)
					save_file << FILE_LABLE_OUTCON << " " << Source.nodes[i].out_connect[j].point_number << endl;

				save_file << FILE_LABLE_NODE << endl;
			}
			// 互连数据.
			for (const auto& linedat : Source.lines) {

				save_file << FILE_LABLE_LINES << " " << linedat.second.begin_point << " " << linedat.second.end_point << endl;
			}
		}
		else
			return_state = false;
		return return_state;
	}

	bool FileProcessing::Read_NodesDataFile(
		StrText                       File,
		vector<IODATA::SaveNodeData>& nodes,
		vector<IODATA::SaveLineData>& lines
	) {
		ifstream read_file(File);
		string   read_line = {}, read_head = {};
		bool     return_state = true;

		IODATA::SaveNodeData nodes_tempdata = {};
		IODATA::SaveLineData lines_tempdata = {};
		
		if (read_file) {
			while (getline(read_file, read_line)) {
				istringstream DataStr(read_line);

				DataStr >> read_head;

				// read nodes data.
				{
					if (read_head == FTLE_LABLE_TYPE)
						DataStr >> nodes_tempdata.node_types;

					if (read_head == FILE_LABLE_ENUM)
						DataStr >> nodes_tempdata.node_unique;

					if (read_head == FILE_LABLE_EPOS)
						DataStr >> nodes_tempdata.node_position.x >> nodes_tempdata.node_position.y;

					if (read_head == FILE_LABLE_INCON) {
						int32_t connect_number = NULL;
						DataStr >> connect_number;
						nodes_tempdata.in_connect_unique.push_back(connect_number);
					}
					if (read_head == FILE_LABLE_OUTCON) {
						int32_t connect_number = NULL;
						DataStr >> connect_number;
						nodes_tempdata.out_connect_unique.push_back(connect_number);
					}

					if (read_head == FILE_LABLE_NODE) {
						nodes.push_back(nodes_tempdata);

						nodes_tempdata.in_connect_unique.clear();
						nodes_tempdata.out_connect_unique.clear();
					}
				}
				// read lines data.
				{
					if (read_head == FILE_LABLE_LINES) {
						DataStr >> lines_tempdata.begin_num >> lines_tempdata.end_num;
						lines.push_back(lines_tempdata);
					}
				}
			}
		}
		else
			return_state = false;
		return return_state;
	}

#define FILE_NAME_BUFFER_LEN 256
	void NodesEditorFileMenu(PNE::EdiorGUI& editobj) {

		static ImGuiWindowFlags window_flags = NULL;
		static bool             window_no_close = false;
		static bool             WindowNoMove = true;

		if (WindowNoMove) {
			window_flags = ImGuiWindowFlags_NoMove;    // 固定位置.
			window_flags |= ImGuiWindowFlags_NoResize; // 固定大小.
			window_flags |= ImGuiWindowFlags_NoCollapse;
		}
		else
			window_flags = NULL;

		ImGui::Begin("Nodes Editor File", &window_no_close, window_flags);
		{
			static char  filename_buffer[FILE_NAME_BUFFER_LEN] = "";
			static float error_info_ahpla = 0.0f;

			if (error_info_ahpla < 0.0f) error_info_ahpla = 0.0f;

			ImGui::Checkbox("Fixed Panel", &WindowNoMove);

			ImGui::SetNextItemWidth(160);
			ImGui::InputText(u8"File", filename_buffer, FILE_NAME_BUFFER_LEN * 8);

			ImGui::SameLine();
			if (ImGui::Button(u8"打开文件夹")) {

				// copy filepath.
				strncpy_s(
					filename_buffer,
					system::OFNWIN().c_str(),
					FILE_NAME_BUFFER_LEN - 1
				);
			}

			if (ImGui::Button(u8"保存节点数据")) {

				FileProcessing SaveFile;
				SaveFile.Save_NodesDataFile(filename_buffer, editobj.GetNodesLinesData());
			}

			if (ImGui::Button(u8"加载节点数据")) {

				vector<IODATA::SaveNodeData> nodes = {};
				vector<IODATA::SaveLineData> lines = {};

				FileProcessing ReadFile;
				if (ReadFile.Read_NodesDataFile(filename_buffer, nodes, lines)) {

					// 加载数据到节点编辑器.
					editobj.ReadNodeLinesData(nodes, lines);
				}
				else
					error_info_ahpla = 1.0f;
			}

			ImGui::SameLine();
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, error_info_ahpla), "Failed load file.");
			error_info_ahpla -= 0.01f;
		}
		ImGui::End();
	}
}