// PomeloNodeEditor_External.
// Pomelo ImGui 扩展窗口模块.

#ifndef _POMELONODEEDITOR_EXTERNAL_H
#define _POMELONODEEDITOR_EXTERNAL_H

#include "PomeloNodeEditor.h"

namespace Ext_EditorFile {

	class FileProcessing {
	public:
		/*
		* @param  const char*        (File Path)
		* @param  IODATA::NodesLines (save src data)
		* @return bool
		*/
		bool Save_NodesDataFile(
			StrText            File,
			IODATA::NodesLines Source
		);
		/*
		* @param  const char* (File Path)
		* @param  std::vector<IODATA::SaveNodeData>& (保存格式节点数据)
		* @param  std::vector<IODATA::SaveLineData>& (保存格式连线数据)
		* @return bool 
		*/
		bool Read_NodesDataFile(
			StrText                            File, 
			std::vector<IODATA::SaveNodeData>& nodes,
			std::vector<IODATA::SaveLineData>& lines
		);
	};

	void NodesEditor_FileMenu(PNE::EdiorGUI& editobj);
}

#endif