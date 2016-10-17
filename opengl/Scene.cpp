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

//#include "mesh.h"
//#include "model.h"
#include "shader.h"
#include "ResourceManager.h"
#include "SceneLevel.h"




CScene::CScene(GLuint width, GLuint height)
{
	Width = width;
	Height = height;
	currentLevel = 1;

	NumberOfLevels = 1;
	
}

CScene::~CScene()
{
}

void CScene::Init()
{
	for (int i = 0; i < NumberOfLevels; i++) {
		CSceneLevel scene_level;
		scene_level.InitLevel(i);
		Levelvec.push_back(scene_level);

	}



}

void CScene::ProcessInput(GLfloat dt)
{
	
}

void CScene::Update(GLfloat dt)
{
}

void CScene::Render(Camera camera)
{
	int i_currentLevel= currentLevel - 1; 
	Levelvec[i_currentLevel].Render(camera);
	

}
