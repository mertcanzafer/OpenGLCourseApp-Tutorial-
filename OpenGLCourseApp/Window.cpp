#include "Window.h"

using namespace WNS;

Window::Window()
	:width{800},height{600},xChange{0.0f},yChange{0.0f}
{
	for (auto &b : keys)
	{
		b = false;
	}
}

Window::Window
(
	GLint windowWidth, GLint windowHeight
)
{
	this->width = windowWidth;
	this->height = windowHeight;
	this->xChange = 0.0f;
	this->yChange = 0.0f;
	
	for (auto& b : keys)
	{
		b = false;
	}
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

	// Handle key + mause input 
	CreateCallBacks();
	glfwSetInputMode(MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

	glfwSetWindowUserPointer(MainWindow, this);
}

void Window::CreateCallBacks()
{
	glfwSetKeyCallback(MainWindow, handleKeys);
	glfwSetCursorPosCallback(MainWindow, handleMouse);
}

void Window::handleKeys
(
	GLFWwindow* window, int key, int code, int action, int mode
)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	// Check if the key is proper in the interval.
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;	
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}

}

void Window::handleMouse
(
	GLFWwindow* window, double Xpos, double Ypos
)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->MouseFirstMoved)
	{
		theWindow->lastX = Xpos;
		theWindow->lastY = Ypos;
		theWindow->MouseFirstMoved = false;
	}

	theWindow->xChange = Xpos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - Ypos;

	theWindow->lastX = Xpos;
	theWindow->lastY = Ypos;

}

GLfloat Window::getXchange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

Window::~Window()
{
	glfwDestroyWindow(MainWindow);
	glfwTerminate();
}