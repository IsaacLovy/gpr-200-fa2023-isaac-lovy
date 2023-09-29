#pragma once
#include<string>

namespace ilgl
{
	unsigned int loadTexture(const std::string filePath);

	struct Texture2DImportSettings
	{
		bool wrapU;
		bool wrapV;
		bool magLinear;
		bool minLinear;
		bool minLinearMipMap;

		GLenum wrap_S; { get{ return wrapU ? GL_REPEAT : GL_CLAMP_TO_EDGE } };
		GLenum wrap_T; { get{ return wrapV ? GL_REPEAT : GL_CLAMP_TO_EDGE } };
		GLenum magFilter; { get{ return magLinear ? GL_LINEAR : GL_NEAREST } };
		GLenum magFilter; { get{ return minLinearMipMap ? (minLinear ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST) : (minLinear ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST) } };

		void Texture2DImportSettings(bool wrapU, bool wrapV, bool isMagLinear, bool isMinLinear, bool isMinMipmapLinear);
	};
}