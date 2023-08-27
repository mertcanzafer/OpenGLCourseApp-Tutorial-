#pragma once
#include <iostream>
#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iomanip>

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

		 // getters for keys changes in corrd. X and Y
		 bool* getKeys() { return keys; }
		 GLfloat getXchange();
		 GLfloat getYChange();

		 void FPSCounter();
		 void swapBuffers(){ glfwSwapBuffers(MainWindow); }

		 ~Window();
	  
	  private:

		  GLFWwindow* MainWindow; // our window
		  GLint width, height;
		  GLint bufferWidth, bufferHeight; // Get buffer size Info.

		  // Important Variables for FPS
		  double currentTime, prevTime, TimeDiff;
		  unsigned int counter;

		  // Some important variables for controls keys on the keyboard and mause
		  bool keys[1024]; // Range of ASCII code

		  GLfloat lastX, lastY, xChange, yChange;
		  /*
		   lastX,lasty: to Measure the last coordiantes
		   xChange, yChange: Calculate the change
		  */

		  bool MouseFirstMoved; 

		  static void handleKeys(GLFWwindow* window,int key,int code,int action,int mode);
		  static void handleMouse(GLFWwindow* window, double Xpos, double Ypos);

		  void CreateCallBacks();

	};

}

