#include <iostream>
#include "Vec.h"

int WIDTH = 1280;
int HEIGHT = 860;

// pixel buffer
// Each pixel is 3 floats: R, G, B (0.0 to 1.0)
std::vector<float> framebuffer(WIDTH * HEIGHT * 3, 0.0f);

void onResize(GLFWwindow*, int w, int h)
{
    WIDTH = w;
    HEIGHT = h;
    framebuffer.assign(WIDTH * HEIGHT * 3, 0.0f);
    glViewport(0, 0, WIDTH, HEIGHT);
}

void setPixel(float x, float y, float r, float g, float b)
{
    int index = (y * WIDTH + x) * 3;
    framebuffer[index + 0] = r;
    framebuffer[index + 1] = g;
    framebuffer[index + 2] = b;
}

void render(Vec3 &CameraPos, double &yaw,double &pitch, Vec3 &center);

int main()
{
    if (!glfwInit())
        return -1;
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Physics_eng - v0: Path Tracer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwGetWindowSize(window, &WIDTH, &HEIGHT);
    framebuffer.assign(WIDTH * HEIGHT * 3, 0.0f);
    glfwSetWindowSizeCallback(window, onResize);

    double lastMouseX = 0.0, lastMouseY = 0.0;
    bool firstClick = true;

    Vec3 cameraPos(0,4,-3);
    Vec3 velocity(0,0,0);
    Vec3 center = Vec3(0.0, 10.0, 2.0);
    int mass = 3;
    Vec3 sphereVelocity(0, 0, 0);

    double lastTime = glfwGetTime();
    double yaw = 0.0, pitch = 0.0;
    float sensitivity = 0.0009f;
    int nbFrames = 0;
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        double now = glfwGetTime();
        float deltaTime = (float)(now - lastTime);
        lastTime = now;
        glfwPollEvents();
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            if (firstClick)
            {
                lastMouseX = mouseX;
                lastMouseY = mouseY;
                firstClick = false;
            }
            double deltaX = mouseX - lastMouseX;
            double deltaY = mouseY - lastMouseY;
            yaw += deltaX * sensitivity;
            pitch += deltaY * sensitivity;
            
            // TODO: Allow the player to move around the sphere's with the mouse

        }
        else
        {
            firstClick = true;
        }

        
        float speed = 5.0f;
        Vec3 targetVelocity(0, 0, 0);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {targetVelocity.x += sin(yaw) * speed * deltaTime; targetVelocity.z += cos(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {targetVelocity.x -= sin(yaw) * speed * deltaTime; targetVelocity.z -= cos(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {targetVelocity.x += cos(yaw) * speed * deltaTime; targetVelocity.z += -sin(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {targetVelocity.x -= cos(yaw) * speed * deltaTime; targetVelocity.z -= -sin(yaw) * speed * deltaTime;}
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) targetVelocity.y -= speed*deltaTime;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) targetVelocity.y += speed*deltaTime;

        float smoothing = 1.0f;
        float t = 1.0f - std::exp(-smoothing * deltaTime);
        velocity.x += (targetVelocity.x - velocity.x) * t;
        velocity.y += (targetVelocity.y - velocity.y) * t;
        velocity.z += (targetVelocity.z - velocity.z) * t;

        cameraPos.x += velocity.x * deltaTime;
        cameraPos.y += velocity.y * deltaTime;
        cameraPos.z += velocity.z * deltaTime;

        if(cameraPos.y <= 0.1){
            cameraPos.y += abs(velocity.y * deltaTime);
        }

        sphereVelocity.y -= (9.8f * mass) * deltaTime;
        center.y += sphereVelocity.y * deltaTime;

        if(center.y <= 1.0f){
            center.y = 1.0f;
            sphereVelocity.y *= -0.6f;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        
        render(cameraPos,yaw,pitch,center);

        glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, framebuffer.data());

        glfwSwapBuffers(window);

    }

    glfwTerminate();
    return 0;
}

void render(Vec3 &cameraPos, double &yaw, double &pitch, Vec3 &center){

    // Initialize origin and point
    Vec3 forward(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch));
    Vec3 right = forward.cross(Vec3(0,1,0)).normalize();
    Vec3 up = right.cross(forward).normalize();

    float aspect = (float)WIDTH/HEIGHT;

    // Iterate through all pixels
    for (int j = 0; j < HEIGHT; j++)
        {
            for (int i = 0; i < WIDTH; i++)
            {
                float u = (2.0f * i / WIDTH  - 1.0f) * aspect;
                float v =  2.0f * j / HEIGHT - 1.0f;
                
                Vec3 dir = (forward + right*u + (up*v)).normalize();
                ray r(cameraPos, dir);
                float t = r.sphereint(1.0f, center);

                Vec3 material = Vec3(1.0f, 0.0f, 0.0f);
                Vec3 lightDir = Vec3(1.0f, 1.0f, -1.0f).normalize();

                // Use the rays off of the light to color the sphere
                Vec3 point = cameraPos + dir*t;
                Vec3 N = (point - center).normalize();

                // Cast Shadow
                Vec3 shadowOrigin = point + N*0.0001f; // Offset it to avoid self intersection
                ray shadowRay(shadowOrigin,lightDir);
                float shadowT = shadowRay.sphereint(1.0f,center);
                
                float intensity = (shadowT > 0) ? 0.0f : std::max(0.0f, N.dot(lightDir));
                Vec3 RGB = material * Vec3(intensity, intensity, intensity);

                // Only set the pixel if it hits the object
                if(t > 0){
                    setPixel(i, j, RGB.x, RGB.y, RGB.z);
                }
                else {

                    float skyT = 0.5f * (dir.y + 1.0f);
                    Vec3 sky = Vec3(1,1,1)*(1-skyT) + Vec3(0.5f,0.5f,1.0f)*skyT;
                    setPixel(i,j,sky.x,sky.y,sky.z);

                    if(r.Direction.y < 0){
                        float floorT = -cameraPos.y / r.Direction.y;
                        Vec3 floorHit = cameraPos + r.Direction * floorT;

                        if(floorHit.x <= 20 && floorHit.x >= -20 && floorHit.z <= 20 && floorHit.z >= -20){
                            // Shadow ray from floor towards the light:
                            ray shadowRay(floorHit + Vec3(0,0.001f,0), lightDir);
                            float shadowT = shadowRay.sphereint(1.0f, center);

                            float shadow = (shadowT > 0) ? 0.3f : 1.0f;

                            if(static_cast<int>(floor(floorHit.x) + floor(floorHit.z)) % 2 == 0){
                                setPixel(i,j, 0.7*shadow,0.7*shadow,0.7*shadow);
                            }
                            else{
                                setPixel(i,j, 0.3*shadow,0.3*shadow,0.3*shadow);
                            }
                        }
                    }
                }
            }   
        }
}