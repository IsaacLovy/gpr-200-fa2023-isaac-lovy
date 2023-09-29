#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"

namespace ilgl
{
	unsigned int loadTexture(const std::string filePath)
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
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		return texture;
	}

	Texture2DImportSettings::Texture2DImportSettings(bool wrapU, bool wrapV, bool isMagLinear, bool isMinLinear, bool isMinMipmapLinear)
	{
		this.wrapU = wrapU;
		this.wrapV = wrapV;
		magLinear = isMagLinear;
		minLinear = isMinLinear;
		minLinearMipMap = isMinMipmapLinear;
	}
}