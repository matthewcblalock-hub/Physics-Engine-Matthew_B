#include "Render_points.h"
#include "Camera.h"
#include "iostream"
#include "Sphere.h"

void render(Vec3 &CameraPos, double &yaw, double &pitch, Vec3 &center, std::vector<Sphere> &sphere,int WIDTH, int HEIGHT)
{
    // Making origin and point based off camera angle:
    Vec3 forward(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch));

    Vec3 right = forward.cross(ren_data.Shift_r).normalize();

    Vec3 up = right.cross(forward).normalize();

    float aspect = (float)WIDTH/HEIGHT;

    // Iterating through pixels on the screen:

    for (int y_axis = 0; y_axis < HEIGHT; y_axis++)
    {
        for (int x_axis = 0; x_axis < WIDTH; x_axis++)
        {
            
            // ndc = Normalized Device Coordinates (Center of the screen)
            float ndcY = (2.0f * y_axis / HEIGHT - 1.0f) * aspect;

            float ndcX = 2.0f * x_axis / WIDTH - 1.0f;

            // Direction of the Ray from Camera and the actual Ray:

            Vec3 dir = (forward + right * ndcX + up * ndcY).normalize();

            ray New_Ray(CameraPos, dir);

            float hitDist = New_Ray.sphereint(1.0f, center);
            //TODO: Iterate through list of spheres and render each of them
            // Try and keep it O LOG(N)
        }
    }
}