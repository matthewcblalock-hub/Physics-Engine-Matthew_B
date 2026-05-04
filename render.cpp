#include "Render_points.h"
#include "Camera.h"
#include "iostream"
#include "Sphere.h"

void setPixel(float x, float y, float r, float g, float b);

void render(Vec3 &CameraPos, double &yaw, double &pitch, Vec3 &center, std::vector<Sphere> &Spheres,int WIDTH, int HEIGHT)
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
            float ndcX = (2.0f * y_axis / WIDTH - 1.0f) * aspect;

            float ndcY = 2.0f * x_axis / HEIGHT - 1.0f;

            // Direction of the Ray from Camera and the actual Ray:

            Vec3 dir = (forward + right * ndcX + up * ndcY).normalize();

            ray New_Ray(CameraPos, dir);

            //TODO: Iterate through list of spheres and render each of them

            // Setting the first closest hit value to be the highest possible to ensure that the next one is contained within it:
            float closestHit = std::numeric_limits<float>::max();

            // Tracking what sphere is closest:
            const Sphere* hitSphere = nullptr;

            for(const Sphere &sphere : Spheres){

                float hitDist = New_Ray.sphereint(1.0f, center);

                if(hitDist > 0 && hitDist < closestHit){
                    closestHit = hitDist;
                    hitSphere = &sphere;
                }
            }
            //TODO: Make lightdir changeable while running
            Vec3 lightDir = Vec3(1.0f,1.0f,-1.0f);

            Vec3 point = CameraPos + dir*closestHit;  

            Vec3 SurfaceNorm = (point-center).normalize();

            Vec3 shadowOrigin = point + SurfaceNorm*0.0001f; // Offset to make sure it doesn't intersect itself.

            ray shadowRay(shadowOrigin, lightDir);

            float shadowT = shadowRay.sphereint(1.0f, center);

            float intensity = (shadowT > 0) ? 0.0f : std::max(0.0f, SurfaceNorm.dot(lightDir));

            //Pointing to the material of the closest sphere:
            Vec3 material = hitSphere->material;

            Vec3 RGB = material * Vec3(intensity, intensity, intensity);

            if(hitSphere){
                setPixel(WIDTH, HEIGHT,RGB.x,RGB.y,RGB.z);
            }
            else{
                float skyT = 0.5f * (dir.y + 1.0f);
                Vec3 sky = Vec3(1,1,1)*(1-skyT) + Vec3(0.5f,0.5f,1.0f)*skyT;
                setPixel(WIDTH,HEIGHT,sky.x,sky.y,sky.z);

                if(New_Ray.Direction.y < 0){
                        float floorT = -CameraPos.y / New_Ray.Direction.y;
                        Vec3 floorHit = CameraPos + New_Ray.Direction * floorT;

                        if(floorHit.x <= 20 && floorHit.x >= -20 && floorHit.z <= 20 && floorHit.z >= -20){
                            // Shadow ray from floor towards the light:
                            ray shadowRay(floorHit + Vec3(0,0.001f,0), lightDir);
                            float shadowT = shadowRay.sphereint(1.0f, center);

                            float shadow = (shadowT > 0) ? 0.3f : 1.0f;

                            if(static_cast<int>(floor(floorHit.x) + floor(floorHit.z)) % 2 == 0){
                                setPixel(WIDTH,HEIGHT, 0.7*shadow,0.7*shadow,0.7*shadow);
                            }
                            else{
                                setPixel(WIDTH,HEIGHT, 0.3*shadow,0.3*shadow,0.3*shadow);
                            }
                }
            }
        }
    }
}