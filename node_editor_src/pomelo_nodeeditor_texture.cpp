// PomeloNodeEditor_Texture.
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

#include <GL/glew.h>
#include <GL/GL.h>

#include "pomelo_nodeeditor.h"

using namespace std;

// USE NVIDIA GPU.
extern "C" { 
	_declspec(dllexport) 
		unsigned long NvOptimusEnablement = 0x00000001; 
}

// opengl,stb_image load texture. [.jpg.png.bmp]
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace NODE_TEXTURE {

	system::texture_info NodeTexture::load_image_texture(StrText img) {

		system::texture_info returnTexture   = {};
		int32_t              getimageinfo[3] = {};

		if (filesystem::exists(img)) {
			returnTexture.texture_state = LOAD_TXETURE_SUCC;

			uint8_t* get_image_srcdata = stbi_load(
				img,
				&getimageinfo[0],
				&getimageinfo[1],
				&getimageinfo[2],
				NULL
			);

			// create => hdlist => bind.
			glGenTextures(1, &returnTexture.texture_handle);
			TEXTURE_HANDLE.push_back(returnTexture.texture_handle);
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
				// RGBA texture.
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
				// RGB texture.
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