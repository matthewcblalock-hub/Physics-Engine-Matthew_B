# Physics-Engine - Matthew Blalock

## Vector Math Reference

Length
|v| = √(x² + y² + z²)

Normalize
v̂ = v / |v|

Addition
a + b = (ax + bx, ay + by, az + bz)

Scalar multiply
tv = (tx, ty, tz)

Dot product
a · b = axbx + ayby + azbz

Cross product
a × b = (aybz − azby, azbx − axbz, axby − aybx)

Intersection point of a sphere
(ax + ay + az)^2 + (2axbx + 2ayby + 2azbz)t + (bx^2 + by^2 + bz^2)t^2 - r^2

Intersection point of a sphere (Dot product form)
(D·D)t² + 2(D·oc)t + (oc·oc - r²) = 0

MAIN 1:

Main 1 is the first version of the engine and is currently being cleaned up to go into the other files, MAIN 1 will be deleted.

The file of include where GLFW was included was not altered in anyway and is used to create the window of this project. Thanks to the creators of GLFW for making it possible

<img width="1246" height="643" alt="Screenshot 2026-05-16 at 7 39 56 PM" src="https://github.com/user-attachments/assets/d366b4eb-48af-4b6c-9722-ebcd0b987d8e" />

Here is the image of the second iteration of the ray tracer with multiple spheres being rendered, evantually the Möller-Trumbore algorithm will be implemented so that the ray tracer can render triangle meshes. Along with improvements in optimization and other expected updates like refelctions.
