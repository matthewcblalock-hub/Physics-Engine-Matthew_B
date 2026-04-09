#include <iostream>

// Holds all the Vec3 classes:
#include "Vec.h"

// Holds Sphere structs:
#include "Sphere.h"

// Initializing the window:

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

void render(Vec3 &CameraPos, double &yaw, double &pitch, Vec3 &center, int WIDTH, int HEIGHT);

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


    std::vector<Sphere> spheres = {
    {Vec3(0, 1, 2), 1.0f, Vec3(1, 0, 0)},
    {Vec3(3, 1, 0), 0.5f, Vec3(0, 1, 0)}
    };


    // Changing window size in real time:

    glfwMakeContextCurrent(window);
    glfwGetWindowSize(window, &WIDTH, &HEIGHT);
    framebuffer.assign(WIDTH * HEIGHT * 3, 0.0f);
    glfwSetWindowSizeCallback(window, onResize);

    // Main loop:

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: Add render function here later


        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, framebuffer.data());

        glfwSwapBuffers(window);
    }

    // End loop:

    glfwTerminate();
    return 0;
}