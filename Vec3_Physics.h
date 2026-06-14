#pragma once
#include <cmath>
#include "Sphere.h"
#include <vector>

// This class is not needed but might be helpful for future implementations, useful for organization as well.
class Physics{

    // Float for the gravity.
    float G = 9.8;

    float Ball_bounciness = -0.9f;

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
        // TODO: This is O(n^2), need to use Barnes-Hut Tree:
        for(int i = 0; i < length-1; i++){
            for(int j = i+1; j < length; j++){
                auto& Body1 = Spheres[i];
                auto& Body2 = Spheres[j];

                Vec3 diff = Body2.center - Body1.center;
                float dist = diff.length();
                if (dist < 0.001f) continue;

                // Force Magnitude:
                float Force_mag = G*Body1.mass * Body2.mass/(dist*dist);

                // Force componets (direction):
                Vec3 force = diff.normalize() * Force_mag;

                // Apply to Body1 (attract to Body2)
                Body1.velocity = Body1.velocity + force * (deltaTime / Body1.mass);
                Body2.velocity = Body2.velocity - force * (deltaTime / Body2.mass) ;
            }
        }
        for(auto& body : Spheres){
            body.center = body.center + body.velocity * deltaTime;

            if(body.center.y <= body.radius){
                body.center.y = body.radius;
                body.velocity.y *= Ball_bounciness;
            }
        }
    }
};