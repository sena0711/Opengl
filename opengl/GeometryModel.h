#pragma once

#include <GL/glew.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include "camera.h"
class GeometryModel
{
public:
	GeometryModel();
	~GeometryModel();
	void InitGeometryModel(GLuint program, Camera* camera);
	void render();
	void setPosition(glm::vec3 pos);
	GLuint program;
	Camera *camera;
	GLuint vbo, vao;

	glm::vec3 position;

};

