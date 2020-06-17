
#include "math/mathutil.h"
#include "ship.h"
#include "ship_cam.h"

#include <stdio.h>
#include <math.h>

/* should be updated after the ship */
void
ship_cam_update(struct ship_cam *cam, float delta_s)
{
  /* compute the eye position w.r.t world space */
  struct vector4f eye_W_m;
  struct vector4f translation = {0.f, 0.f, cam->seperation_m, 0.f};
  translation = multiply44fm(&(cam->target->mw), translation);
  //eye_W_m = add4fv(cam->target->pos_W_m, translation); 
  
  eye_W_m = multiply44fm(&(cam->target->mw), (struct vector4f){0.f, 0.f, cam->seperation_m, 1.f});

  /* compute the local axes of the camera */
  struct vector4f x = multiply44fm(&(cam->target->mw), 
                                   (struct vector4f){1.f, 0.f, 0.f, 0.f});
  struct vector4f y = multiply44fm(&(cam->target->mw), 
                                   (struct vector4f){0.f, 1.f, 0.f, 0.f});
  struct vector4f z = multiply44fm(&(cam->target->mw), 
                                   (struct vector4f){0.f, 0.f, 1.f, 0.f});

  //printf("y_basis={%f,%f,%f}\n", y.x, y.y, y.z);
  //printf("z_basis={%f,%f,%f}\n", z.x, z.y, z.z);
  //printf("t_dir  ={%f,%f,%f}\n", translation.x, translation.y, translation.z);
  //float dot = dot4fv(z, translation);
  //float tmp = dot / (length4fv(z) * length4fv(translation));
  //float theta = acos(tmp);
  //printf("angle_between(z_basis, t_dir)=%f\n", RAD_TO_DEG(theta));
  //
  //printf("ship pitch = %f\n", cam->target->pitch_deg);
  //printf("ship roll = %f\n", cam->target->roll_deg);
  //printf("ship pos_W={%f, %f, %f}\n", cam->target->pos_W_m.x, cam->target->pos_W_m.y, cam->target->pos_W_m.z); 
  //printf("eye pos_W={%f, %f, %f}\n", eye_W_m.x, eye_W_m.y, eye_W_m.z); 

  /* this code is translating my camera to the point in world space
   * 'seperation' distance behind the ship, and then rotating about this
   * world space origin, so it is translating my position vector from
   * the local ship space into world space and then performing the rotation,
   * thus rotating about the world space origin. 
   *
   * I want to rotate first and then translate I think */
  ///float tx, ty, tz;
  ///tx = x.x*eye_W_m.x + y.x*eye_W_m.y + z.x*eye_W_m.z; 
  ///ty = x.y*eye_W_m.x + y.y*eye_W_m.y + z.y*eye_W_m.z; 
  ///tz = x.x*eye_W_m.z + y.z*eye_W_m.y + z.z*eye_W_m.z; 
  struct matrix44f rm = (struct matrix44f){
    //{{x.x, x.y, x.z, 0.f},
    // {y.x, y.y, y.z, 0.f},
    // {z.x, z.y, z.z, 0.f},
    {{x.x, y.x, z.x, 0.f},
     {x.y, y.y, z.y, 0.f},
     {x.z, y.z, z.z, 0.f},
     {0.f, 0.f, 0.f, 1.f}}
     //{-eye_W_m.x, -eye_W_m.y, -eye_W_m.z, 1.f}}
    //{{x.x, y.x, z.x, 0.f},
    // {x.y, y.y, z.y, 0.f},
    // {x.z, y.z, z.z, 0.f},
    // {-tx, -ty, -tz, 1.f}}
  };

  struct matrix44f tm;
  translation44fm(-eye_W_m.x, -eye_W_m.y, -eye_W_m.z, &tm);

  concatenate44fm(&rm, &tm, &(cam->view));

  //printf("view=\n\
  //    |%f, %f, %f, %f|\n\
  //    |%f, %f, %f, %f|\n\
  //    |%f, %f, %f, %f|\n\
  //    |%f, %f, %f, %f|\n",
  //    cam->view.m[0][0], cam->view.m[1][0], cam->view.m[2][0], cam->view.m[3][0],
  //    cam->view.m[0][1], cam->view.m[1][1], cam->view.m[2][1], cam->view.m[3][1],
  //    cam->view.m[0][2], cam->view.m[1][2], cam->view.m[2][2], cam->view.m[3][2],
  //    cam->view.m[0][3], cam->view.m[1][3], cam->view.m[2][3], cam->view.m[3][3]);


  /* copy the ships orientation in space */
  //struct matrix44f r, p;
  //rotation_x44fm(cam->target->pitch_deg, &p);
  //rotation_z44fm(cam->target->roll_deg, &r);
  //concatenate44fm(&r, &p, &(cam->view));

  ///* set the ships translation */
  //cam->view.m[3][0] = eye_W_m.x;
  //cam->view.m[3][1] = eye_W_m.y;
  //cam->view.m[3][2] = eye_W_m.z;

  /* compute the view matrix */
  //view_look_at(eye_W_m, 
  //             cam->target->pos_W_m, 
  //             (struct vector4f){0.f, 1.f, 0.f, 0.f}, 
  //             &(cam->view)); 
}
