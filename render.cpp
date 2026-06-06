#include "Camera.h"
#include "iostream"
#include "Sphere.h"
#include <thread>
#include <functional>

void setPixel(float x, float y, float r, float g, float b);

void renderRows(int startY, int endY,Vec3 CameraPos, Vec3 forward, Vec3 right, Vec3 up, float aspect, Vec3 lightDir, const std::vector<Sphere> &Spheres, int WIDTH, int HEIGHT);

void render(Vec3 &CameraPos, double &yaw, double &pitch, std::vector<Sphere> &Spheres,int WIDTH, int HEIGHT)
{
    // Making origin and point based off camera angle:
    Vec3 forward(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch));

    Vec3 right = forward.cross(Vec3(0,1,0)).normalize();

    Vec3 up = right.cross(forward).normalize();

    float aspect = (float)WIDTH/HEIGHT;

    Vec3 lightDir = Vec3(1.0f,1.0f,0.0f).normalize();

    int numThreads = std::thread::hardware_concurrency();
    int rowsPerThread = HEIGHT/numThreads;

    std::vector<std::thread> threads;

    for(int i = 0; i < numThreads; i++)
    {
        int startY = i * rowsPerThread;
        int endY = (i == numThreads - 1) ? HEIGHT : startY + rowsPerThread;

        threads.emplace_back(renderRows, startY, endY, CameraPos, forward, right, up, aspect, lightDir, std::reference_wrapper(Spheres), WIDTH, HEIGHT);
    
    }

    for (auto& t : threads)
    t.join();
}

void renderRows(int startY, int endY,Vec3 CameraPos, Vec3 forward, Vec3 right, Vec3 up, float aspect, Vec3 lightDir, const std::vector<Sphere> &Spheres, int WIDTH, int HEIGHT){
       // Iterating through pixels on the screen:

    for (int y_axis = startY; y_axis < endY; y_axis++)
    {
        for (int x_axis = 0; x_axis < WIDTH; x_axis++)
        {
            
            // ndc = Normalized Device Coordinates (Center of the screen)
            float ndcX = (2.0f * x_axis / WIDTH - 1.0f) * aspect;

            float ndcY = 2.0f * y_axis / HEIGHT - 1.0f;

            // Direction of the Ray from Camera and the actual Ray:

            Vec3 dir = (forward + right * ndcX + up * ndcY).normalize();

            ray New_Ray(CameraPos, dir);


            // Setting the first closest hit value to be the highest possible to ensure that the next one is contained within it:
            float closestHit = std::numeric_limits<float>::max();

            // Tracking what sphere is closest:
            const Sphere* hitSphere = nullptr;

            for(const Sphere &sphere : Spheres){

                float hitDist = New_Ray.sphereint(sphere.radius, sphere.center);

                if(hitDist > 0 && hitDist < closestHit){
                    closestHit = hitDist;
                    hitSphere = &sphere;
                }
            }
            //TODO: Make lightdir changeable while running
            
            
            if(hitSphere){

                Vec3 point = CameraPos + dir*closestHit;  

                Vec3 SurfaceNorm = (point-hitSphere->center).normalize();

                Vec3 shadowOrigin = point + SurfaceNorm * 0.0001f; // Offset to make sure it doesn't intersect itself.

                ray shadowRay(shadowOrigin, lightDir);

                bool inShadow = false;

                for(const Sphere &each_sphere : Spheres){
                    if(shadowRay.sphereint(each_sphere.radius, each_sphere.center) > 0){
                        inShadow = true;
                        break;
                    }
                }

                // TODO: add real path tracing with real bounces of light
                // This is faking light bounces: 
                float ambient = 0.1f;

                float diffuse = std::max(0.0f, SurfaceNorm.dot(lightDir));

                float intensity = inShadow ? ambient : ambient + (1.0f - ambient) * diffuse;

                //Pointing to the material of the closest sphere:
                Vec3 material = hitSphere->material;

                Vec3 RGB = material * Vec3(intensity, intensity, intensity);
                setPixel(x_axis, y_axis,RGB.x,RGB.y,RGB.z);
            }
            else{
                
                float skyT = 0.5f * (dir.y + 1.0f);
                Vec3 sky = Vec3(1,1,1)*(1-skyT) + Vec3(0.5f,0.5f,1.0f)*skyT;
                setPixel(x_axis,y_axis,sky.x,sky.y,sky.z);

                if(New_Ray.Direction.y < 0){

                        float floorT = -CameraPos.y / New_Ray.Direction.y;
                        Vec3 floorHit = CameraPos + New_Ray.Direction * floorT;

                        if(floorHit.x <= 20 && floorHit.x >= -20 && floorHit.z <= 20 && floorHit.z >= -20){
                            // Shadow ray from floor towards the light:
                            ray shadowRay(floorHit + Vec3(0,0.001f,0), lightDir);

                            bool inshadow = false;

                            for(const Sphere& sphere : Spheres){
                                if(shadowRay.sphereint(sphere.radius, sphere.center) > 0){inshadow = true; break;}
                            }

                            Vec3 floorNorm(0, 1, 0);
                            float floorDiffuse = std::max(0.0f, floorNorm.dot(lightDir));
                            float shadow = inshadow ? 0.2f : 0.2f + floorDiffuse;

                            if(static_cast<int>(floor(floorHit.x) + floor(floorHit.z)) % 2 == 0){
                                setPixel(x_axis,y_axis, 0.8*shadow,0.8*shadow,0.8*shadow);
                            }
                            else{
                                setPixel(x_axis,y_axis, 0.2*shadow,0.2*shadow,0.2*shadow);
                            }
                        }
                }
            }
        }
    }
}