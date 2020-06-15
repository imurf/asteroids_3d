#include "mathutil.h"
#include "vector4f.h"

struct matrix44f *
view_look_at(struct vector4f eye_W, 
             struct vector4f at_W, 
             struct vector4f up_W,
             struct matrix44f *out)
{
  struct vector4f x; 
  struct vector4f y; 
  struct vector4f z; 

  /* compute local (+)z-axis */ 
  z = sub4fv(eye_W, at_W);
  z = scale4fv(z, -1.0f); /* camera looks down (-)z */
  z = normalise4fv(z);

  /* compute local (+)x-axis */
  x = cross4fv(up_W, z);
  x = normalise4fv(x);

  /* compute local (+)y-axis */
  y = cross4fv(z, x);

  *out = (struct matrix44f){
    {{x.x    , x.y    , x.z    , 0.0f},
     {y.x    , y.y    , y.z    , 0.0f},
     {z.x    , z.y    , z.z    , 0.0f},
     {eye_W.x, eye_W.y, eye_W.z, 1.0f}}
  };
  return out;
}
