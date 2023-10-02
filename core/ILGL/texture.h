#pragma once
#include<string>
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

namespace ilgl
{
	struct Texture2DImportSettings
	{
		bool wrapU;
		bool wrapV;
		bool magLinear;
		bool minLinear;
		bool minLinearMipMap;

		GLenum wrap_S;
		GLenum wrap_T;
		GLenum magFilter;
		GLenum minFilter;

		Texture2DImportSettings(bool wrapU, bool wrapV, bool isMagLinear, bool isMinLinear, bool isMinMipmapLinear);
		GLenum getWrap_S();
		GLenum getWrap_T();
		GLenum getmagFilter();
		GLenum getminFilter();
	};

	unsigned int loadTexture(const std::string filePath, Texture2DImportSettings settings);
}