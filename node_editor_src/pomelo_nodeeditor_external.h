// PomeloNodeEditor_External. RCSZ.
// Pomelo ImGui 扩展窗口模块.

#ifndef _POMELONODEEDITOR_EXTERNAL_H
#define _POMELONODEEDITOR_EXTERNAL_H

#include "pomelo_nodeeditor.h"

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
			IODATA::NodesExport Source
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

	void NodesEditorFileMenu(PNE::EdiorGUI& editobj);
}

#endif