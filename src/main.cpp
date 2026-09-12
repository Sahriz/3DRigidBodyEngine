#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <vector>
#include <utility>                // std::exchange
#include <glm/gtc/quaternion.hpp> // glm::quat and glm::mat4_cast



std::string const SHADER_PATH = SHADER_DIR;

struct MeshID {
	std::size_t index;
};

struct Transform {
	glm::vec3 position{ 0.0f };
	glm::quat  rotation{ 1.0f, 0.0f, 0.0f, 0.0f };
	glm::vec3 scale{ 1.0f };
};

class SimulatedObject {
public:
	SimulatedObject(MeshID id, Transform initialTransform) : 
		mesh{ id }, transform{ initialTransform } {
	}

	MeshID getMesh() const {
		return mesh;
	}

	Transform getTransform() const {
		return transform;
	}
private:
	MeshID mesh;
	Transform transform;
	glm::vec3 visualScale{1.0f};
};



struct MeshData {
private:
public:
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
};
MeshData createCubeMeshData() {
	MeshData data;
	data.positions = { 
		{ -0.5f, -0.5f, -0.5f },	// 0
		{ -0.5f, -0.5f, 0.5f },		// 1
		{ 0.5f, -0.5f, 0.5f },		// 2
		{ 0.5f, -0.5f, -0.5f },		// 3
		{ -0.5f, -0.5f, 0.5f },		// 4
		{ 0.5f, -0.5f, 0.5f },		// 5
		{ 0.5f, 0.5f, 0.5f },		// 6
		{ -0.5f, 0.5f, 0.5f },		// 7
		{ 0.5f, -0.5f, 0.5f },		// 8
		{ 0.5f, -0.5f, -0.5f },		// 9
		{ 0.5f, 0.5f, -0.5f },		// 10
		{ 0.5f, 0.5f, 0.5f },		// 11
		{ 0.5f, -0.5f, -0.5f },		// 12
		{ -0.5f, -0.5f, -0.5f },	// 13
		{ -0.5f, 0.5f, -0.5f },		// 14 
		{ 0.5f, 0.5f, -0.5f },		// 15
		{ -0.5f, -0.5f, -0.5f },	// 16
		{ -0.5f, -0.5f, 0.5f },		// 17
		{ -0.5f, 0.5f, 0.5f },		// 18
		{ -0.5f, 0.5f, -0.5f },		// 19
		{ -0.5f, 0.5f, -0.5f },		// 20
		{ -0.5f, 0.5f, 0.5f },		// 21
		{ 0.5f, 0.5f, 0.5f },		// 22
		{ 0.5f, 0.5f, -0.5f } 		// 23
	};

	data.normals = { 
		{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, // Bottom
		{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},		// Front
		{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},		// Right
		{0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},	// Behind
		{-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},{-1.0f, 0.0f, 0.0f},	// Left
		{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}		// Top
	};
	data.indices = {
		0, 2, 1,		0, 3, 2,	// bottom
		4, 5, 7,		5, 6, 7,	// front
		8, 9, 11,		9, 10, 11,	//right
		12, 13, 14,		12, 14, 15, // behind
		16, 17, 18,		16, 18 ,19, // left
		20, 21, 22,		20, 22, 23	// top
	};
	return data;
}
MeshData createPyramidMeshData();



glm::mat4 modelMatrix(const Transform& transform) {
	glm::mat4 translation = glm::translate(
		glm::mat4(1.0f),
		transform.position
	);

	glm::mat4 rotation = glm::mat4_cast(transform.rotation);

	glm::mat4 scale = glm::scale(
		glm::mat4(1.0f),
		transform.scale
	);

	return translation * rotation * scale;
}

class Mesh {
public:
	Mesh() = default;

	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept 
		: vao(std::exchange(other.vao, 0)),
		positionVBO(std::exchange(other.positionVBO, 0)),
		normalVBO(std::exchange(other.normalVBO, 0)),
		ebo(std::exchange(other.ebo, 0)), 
		indexCount(std::exchange(other.indexCount, 0))
	{}

	Mesh& operator=(Mesh&&) = delete;

	
	void init(const MeshData& data) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		positionVBO = createAttribBuffer(0, data.positions);
		normalVBO = createAttribBuffer(1, data.normals);

		ebo = createIndexBuffer(data.indices);

		indexCount = static_cast<GLsizei>(data.indices.size());
	}

	~Mesh() {
		glDeleteBuffers(1, &positionVBO);
		glDeleteBuffers(1, &normalVBO);
		glDeleteBuffers(1, &ebo);
		glDeleteVertexArrays(1, &vao);

	}

	

	void draw() const {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}

private:

	GLuint vao			= 0;
	GLuint positionVBO	= 0;
	GLuint normalVBO	= 0;
	GLuint ebo			= 0;

	GLsizei indexCount	= 0;

	GLuint createAttribBuffer(GLuint location, const std::vector<glm::vec3>& data)
	{
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glBufferData(
			GL_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(data.size() * sizeof(glm::vec3)),
			data.data(),
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

	GLuint createIndexBuffer(const std::vector<unsigned int>& indices)
	{
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);

		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
			indices.data(),
			GL_STATIC_DRAW
		);

		return buffer;
	}
};

class Camera {
public:
	glm::mat4 getViewMatrix() const {
		glm::vec3 forward = getForward();

		return glm::lookAt(
			position,
			position + forward,
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
	}
	glm::mat4 getProjectionMatrix(int Width, int Height) const {
		return glm::perspective(glm::radians(fieldOfView), static_cast<float>(Width) / Height, 0.1f, 100.0f);
	}

	void moveLocal(glm::vec3 direction, float deltaTime) {
		glm::vec3 forward = getForward();
		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
		glm::vec3 up = glm::vec3(0, 1, 0);

		position += (
			right * direction.x +
			up * direction.y +
			forward * direction.z
			) * movementSpeed * deltaTime;
	}
	void look(float yawDelta, float pitchDelta) {
		yaw += yawDelta;
		pitch = std::clamp(pitch + pitchDelta, -89.0f, 89.0f);
	}

private:
	glm::vec3 getForward() const {
		const float yawRadiands = glm::radians(yaw);
		const float pitchRadians = glm::radians(pitch);

		glm::vec3 forward{
			std::cos(yawRadiands) * std::cos(pitchRadians),
			std::sin(pitchRadians),
			std::sin(yawRadiands) * std::cos(pitchRadians)
		};

		forward = glm::normalize(forward);
		return forward;
	}

	glm::vec3 position{ 0.0f, 0.0f, 3.0f };
	float yaw = -90.f;
	float pitch = 0.f;

	float movementSpeed = 3.0f;
	float fieldOfView = 45.0f;
};

class Shader {
public:
	void InitProgram(std::string const source) {
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

	~Shader() {
		glDeleteProgram(ShaderProgram);
	}

	GLuint getShader() {
		return ShaderProgram;
	}
private:
	GLuint ShaderProgram = 0;

	std::string readFile(std::string path) {
		std::ifstream in(path);
		std::string contents((std::istreambuf_iterator<char>(in)),
			std::istreambuf_iterator<char>());

		return contents;
	}
};

class Renderer {
public:

	void init() {
		shader.InitProgram(SHADER_PATH);

	}

	MeshID createMesh(const MeshData& data) {
		MeshID id{ meshes.size() };

		meshes.emplace_back();
		meshes.back().init(data);

		return id;
	}

	void beginFrame(const Camera camera, int framebufferWidth, int framebufferHeight) {
		glClearColor(0.2, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		view = camera.getViewMatrix();
		projection = camera.getProjectionMatrix(framebufferWidth, framebufferHeight);		
	}

	void draw(MeshID id, const glm::mat4& model) {
		GLint viewLoc = glGetUniformLocation(shader.getShader(), "view");
		GLint projectionLoc = glGetUniformLocation(shader.getShader(), "projection");
		GLint modelLoc = glGetUniformLocation(shader.getShader(), "model");

		glUseProgram(shader.getShader());
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Mesh& mesh = meshes[id.index];
		mesh.draw();
	}


private:
	Shader shader;

	std::vector<Mesh> meshes;

	glm::mat4 view{ 1.0f };
	glm::mat4 projection{ 1.0f };
};

Camera camera;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, float deltaTime) 
{
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	static double previousX = mouseX;
	static double previousY = mouseY;

	float deltaX = static_cast<float>(mouseX - previousX);
	float deltaY = static_cast<float>(mouseY -previousY);

	previousX = mouseX;
	previousY = mouseY;

	constexpr float sensitivity = 0.1f;
	camera.look(deltaX * sensitivity, -deltaY * sensitivity);


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.moveLocal(glm::vec3(0, 0, 1), deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.moveLocal(glm::vec3(1, 0, 0), deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.moveLocal(glm::vec3(-1, 0, 0), deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.moveLocal(glm::vec3(0, 0, -1), deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		camera.moveLocal(glm::vec3(0, 1, 0), deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		camera.moveLocal(glm::vec3(0, -1, 0), deltaTime);
	}
}





int main() 
{
	int Width = 800, Height = 800;

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	{
		Renderer renderer;
		renderer.init();

		std::vector<SimulatedObject> meshes;

		//set screen dimensions
		glViewport(0, 0, Width, Height);
		glEnable(GL_DEPTH_TEST);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);



		

		float time = glfwGetTime();
		float lastTime = time;
		float deltaTime = 0.0;

		Transform transform;
		transform.position = glm::vec3(0.0f, 0.0f, 1.0f);

		MeshID cube = renderer.createMesh(createCubeMeshData());

		meshes.emplace_back(cube, transform);
		transform.position = glm::vec3(2.0f, 1.0f, 0.0f);
		transform.rotation = glm::qua(1.0f, 0.5f, 0.0f, 0.0f);
		meshes.emplace_back(cube, transform);
		//renderloop
		while (!glfwWindowShouldClose(window)) {

			lastTime = time;
			time = glfwGetTime();
			deltaTime = time - lastTime;
			processInput(window, deltaTime);

			renderer.beginFrame(camera, Width, Height);
			//rendering commands here

			for (SimulatedObject const object : meshes) {
				renderer.draw(
					object.getMesh(),
					modelMatrix(object.getTransform())
				);
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}
	

	glfwTerminate();
	return 0;
}