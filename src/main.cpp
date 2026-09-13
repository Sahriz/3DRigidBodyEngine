#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/Transform.hpp"
#include "demo/SimulatedObject.hpp"
#include "render/Camera.hpp"
#include "render/Geometry.hpp"
#include "render/Renderer.hpp"

#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include <vector>

#include "RigidBodyEngine/World.hpp"
#include "RigidBodyEngine/Body.hpp"



rbe::World world(glm::vec3(0.0f, -9.82f, 0.0f));

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
    int Width = 1600, Height = 1200;

    //init glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //create window
    GLFWwindow* window = glfwCreateWindow(Width, Height, "Physics Engine Demo", NULL, NULL);
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

        std::vector<SimulatedObject> SimedObjects;

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
        rbe::BodyID box  = world.Add(new rbe::Body());

        SimedObjects.emplace_back(cube, box, transform);
        transform.position = glm::vec3(2.0f, 1.0f, 0.0f);
        transform.rotation = glm::qua(1.0f, 0.5f, 0.0f, 0.0f);
        SimedObjects.emplace_back(cube, box, transform);
        //renderloop
        while (!glfwWindowShouldClose(window)) {

            lastTime = time;
            time = glfwGetTime();
            deltaTime = time - lastTime;
            processInput(window, deltaTime);
            world.Step(deltaTime);
            renderer.beginFrame(camera, Width, Height);
            //rendering commands here

            for (const SimulatedObject& object : SimedObjects) {
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
