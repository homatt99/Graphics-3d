#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace std;

void error_callback(int error, const char* description);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

const char *vertexShaderSource = 
	"#version 410 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main() {\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = 
	"#version 410 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vec4(gl_FragCoord.x/640.0, gl_FragCoord.y/480.0, 0.7f, 1.0f);\n"
	"}\n\0";

//g++ -I/Users/ericwang/Desktop/Graphics3D/include -L/Users/ericwang/Desktop/Graphics3D/libs -o Graphics3D Main.cpp -lglfw -lGLEW -framework OpenGl
int main() {
	//Initialize and configure GLFW
	if (!glfwInit()) {
		return EXIT_FAILURE;
	}
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//GLFW Window Creation
	GLFWwindow* window = glfwCreateWindow(640, 480, "Graphics3D", NULL, NULL);
	if (!window) {
		cout << "Failed to create window" << endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 640, 480);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
	glfwMakeContextCurrent(window);

	//GLEW
	GLenum glew_status = glewInit();
	if(glew_status != GLEW_OK){
		cerr << "Error: glewINIT: " << glewGetErrorString(glew_status) << endl;
		return EXIT_FAILURE;
    }

	//Vertex Shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Delete shaders - no longer needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//vertices
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};  

	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}

void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);
}



