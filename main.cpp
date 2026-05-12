#include <iostream>

// Holds all the Vec3 classes:
#include "Vec.h"

// Allow Camera attributes to be included:
#include "Camera.h"

//Include file for all physics attributes along with the Sphere struct:
#include "Vec3_Physics.cpp"
Physics sphere_p;


int WIDTH = 1480;
int HEIGHT = 860;

// Pixel buffer:

std::vector<float> framebuffer(WIDTH * HEIGHT * 3, 0.0f);

// Resizing Window:

void onResize(GLFWwindow*, int w, int h)
{
    WIDTH = w;
    HEIGHT = h;
    framebuffer.assign(WIDTH * HEIGHT * 3, 0.0f);
    glViewport(0, 0, WIDTH, HEIGHT);
}

// Pixel Buffer: setting each pixel:

void setPixel(float x, float y, float red, float green, float blue)
{
    int index = (y * WIDTH + x) * 3;
    framebuffer[index + 0] = red;
    framebuffer[index + 1] = green;
    framebuffer[index + 2] = blue;
}

// Initialize render function in this file to be compiled:

void render(Vec3 &CameraPos, double &yaw, double &pitch, std::vector<Sphere> &Spheres,int WIDTH, int HEIGHT);

int main()
{
    // Making the window with the screen dimensions:

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Physics_eng - v1: Basic Mechanics and Ray Tracing", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    std::vector<Sphere> Spheres = {
    {Vec3(0, 10, 2), 1.0f, Vec3(1, 0, 0), Vec3(0,0,0), 1.0f},
    {Vec3(0, 5, -1.5), 0.5f, Vec3(0, 1, 0), Vec3(0,0,0), 1.0f},
    {Vec3(0, 6, 0), 0.75f, Vec3(0, 0, 1), Vec3(0,0,0), 1.0f}
    };

    // Changing window size in real time:

    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &WIDTH, &HEIGHT);
    framebuffer.assign(WIDTH * HEIGHT * 3, 0.0f);
    glfwSetWindowSizeCallback(window, onResize);

    camera cam;

    float lastTime = glfwGetTime(); float deltaTime = 0;

    // Main loop:
    
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapInterval(1);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        deltaTime = glfwGetTime() - lastTime; lastTime = glfwGetTime();
        cam.update(window, deltaTime);

        sphere_p.update_velocity(Spheres, deltaTime);

        render(cam.cameraPos, cam.yaw, cam.pitch, Spheres, WIDTH, HEIGHT);

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, framebuffer.data());

        glfwSwapBuffers(window);
    }

    // End loop:

    glfwTerminate();
    return 0;
}