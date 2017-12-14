#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <iostream>

void errorCallback(int _error, const char * _description)
{
	fprintf(stderr, "Error: %s\n", _description);
}
void keyCallback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, GLFW_TRUE);
}


void setCallbackFunctions(GLFWwindow * _window)
{
	glfwSetErrorCallback(errorCallback);
	glfwSetKeyCallback(_window, keyCallback);
}


int main()
{
	//Initialise GLFW
	if (!glfwInit())
	{
		// Initialization failed
		std::cout << "Failed to initialise GLFW" << std::endl;
		return 0;
	}

	//Create window
	GLFWwindow * window = glfwCreateWindow(1280, 800, "My Title", NULL, NULL);
	if (!window)
	{
		// Window or OpenGL context creation failed
		std::cout << "Failed to create window" << std::endl;
		return 0;
	}

	//Setup callback functions
	setCallbackFunctions(window);

	//Set current context
	glfwMakeContextCurrent(window);

	//Initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// Glew loading failed
		std::cout << "Failed to initialise GLEW" << std::endl;
		return 0;
	}
	

	//Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Keep running
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glViewport(0, 0, width, height);
		glClearColor(1.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	glfwTerminate();
	return 1;
}

