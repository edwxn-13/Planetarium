#pragma once

#include <iostream>
#include "stb_image.h"

GLuint setup_texture(const char* filename)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int w, h, chan;
	unsigned char* pxls = stbi_load(filename, &w, &h, &chan, 0);
	stbi_set_flip_vertically_on_load(true);
	if (pxls)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
		delete[] pxls;
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	return texObject;
}




GLuint setup_mipmaps(const char* filename[], int n)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	 
	int w[16], h[16], chan[16];
	unsigned char* pxls[16];

	stbi_set_flip_vertically_on_load(true);

	for (int i = 0; i < n; i++) {


		pxls[i] = stbi_load(filename[i], &w[i], &h[i], &chan[i], 0);

		if (pxls[i])
		{

			glTexImage2D(GL_TEXTURE_2D, i, GL_RGB, w[i], h[i], 0, GL_RGB, GL_UNSIGNED_BYTE, pxls[i]);
			glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps after each texture upload

		}
		delete[] pxls[i];
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);


	return texObject;
}


GLuint CreateTexture(const char* filename)
{
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);



	int width, height, channels;
	unsigned char* pxls = stbi_load(filename, &width, &height, &channels, 0);


	if (pxls != NULL)
	{
		printf("Loaded %s\n", filename);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxls);
		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
	}
	else
	{
		printf("Failed to load %s\n", filename);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] pxls;

	glDisable(GL_BLEND);

	return texObject;
}
