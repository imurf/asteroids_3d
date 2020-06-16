#include "mathutil.h"
#include "vector4f.h"

struct matrix44f *
view_look_at(struct vector4f eye_W, 
             struct vector4f at_W, 
             struct vector4f up_W,
             struct matrix44f *out)
{
  /* note - the view matrix transforms the world geometry into view space, 
   * thus it effectively moves the world, rather than moving the camera.
   *
   * Hence a translation of, say, +20 units up the +z-axis, actually moves the
   * world +20 units up the +z-axis, essentially moving the camera, -20 units
   * down the -z-axis.
   *
   * The same applies to rotations, in that the rotations apply to the world, 
   * not the camera.
   *
   * The interface for this function is conceptually moving the camera, i.e.
   * the eye_W vector is supposed to be a vector to the position of the camera
   * in world space. Thus the translation to place the camera at the expected 
   * position will actually be the inverse translation.
   */

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
     {-eye_W.x, -eye_W.y, -eye_W.z, 1.0f}}
  };
  return out;
}
