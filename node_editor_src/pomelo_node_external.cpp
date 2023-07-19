// pomelo_node_editor_ext.
#include <string>
#include <vector>
#include <filesystem>

#include <GL/glew.h>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>

#include "pomelo_node_external.h"

using namespace std;
// USE NVIDIA GPU.
extern "C" { _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001; }

namespace pne_ext_texhandle {

	vector<uint32_t> TextureHandle = {};

	void create_texture_hd(uint32_t& texhd) {
		glGenTextures(1, &texhd);
		if (texhd > 0)
			TextureHandle.push_back(texhd);
	}
}

namespace PNEext_free {

	void free_node_texture() {
		// opengl delete texture handle.
		for (size_t i = 0; i < pne_ext_texhandle::TextureHandle.size(); ++i)
			glDeleteTextures(1, &pne_ext_texhandle::TextureHandle[i]);
	}
}

// opengl,stb_image load texture. [.jpg.png]
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace pne_ext_component {

	texture_info extcmp_loadtexture(StrText image_file) {

		texture_info returnTexture = {};
		int32_t getimageinfo[3] = {};

		if (filesystem::exists(image_file)) {
			returnTexture.texture_state = LOAD_TXETURE_SUCC;

			uint8_t* get_image_srcdata = stbi_load(
				image_file, 
				&getimageinfo[0],
				&getimageinfo[1],
				&getimageinfo[2],
				NULL
			);

			pne_ext_texhandle::create_texture_hd(returnTexture.texture_handle);
			glBindTexture(GL_TEXTURE_2D, returnTexture.texture_handle);

			// error create failed.
			if (returnTexture.texture_handle == NULL)
				returnTexture.texture_state = LOAD_TEXTURE_ERRCTE;

			// error load_data failed.
			if (get_image_srcdata == nullptr)
				returnTexture.texture_state = LOAD_TEXTURE_ERRDAT;

			// [弃用项].
			//gluBuild2DMipmaps(GL_TEXTURE_2D, nrChannels, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
			if (getimageinfo[2] > 3) {
				// RGBA txeture.
				glTexImage2D(
					GL_TEXTURE_2D, NULL, 
					getimageinfo[2],
					getimageinfo[0],
					getimageinfo[1],
					NULL, GL_RGBA, GL_UNSIGNED_BYTE, 
					get_image_srcdata
				);
			}
			else {
				// RGB txeture.
				glTexImage2D(
					GL_TEXTURE_2D, NULL,
					getimageinfo[2],
					getimageinfo[0],
					getimageinfo[1],
					NULL, GL_RGB, GL_UNSIGNED_BYTE,
					get_image_srcdata
				);
			}

			// opengl load texture init.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// 生成 mipmap 级贴图. [err]
			//glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, NULL); // 解绑纹理.
			stbi_image_free(get_image_srcdata); // freeimgdata.
		}
		else
			returnTexture.texture_state = LOAD_TXETURE_ERRFIL;
		return returnTexture;
	}
}