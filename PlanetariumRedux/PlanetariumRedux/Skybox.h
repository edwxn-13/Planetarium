#pragma once


#define WIDTH 1024
#define HEIGHT 768

float skyboxVerts[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//0        7--------6
	 1.0f, -1.0f,  1.0f,//1       /|       /|
	 1.0f, -1.0f, -1.0f,//2      4--------5 |
	-1.0f, -1.0f, -1.0f,//3      | |      | |
	-1.0f,  1.0f,  1.0f,//4      | 3------|-2
	 1.0f,  1.0f,  1.0f,//5      |/       |/
	 1.0f,  1.0f, -1.0f,//6      0--------1
	-1.0f,  1.0f, -1.0f//7
};

unsigned int skyboxIndicies[] =
{
	//right 
	1,2,6,
	6,5,1,

	//left 
	0,4,7,
	7,3,0,

	//top
	4,5,6,
	6,7,4,

	//bottom
	0,3,2,
	2,1,0,

	//back
	0,1,5,
	5,4,0,

	//front
	3,7,6,
	6,2,3
};

struct Skybox
{
	GLuint cubemapTex;
	unsigned int skyboxVBO, skyboxVAO, skyboxEBO;

	Skybox()
	{
		cubemapTex = loadCubemap();
	}

	GLuint loadCubemap()
	{
		glEnable(GL_TEXTURE_CUBE_MAP);

		std::string cubemapFaces[6] =
		{
			"cubemap/right.png",
			"cubemap/left.png",
			"cubemap/top.png",
			"cubemap/bottom.png",
			"cubemap/front.png",
			"cubemap/back.png"
		};
		GLuint cubemapTex;

		glGenTextures(1, &cubemapTex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



		for (unsigned int i = 0; i < 6; i++)
		{
			int width, height, nrChannels;
			unsigned char* data = stbi_load(cubemapFaces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);

				cout << "succ" << std::endl;
			}
			else
			{
				cout << "fucc" << std::endl;
				stbi_image_free(data);

			}
		}
		glDisable(GL_BLEND);

		return cubemapTex;
	}

	void setupCubemap()
	{
		glGenVertexArrays(1, &skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glGenBuffers(1, &skyboxEBO);
		glBindVertexArray(skyboxVAO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerts), &skyboxVerts, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndicies), &skyboxIndicies, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void renderCubemap(unsigned int skybox_shader, SCamera *Camera)
	{
		glDisable(GL_DEPTH_CLAMP);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		glUseProgram(skybox_shader);

		glUniform1i(glGetUniformLocation(skybox_shader, "skybox"), 0);

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view = glm::mat4(1.f);
		view = glm::mat4(glm::mat3(glm::lookAt(Camera->Position, Camera->Position + Camera->Front, Camera->Up)));
		glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
		glBindVertexArray(skyboxVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glEnable(GL_BLEND);
		glDepthFunc(GL_LESS);
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_CLAMP);
	}
};