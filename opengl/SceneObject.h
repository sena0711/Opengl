#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <GL/glew.h>
#include <glm/glm/glm.hpp>

class CSceneObject
{
public:

	// Object state
	GLfloat vertices[350];
	glm::vec3   Position, Size, Velocity;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	// Render state
	
	// Constructor(s)
	CSceneObject();
	~CSceneObject();
	
	// Draw sprite
	
};

#endif


