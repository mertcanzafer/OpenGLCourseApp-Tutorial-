#pragma once
#include <iostream>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

namespace WNS 
{
	class Window
	{
	  public:
		 Window();
		 Window(GLint windowWidth, GLint windowHeight);

		 int Initialise();

		 // getters
		 GLfloat GetbufferWidth(){ return bufferWidth; }
		 GLfloat GetbufferHeight() { return bufferHeight; }

		 bool getShouldClose() { return glfwWindowShouldClose(MainWindow); }
		 void swapBuffers(){ glfwSwapBuffers(MainWindow); }

		 ~Window();
	  
	  private:

		  GLFWwindow* MainWindow; // our window
		  GLint width, height;
		  GLint bufferWidth, bufferHeight; // Get buffer size Info.

	};

}

