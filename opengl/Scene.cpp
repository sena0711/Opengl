#include "Scene.h"
#include <iostream>
#include <cmath>

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW

#include <glm/glm/glm.hpp> //vec3, vec4, ivec4, mat4
#include <glm/glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
//Other lib
#include <SOIL/SOIL.h>
// Other includes
#include "camera.h"
//#include "mesh.h"
//#include "model.h"
#include "shader.h"
#include "ResourceManager.h"



CScene::CScene(GLuint width, GLuint height)
{
	Width = width;
	Height = height;
	// Light attributes
	lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

}

CScene::~CScene()
{
}

void CScene::Init()
{//// Build and compile our shader program
	ResourceManager::LoadShader("Assets/Shaders/materials.vs", "Assets/Shaders/materials.fs", nullptr, "lightingShader");
	ResourceManager::LoadShader("Assets/Shaders/lamp.vs", "Assets/Shaders/lamp.fs", nullptr, "lampShader");
	
		// Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	// First, set the container's VAO (and VBO)
	
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the normal vectors
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}

void CScene::ProcessInput(GLfloat dt)
{
	
}

void CScene::Update(GLfloat dt)
{
}

void CScene::Render(Camera camera)
{
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
	glm::mat4 view;
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)Width / (float)Height, 0.1f, 100.0f);
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

}
