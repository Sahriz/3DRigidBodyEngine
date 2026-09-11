#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	glm::mat4 getViewMatrix() const;
	glm::mat4 getProjectionMatrix() const;

	void moveLocal(glm::vec3 direction, float deltaTime);
	void look(float uawDelta, float pitchDelta);

private:
	glm::vec3 position{0.0f, 0.0f, 3.0f};
	float yaw = -90.f;
	float pitch = 0.f;

	float movementSpeed = 3.0f;
	float fieldOfView = 45.0f;
};

std::string const SHADER_PATH = SHADER_DIR;

constexpr int Width = 800;
constexpr int Height = 800;

unsigned int ShaderProgram;
unsigned int VAO;
unsigned int EBO;

glm::vec3 vertices[] = 
{
	{-0.5f, -0.5f, -0.5f},	// 0
	{-0.5f, -0.5f, 0.5f},	// 1
	{0.5f, -0.5f, 0.5f},	// 2
	{0.5f, -0.5f, -0.5f},	// 3
	{-0.5f, -0.5f, 0.5f},	// 4
	{0.5f, -0.5f, 0.5f},	// 5
	{0.5f, 0.5f, 0.5f},		// 6
	{-0.5f, 0.5f, 0.5f},	// 7
	{0.5f, -0.5f, 0.5f},	// 8
	{0.5f, -0.5f, -0.5f},	// 9
	{0.5f, 0.5f, -0.5f},	// 10
	{0.5f, 0.5f, 0.5f},		// 11
	{0.5f, -0.5f, -0.5f},	// 12
	{-0.5f, -0.5f, -0.5f},	// 13
	{-0.5f, 0.5f, -0.5f},	// 14 
	{0.5f, 0.5f, -0.5f},	// 15
	{-0.5f, -0.5f, -0.5f},	// 16
	{-0.5f, -0.5f, 0.5f},	// 17
	{-0.5f, 0.5f, 0.5f},	// 18
	{-0.5f, 0.5f, -0.5f},	// 19
	{-0.5f, 0.5f, -0.5f},	// 20
	{-0.5f, 0.5f, 0.5f},	// 21
	{0.5f, 0.5f, 0.5f},		// 22
	{0.5f, 0.5f, -0.5f}		// 23
};
unsigned int indices[] = 
{  
	0, 2, 1,		0, 3, 2,	// bottom
	4, 5, 7,		5, 6, 7,	// front
	8, 9, 11,		9, 10, 11,	//right
	12, 13, 14,		12, 14, 15, // behind
	16, 17, 18,		16, 18 ,19, // left
	20, 21, 22,		20, 22, 23	// top
};

glm::vec3 normals[] =
{
	{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, // Bottom
	{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},		// Front
	{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},		// Right
	{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},	// Behind
	{-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{-1.0f, 0.0f, 0.0f},	// Left
	{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}		// Top
};

std::string readFile(std::string path) 
{
	std::ifstream in(path);
	std::string contents((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());

	return contents;
}

void CreateShaderProgram(std::string const source) 
{
	std::string vertexShaderSourceString = readFile(source + "shader.vert");
	std::string fragmentShaderSourceString = readFile(source + "shader.frag");

	const char* vertexShaderSource = vertexShaderSourceString.c_str();
	const char* fragmentShaderSource = fragmentShaderSourceString.c_str();

	unsigned int vertexShader;
	unsigned int fragmentShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	int  success;
	char infoLog[512];
	

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	ShaderProgram = glCreateProgram();

	glAttachShader(ShaderProgram, vertexShader);
	glAttachShader(ShaderProgram, fragmentShader);
	glLinkProgram(ShaderProgram);

	glUseProgram(ShaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) 
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

template<std::size_t N>
GLuint createAttribBuffer(GLuint location, const glm::vec3(&data)[N]) 
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(sizeof(data)),
		data,
		GL_STATIC_DRAW
	);

	glVertexAttribPointer(
		location,
		3,
		GL_FLOAT,
		GL_FALSE,
		static_cast<GLsizei>(sizeof(glm::vec3)),
		nullptr
	);

	glEnableVertexAttribArray(location);

	return buffer;
}

template<size_t N>
GLuint createIndexBuffer(const unsigned int(&indices)[N]) 
{
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(sizeof(indices)),
		indices,
		GL_STATIC_DRAW
	);

	return buffer;
}

int main() 
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window
	GLFWwindow* window = glfwCreateWindow(Width, Height, "window", NULL, NULL);
	if(window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	CreateShaderProgram(SHADER_PATH);
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint positionVBO	= createAttribBuffer(0, vertices);
	GLuint normalVBO	= createAttribBuffer(1, normals);

	EBO					= createIndexBuffer(indices);

	

	//set screen dimensions
	glViewport(0, 0, Width, Height);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	unsigned int rotationLoc = glGetUniformLocation(ShaderProgram, "rotation");

	glm::mat4 trans = glm::mat4(1.0f);
	
	float time = glfwGetTime();
	float lastTime = time;
	float deltaTime = 0.0;
	//renderloop
	while (!glfwWindowShouldClose(window)) {
		lastTime = time;
		time = glfwGetTime();
		deltaTime = time - lastTime;
		trans = glm::rotate(trans, 10*glm::radians(10*deltaTime), glm::vec3(1.0, 0.0, 0.0));
		processInput(window);

		//rendering commands here
		glClearColor(0.2, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(ShaderProgram);
		glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}