#include "Vec.h"
struct Ren_Data{
    Vec3 Shift_r = Vec3(0,1,0);
};

inline Ren_Data ren_data; // Makes this a Global instance to be used anywhere