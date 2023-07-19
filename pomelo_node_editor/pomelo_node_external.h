// pomelo_node_external.

#ifndef _POMELO_NODE_EXTERNAL_H
#define _POMELO_NODE_EXTERNAL_H

#include <fstream>

typedef const char* StrText;

namespace PNEext_free {
	// 释放节点所有纹理.
	void free_node_texture();
}

namespace pne_ext_component {

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
	/*
	* @param  const char*  (image file path)
	* @return texture_info (tex handle, image info)
	*/
	texture_info extcmp_loadtexture(StrText image_file);
}

#endif