#include "camera.h"
#include <iostream>
#include <cmath>


//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW

#include <glm/glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
//Other lib
#include <SOIL/SOIL.h>

#include "SceneLevel.h"
#include "MaterialBox.h"
#include "LoadModel.h"
#include "GeometryModel.h"


GeometryModel *geomModel;
//MaterialBox matbox;


GLuint loadCubemap(vector<const GLchar*> faces);


CSceneLevel::CSceneLevel(GLuint width, GLuint height)
{
	Width = width, Height = height;
}


CSceneLevel::~CSceneLevel()
{
}

void CSceneLevel::InitLevel(int level)
{	// Light attributes
	lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	//// Build and compile our shader program
	ResourceManager::LoadShader("Assets/Shaders/materials.vs", "Assets/Shaders/materials.fs", nullptr, "lightingShader");
	ResourceManager::LoadShader("Assets/Shaders/lamp.vs", "Assets/Shaders/lamp.fs", nullptr, "lampShader");
	ResourceManager::LoadShader("Assets/Shaders/model.vs", "Assets/Shaders/model.fs", nullptr, "modelShader");
	
	ResourceManager::LoadShader("Assets/Shaders/skybox.vs", "Assets/Shaders/skybox.fs", nullptr, "SkyboxShader");


	ResourceManager::LoadShader("Assets/Shaders/star.vs", "Assets/Shaders/star.fs", "Assets/Shaders/star.gs", "StarShader");
	// geometrymodel star
	geomModel = new GeometryModel();
	geomModel->setPosition(glm::vec3(6.0f, 1.0f, 0.0f));

	
	
	/*CLoadModel CastleModel("Assets/Models/castle/Castle OBJ.obj");
	CLoadModel NanoModel("Assets/Models/nanosuit/nanosuit.obj");*/
	//ResourceManager::LoadShader("Assets/Shaders/lamp.vs", "Assets/Shaders/lamp.fs", nullptr, "CastleModel");
	// Set up vertex data (and buffer(s)) and attribute pointers
	
	GLfloat vertices[] = {
		// Positions           // Normals           // Texture Coords
		// Back Face
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		// Front Face
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		// Left Face
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		// Right Face
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		// Bottom Face
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		// Top Face
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f
	};

	// Skybox vertices
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	// First, set the container's VAO (and VBO)

	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	// Texture Coords Attribute
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glBindVertexArray(0);

	//matbox.Init();


//	the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);



	// Setup skybox VAO

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Cubemap/Skybox
	vector<const GLchar*> faces;
	faces.push_back("Assets/Skybox/right.jpg");
	faces.push_back("Assets/Skybox/left.jpg");
	faces.push_back("Assets/Skybox/top.jpg");
	faces.push_back("Assets/Skybox/bottom.jpg");
	faces.push_back("Assets/Skybox/back.jpg");
	faces.push_back("Assets/Skybox/front.jpg");
	 cubemapTexture = loadCubemap(faces);

}

void CSceneLevel::Load(const GLchar * file, GLuint levelWidth, GLuint levelHeight)
{
}

void CSceneLevel::Render(Camera camera)
{
	
	// Draw skybox first
	glDepthMask(GL_FALSE);// Remember to turn depth writing off for skybox
	
	glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 300.0f);

						  // Activate skybox shader
	
	ResourceManager::GetShader("SkyboxShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("SkyboxShader").Use().SetMatrix4("projection", projection);

	// Skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetShader("SkyboxShader").Use().SetInteger("skybox", 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);


	glEnable(GL_FOG);

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogi(GL_FOG_INDEX, 32);
	glFogf(GL_FOG_START, 1.0);
	glFogf(GL_FOG_END, 6.0);
	glHint(GL_FOG_HINT, GL_NICEST);
	glClearIndex((GLfloat)(32 + 16 - 1));


						  // Change the light's position values over time (can be done anywhere in the game loop actually, but try to do it at least before using the light source positions)
	lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.position", lightPos.x, lightPos.y, lightPos.z);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);


	// Set lights properties
	glm::vec3 lightColor;
	lightColor.x = sin(glfwGetTime() * 2.0f);
	lightColor.y = sin(glfwGetTime() * 0.7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);
	// Set material properties
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
	ResourceManager::GetShader("lightingShader").Use().SetVector3f("material.specular", 0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
	ResourceManager::GetShader("lightingShader").Use().SetFloat("material.shininess", 32.0f);

	// Create camera transformations

	view = camera.GetViewMatrix();
	//projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
	// Get the uniform locations

	ResourceManager::GetShader("lightingShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("lightingShader").Use().SetMatrix4("projection", projection);
	glBindVertexArray(containerVAO);
	glm::mat4 model;

	ResourceManager::GetShader("lightingShader").Use().SetMatrix4("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);




	// Also draw the lamp object, again binding the appropriate shader

	ResourceManager::GetShader("lampShader").Use().SetMatrix4("view", view);
	ResourceManager::GetShader("lampShader").Use().SetMatrix4("projection", projection);


	model = glm::mat4();
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube

	ResourceManager::GetShader("lampShader").Use().SetMatrix4("model", model);
	// Draw the light object (using light's vertex attributes)
	glBindVertexArray(lightVAO);


	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	geomModel->render(camera);

}

GLboolean CSceneLevel::IsCompleted()
{
	return GLboolean();
}

GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void CSceneLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
}
