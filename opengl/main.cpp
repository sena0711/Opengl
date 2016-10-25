

// GLEW
//#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "camera.h"
#include "ResourceManager.h"
#include "Scene.h"



// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void do_light_movement();
void back_Culling();
bool enableCulling = false;
GLuint generateMultiSampleTexture(GLuint samples);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Camera
Camera  camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool    keys[1024];


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame


//ATIALISING framebuffer
GLuint framebuffer;


CScene game(WIDTH, HEIGHT);
							// The MAIN function, from here we start the application and run the game loop
int main()
{
	camera.lightmovement = NONE;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.
	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	
	// OpenGL options
	glEnable(GL_MULTISAMPLE); // multi sampling for antialising
	glEnable(GL_DEPTH_TEST);

	
	game.Init();

	game.State = GAME_ACTIVE;



	//  Antialising
	// Framebuffers

	// Framebuffers

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a multisampled color attachment texture
	GLuint textureColorBufferMultiSampled = generateMultiSampleTexture(4);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
	// Create a renderbuffer object for depth and stencil attachments
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();
		do_light_movement();
		back_Culling();

		game.ProcessInput(deltaTime);
		game.Update(deltaTime);
		// Clear the colorbuffer

		// 1. Draw scene as normal in multisampled buffers
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.Render(camera);

		// 2. Now blit multisampled buffer(s) to default framebuffers
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, WIDTH, HEIGHT, 0, 0, WIDTH, HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	
		glfwSwapBuffers(window);
	}
	
	

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void back_Culling()
{
	if (keys[GLFW_KEY_B])
	{
		if (enableCulling == false)
		{
			enableCulling = true;
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
		}
		else
		{
			enableCulling = false;
			glDisable(GL_CULL_FACE);
		}
		
	}
		

}
void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	

}
void do_light_movement()
{
	if (keys[GLFW_KEY_I])
		camera.Setlightmovement(FORWARD);
	if (keys[GLFW_KEY_K])
		camera.Setlightmovement(BACKWARD);
	if (keys[GLFW_KEY_J])
		camera.Setlightmovement(LEFT);
	if (keys[GLFW_KEY_L])
		camera.Setlightmovement(RIGHT);
}


GLuint generateMultiSampleTexture(GLuint samples)
{
	GLuint texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, WIDTH, HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	return texture;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (GLfloat)xpos;
		lastY = (GLfloat)ypos;
		firstMouse = false;
	}

	GLfloat xoffset = (GLfloat)xpos - lastX;
	GLfloat yoffset = lastY - (GLfloat)ypos;  // Reversed since y-coordinates go from bottom to left

	lastX = (GLfloat)xpos;
	lastY = (GLfloat)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((GLfloat)yoffset);
}