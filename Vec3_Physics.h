#include <cmath>
#include "Sphere.h"
#include <vector>

class Physics{

    // Float for the gravity.
    float G = 9.8;

    float Ball_bounciness = -0.6f;

    public:

    void update_velocity_sphere(std::vector<Sphere> &Spheres, float deltaTime){

        for(Sphere &sphere : Spheres){

            sphere.velocity.y -= (G * sphere.mass) * deltaTime;

            sphere.center.y += sphere.velocity.y * deltaTime;

            if(sphere.center.y <= sphere.radius){

                sphere.center.y = sphere.radius;
                
                sphere.velocity.y *= Ball_bounciness;
            }
        }
    }
    // TODO: Change the calculating of velocity from Eulers method to RK4 to allow more accurate calculations and to allow 
    void orbital_mechanics(std::vector<Sphere> &Spheres, float deltaTime){
        // Get the size of the Vector
        int length = Spheres.size();

        for(int i = 0; i < length-1; i++){
            for(int j = i+1; j < length; j++){
                auto& Body1 = Spheres[i];
                auto& Body2 = Spheres[j];

                float dx = Body2.center.x - Body1.center.x;
                float dy = Body2.center.y - Body1.center.y;
                float dz = Body2.center.z - Body1.center.z;
                float hypot = sqrt(dx*dx + dy*dy + dz*dz);
                if (hypot < 0.001f) continue;

                // Force Magnitude:
                float Force_mag = G*Body1.mass * Body2.mass/(hypot*hypot);

                // Force componets (direction):
                float fx = (dx/hypot) * Force_mag;
                float fy = (dy/hypot) * Force_mag;
                float fz = (dz/hypot) * Force_mag;

                // Apply to Body1 (attract to Body2)
                Body1.velocity.x += (fx/Body1.mass)*deltaTime;
                Body1.velocity.y += (fy/Body1.mass)*deltaTime;
                Body1.velocity.z += (fz/Body1.mass)*deltaTime;

                Body2.velocity.x -= (fx/Body2.mass)*deltaTime;
                Body2.velocity.y -= (fy/Body2.mass)*deltaTime;
                Body2.velocity.z -= (fz/Body2.mass)*deltaTime;
            }
        }
        for(auto& body : Spheres){
                    body.center.x += body.velocity.x * deltaTime;
                    body.center.y += body.velocity.y * deltaTime;
                    body.center.z += body.velocity.z * deltaTime;
        }
    }
};