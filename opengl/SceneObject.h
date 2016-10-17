#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <GL/glew.h>
#include <glm/glm/glm.hpp>
enum ObjectShape {
	PYRAMID,
	CUBE,
	SPHERE,
};
class CSceneObject
{
public:

	// Object state
	ObjectShape shape;

	glm::vec3   Position, Size, Velocity;
	glm::vec3   Color;
	GLfloat     Rotation;
	GLboolean   IsSolid;
	GLboolean   Destroyed;
	// Render state
	
	// Constructor(s)
	CSceneObject();
	~CSceneObject();
	virtual void init();
	// Draw sprite

	bool enablePosAttrib, enableNormAttrib;
	
	GLuint m_VBO, m_VAO;
	
};

#endif


