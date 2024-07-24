#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <iostream>

#include "camera.h"
#include "error.h"
#include "file.h"
#include "shader.h"
#include "shadow.h"
#include "obj.h"
#include "texture.h"
#include "EngineObject.h"
#include "Skybox.h"
#include "PlayerControlled.h"

//light direction variable here

SCamera Camera;
PlayerControlled player;
glm::vec3 lightDirection = glm::vec3(0.1f, -0.81f, -0.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);
vector<EngineMesh> eObjs;
vector<Planet> planetz;
glm::vec4 input_mat = glm::vec4(0.0f); 

void clampInput() 
{
	if (input_mat.x > 2.0f) { input_mat.x = 2.0f; }
	if (input_mat.x < - 2.0f) { input_mat.x = -2.0f; }
	if (input_mat.y < -2.0f) { input_mat.y = 2.0f; }
	if (input_mat.y < -2.0f) { input_mat.y = -2.0f; }


}
void dampInput() 
{
	input_mat -= 0.2f * input_mat;
}
float vertices[] =
{
	//back face
	//pos					//col				//normal
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	-0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,0.f,-1.f,

	//front face
	-0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,
	-0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	0.f,0.f,1.f,

	//left face
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-0.5f,  0.5f, -0.5f,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-0.5f, -0.5f, -0.5f,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-0.5f, -0.5f, -0.5f,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	-1.f,0.f,0.f,

	//right face
	0.5f,  0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	0.5f, -0.5f, -0.5f, 	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,
	0.5f,  0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	1.f,0.f,0.f,

	//bottom face
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	-0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	0.f,-1.f,0.f,

	//top face
	-0.5f,  0.5f, -0.5f,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	0.5f,  0.5f, -0.5f,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.f, 1.0f,	0.f,1.f,0.f,
	-0.5f,  0.5f, -0.5f, 	1.0f, 1.f, 1.0f, 	0.f,1.f,0.f
};


#define NUM_BUFFERS 1
#define NUM_VAOS 1
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];

#define WIDTH 1024
#define HEIGHT 768

#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048

void drawFloorAndCubes(unsigned int shaderProgram)
{
	glBindVertexArray(VAOs[0]);

	//floor
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0, -3, 0));
	model = glm::scale(model, glm::vec3(100, 0.1, 100));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cubes
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			for (int k = -1; k < 2; k++)
			{
				glm::mat4 model = glm::mat4(1.f);
				model = glm::translate(model, glm::vec3(float(i * 2), float(j * 2), float(k * 2)));
				glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}

}

void setUpCubes()
{
	glCreateBuffers(NUM_BUFFERS, Buffers);
	glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, 0);
	glGenVertexArrays(NUM_VAOS, VAOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}





void renderWithShadow(unsigned int renderShaderProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(renderShaderProgram);
	glUniform3f(glGetUniformLocation(renderShaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(renderShaderProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(renderShaderProgram, "lightColour"), 0.f, 0.f, 1.f);
	glUniform3f(glGetUniformLocation(renderShaderProgram, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(renderShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 100.f);
	glUniformMatrix4fv(glGetUniformLocation(renderShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	drawFloorAndCubes(renderShaderProgram);
}

void renderWithTexture(unsigned int shader, vector<EngineMesh>eObjs, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 10000.f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 0.5f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);

	for (int i = 0; i < eObjs.size(); i++)
	{
		if (eObjs[i].getType() != shader) { continue; }
		eObjs[i].renderObj();
	}
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
}

void renderWithTextureP(unsigned int shader, vector<Planet>eObjs, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shader);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 10000.f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 0.5f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);

	for (int i = 0; i < eObjs.size(); i++)
	{
		//if (eObjs[i].getType() != shader) { continue; }
		eObjs[i].renderObj();
	}
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

}


void renderOceanP(unsigned int shader, vector<Planet>eObjs)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUseProgram(shader);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), (float)WIDTH / (float)HEIGHT, .01f, 10000.f);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glUniform3f(glGetUniformLocation(shader, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
	glUniform3f(glGetUniformLocation(shader, "lightColour"), 0.5f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(shader, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);

	for (int i = 0; i < eObjs.size(); i++)
	{
		//if (eObjs[i].getType() != shader) { continue; }
		eObjs[i].renderOcean();
	}
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(0);

}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE && action) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float x_offset = 0.f;
	float y_offset = 0.f;
	bool cam_changed = false;

	if (glfwGetKey(window, GLFW_KEY_RIGHT && action) == GLFW_PRESS)
	{
		x_offset = 1.f;
		y_offset = 0.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT && action) == GLFW_PRESS)
	{
		x_offset = -1.f;
		y_offset = 0.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_UP && action) == GLFW_PRESS)
	{
		x_offset = 0.f;
		y_offset = 1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN && action) == GLFW_PRESS)
	{
		x_offset = 0.f;
		y_offset = -1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_R && action) == GLFW_REPEAT)
	{
		cam_dist += 0.1f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F && action) == GLFW_REPEAT)
	{
		cam_dist -= 0.1f;
		cam_changed = true;
	}

	if (cam_changed)
	{
		MoveAndOrientCamera(Camera, glm::vec3(0.f, 0.f, 0.f), cam_dist, x_offset, y_offset);
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void SizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

void processKeyboard(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float x_offset = 0.f;
	float y_offset = 0.f;
	bool cam_changed = false;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		x_offset = 1.f;
		y_offset = 0.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		input_mat.x += 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		input_mat.x -= 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		input_mat.y -= 0.1;
	}


	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		input_mat.y += 0.1;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightDirection = Camera.Front;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		x_offset = -1.f;
		y_offset = 0.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		x_offset = 0.f;
		y_offset = 1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		x_offset = 0.f;
		y_offset = -1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		cam_dist += .5f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		cam_dist -= .5f;
		cam_changed = true;
	}

	if (cam_changed)
	{
		
		MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), cam_dist, x_offset, y_offset);
		std::cout << Camera.Front.x << " , " << Camera.Front.y << " , " << Camera.Front.z << " : " << std::endl;

	}
}

void generateDepthMap(unsigned int shadowShaderProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix)
{


	glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
	//drawFloorAndCubes(shadowShaderProgram);
	renderWithTextureP(shadowShaderProgram, planetz, shadow, projectedLightSpaceMatrix);
	renderWithTexture(shadowShaderProgram, eObjs, shadow, projectedLightSpaceMatrix);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


}

int main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Planetarium!", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, KeyCallback);

	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);

	ShadowStruct shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);

	GLuint program = CompileShader("phong.vert", "phong.frag");
	GLuint shadow_program = CompileShader("shadow.vert", "shadow.frag");

	GLuint texture_program = CompileShader("textured.vert", "textured.frag");
	GLuint skybox_shader = CompileShader("skybox.vert", "skybox.frag");
	GLuint simple_planet = CompileShader("simplePlanetShader.vert", "simplePlanetShader.frag");


	glDebugMessageCallback(DebguMessageCallback, 0);

	InitCamera(Camera);
	cam_dist = 5.f;
	MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), cam_dist, 0.f, 0.f);

	
	//SECTION A - EDIT THIS CODE TO TEST
	//EngineMesh tree = EngineMesh("objs/white_oak/white_oak.obj", texture_program);
	EngineMesh halo = EngineMesh("objs/halothree/halothree.obj", texture_program);
	EngineMesh station = EngineMesh("objs/station/spaceStation.obj", texture_program);
	EngineMesh ship = EngineMesh("objs/fighter/fighter.obj", texture_program);


	ship.loadObj();
	ship.setUpObject();

	halo.loadObj();
	halo.setUpObject();
	halo.getTransform()->scale = glm::vec3(1, 1, 1);
	halo.getTransform()->rotation = glm::vec3(30, 20, 1);

	station.loadObj();
	station.setUpObject();
	station.getTransform()->scale = glm::vec3(0.5f, 0.5f, 0.5f);
	station.getTransform()->position.x += 43;
	station.getTransform()->position.z += 40;

	eObjs.push_back(ship);
	eObjs.push_back(halo);
	eObjs.push_back(station);

	player = PlayerControlled(texture_program, &ship);


	Planet mars = Planet(texture_program, program , 0.2f, 130.0f);
	mars.loadObj();
	mars.setUpObject();
	mars.setupPlanet();
	mars.setupOcean();

	Planet june = Planet(texture_program, program , 0.04f, 80.f);
	june.loadObj();
	june.setUpObject();
	june.setupPlanet();
	june.setupOcean();

	june.getTransform()->position.x += 60;
	planetz.push_back(mars);
	planetz.push_back(june);

	//SetUpSkybox
	Skybox skybox = Skybox();
	skybox.setupCubemap();


	setUpCubes();
	glEnable(GL_DEPTH_TEST);
	MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), cam_dist, 0.f, 0.f);

	while (!glfwWindowShouldClose(window))
	{
		clampInput();
		dampInput();

		float near_plane = 1.0f, far_plane = 70.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projectedLightSpaceMatrix = lightProjection * lightView;

		//(shadow_program, shadow, projectedLightSpaceMatrix);
		

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//generateDepthMap(shadow_program, shadow, projectedLightSpaceMatrix);
		//saveShadowMapToBitmap(shadow.Texture, SH_MAP_WIDTH, SH_MAP_HEIGHT);
		glViewport(0, 0, WIDTH, HEIGHT);
		player.Update();
		ship.getTransform()->position.x += input_mat.x;
		ship.getTransform()->position.y += input_mat.y;

		skybox.renderCubemap(skybox_shader, &Camera);

		//renderWithShadow(texture_program, shadow, glm::mat4(1));
		renderWithTextureP(texture_program, planetz, shadow, projectedLightSpaceMatrix);

		renderOceanP(texture_program, planetz);

		renderWithTexture(texture_program, eObjs, shadow, projectedLightSpaceMatrix);
		glfwSwapBuffers(window);
		glfwPollEvents();
		processKeyboard(window);

	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}