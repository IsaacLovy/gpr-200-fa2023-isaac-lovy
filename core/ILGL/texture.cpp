#include "texture.h"

namespace ilgl
{
	unsigned int loadTexture(const std::string filePath, Texture2DImportSettings importSettings)
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, numComponents;
		unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numComponents, 0);
		if (data == NULL)
		{
			printf("Failed to load image %s", filePath);
			stbi_image_free(data);
			return 0;
		}

		//Want to catch any potential format error before binding the texture
		GLenum format = GL_INVALID_ENUM;
		switch (numComponents)
		{
		default:
			break;
		case 1:
			format = GL_RED;
			break;
		case 2:
			format = GL_RG;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		}
		if (format == GL_INVALID_ENUM)
		{
			printf("Failed to load image format %s", filePath);
			stbi_image_free(data);
			return 0;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, importSettings.getWrap_S(), GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, importSettings.getWrap_T(), GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, importSettings.getminFilter());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, importSettings.getmagFilter());

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		return texture;
	}

	ilgl::Texture2DImportSettings::Texture2DImportSettings(bool wrapU, bool wrapV, bool isMagLinear, bool isMinLinear, bool isMinMipmapLinear)
	{
		this->wrapU = wrapU;
		this->wrapV = wrapV;
		magLinear = isMagLinear;
		minLinear = isMinLinear;
		minLinearMipMap = isMinMipmapLinear;
	}

	GLenum ilgl::Texture2DImportSettings::getWrap_S()
	{
		return wrapU ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	}

	GLenum ilgl::Texture2DImportSettings::getWrap_T()
	{
		return wrapV ? GL_REPEAT : GL_CLAMP_TO_EDGE;
	}

	GLenum ilgl::Texture2DImportSettings::getmagFilter()
	{
		return magLinear ? GL_LINEAR : GL_NEAREST;
	}

	GLenum ilgl::Texture2DImportSettings::getminFilter()
	{
		return minLinearMipMap ? (minLinear ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR_MIPMAP_NEAREST) : (minLinear ? GL_NEAREST_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
	}

}