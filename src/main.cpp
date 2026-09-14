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

        MeshID cube1 = renderer.createMesh(createCubeMeshData());
        MeshID cube2 = renderer.createMesh(createCubeMeshData());
        rbe::BodyID box1  = world.Add(new rbe::Body(glm::vec3(1.5f), glm::angleAxis(45.0f, glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(1.0f), 1.0f));
        rbe::BodyID box2 = world.Add(new rbe::Body(glm::vec3(0.0f), 1.0f));

        SimedObjects.emplace_back(cube1, box1, transform);
        SimedObjects.emplace_back(cube2, box2, transform);
        //renderloop
        while (!glfwWindowShouldClose(window)) {

            lastTime = time;
            time = glfwGetTime();
            deltaTime = time - lastTime;
            processInput(window, deltaTime);
            world.Step(deltaTime);
            renderer.beginFrame(camera, Width, Height);
            //rendering commands here

            for (SimulatedObject& object : SimedObjects) {
				rbe::BodyID body = object.getBody();
				rbe::Body* bodyPtr = world.getBody(body);
                if(bodyPtr) {
                    Transform newTransform;
					newTransform.position   = bodyPtr->position;
					newTransform.rotation   = bodyPtr->rotation;
					newTransform.scale      = bodyPtr->scale;
                    object.setTransform(newTransform);
				}
                else {
					std::cout << "Body not found in world!" << std::endl;
                    continue;
                }
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
