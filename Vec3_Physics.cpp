#include <cmath>
#include "Sphere.h"
#include <vector>

class Physics{
    float G = 9.8;
    float Ball_bounciness = -0.6f;
    public:
    void update_velocity(std::vector<Sphere> &Spheres, float deltaTime){
        for(Sphere &sphere : Spheres){
            sphere.velocity.y -= (G * sphere.mass) * deltaTime;
            sphere.center.y += sphere.velocity.y * deltaTime;

            if(sphere.center.y <= sphere.radius){
                sphere.center.y = sphere.radius;
                sphere.velocity.y *= Ball_bounciness;
            };
    };
    }
    

};