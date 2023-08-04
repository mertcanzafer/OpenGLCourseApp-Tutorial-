#include "Window.h"

using namespace WNS;

Window::Window()
	:width{800},height{600}
{
}

Window::Window
(
	GLint windowWidth, GLint windowHeight
)
{
	this->width = windowWidth;
	this->height = windowHeight;
}

int Window::Initialise()
{
	if (!glfwInit())
	{
		std::cout << "GLFW initialisation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}
	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Set the profile
	// Core Profile = No Backwards Compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow Forward Compatbility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Creating our window...

	MainWindow = glfwCreateWindow(width, height, "3D Game Engine", NULL, NULL);
	if (!MainWindow)
	{
		std::cout << "GLFW window creation failed!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// Get buffer size Info.
	glfwGetFramebufferSize(MainWindow, &bufferWidth, &bufferHeight);

	// Set the context for GLEW to use
	glfwMakeContextCurrent(MainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW initialisation failed!" << std::endl;
		glfwDestroyWindow(MainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	// Setup Viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

}

Window::~Window()
{
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}