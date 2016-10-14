//#include "Dependencies\glew\glew.h"
//#include "Dependencies\freeglut\freeglut.h"
//#include <iostream>
#include "ShaderLoader.h" 
#include "GameModels.h"

#include<iostream>
#include<stdio.h>;
#include<stdlib.h>;
#include<fstream>;
#include<vector>;
Models::GameModels* gameModels;
GLuint program;
void renderScene(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.3, 0.3, 1.0);

	glBindVertexArray(gameModels->GetModel("triangle1"));
	glUseProgram(program);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

void Init()
{
	glEnable(GL_DEPTH_TEST);

	gameModels = new Models::GameModels();
	gameModels->CreateTriangleModel("triangle1");


	ShaderLoader sahderLoader;
	program = sahderLoader.CreateProgram("Shaders\\VAO_Triangle.vs",
		"Shaders\\VAO_Triangle.fs");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void closeCallback()
{

	std::cout << "GLUT:\t Finished" << std::endl;
	glutLeaveMainLoop();
}
int main(int argc, char **argv)
{

	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(500, 300);
	glutInitWindowSize(800, 600);
	glutCreateWindow("VAO Triangle");

	glewInit();
	if (glewIsSupported("GL_VERSION_4_5"))
	{
		std::cout << " GLEW Version is 4.5\n ";
	}
	else
	{
		std::cout << "GLEW 4.5 not supported\n ";
	}


									 // register callbacks
	Init();
	// register callbacks
	glutDisplayFunc(renderScene);
	glutCloseFunc(closeCallback);

	glutMainLoop();

	delete gameModels;
	glDeleteProgram(program);

	return 0;
}