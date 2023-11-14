#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"


namespace qm
{
	unsigned int loadTexture(const char* filePath, int wrapMode, int filterMode)
	{
		stbi_set_flip_vertically_on_load(true);

		int width, height, numComponents;

		unsigned int texture;
		unsigned int colorNum;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		unsigned char* data = stbi_load(filePath, &width, &height, &numComponents, 0);

		if (data == NULL)
		{
			printf("Failed to load image %s", filePath);
			stbi_image_free(data);
			return 0;
		}
	
		switch (numComponents)
		{
		case 1:
			colorNum = GL_RED;
			break;

		case 2:
			colorNum = GL_RG;
			break;

		case 3:
			colorNum = GL_RGB;
			break;

		case 4:
			colorNum = GL_RGBA;
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, colorNum, width, height, 0, colorNum, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(data);
		return texture;
	}


}

