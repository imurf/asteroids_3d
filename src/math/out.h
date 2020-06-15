# 1 "mathutil.h"
# 1 "<built-in>"
# 1 "<command-line>"
# 31 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 32 "<command-line>" 2
# 1 "mathutil.h"



# 1 "matrix44f.h" 1







# 1 "templates/__matrix.h" 1







# 1 "templates/../vector4f.h" 1






# 1 "templates/../templates/__vector.h" 1
# 90 "templates/../templates/__vector.h"
struct vector4f
{
  float x;
  float y;


  float z;



  float w;

};
# 111 "templates/../templates/__vector.h"
struct vector4f
add4fv(struct vector4f a, struct vector4f b);
# 122 "templates/../templates/__vector.h"
struct vector4f
sub4fv(struct vector4f a, struct vector4f b);





struct vector4f
scale4fv(struct vector4f a, float scale);





double
dot4fv(struct vector4f a, struct vector4f b);
# 159 "templates/../templates/__vector.h"
struct vector4f
cross4fv(struct vector4f a, struct vector4f b);







struct vector4f
hadamard4fv(struct vector4f a, struct vector4f b);





double
length4fv(struct vector4f a);





double
length_squared4fv(struct vector4f a);





struct vector4f
normalise4fv(struct vector4f a);
# 211 "templates/../templates/__vector.h"
struct spherical4f
{
  float r;
  float theta;
  float phi;


  float w;


};
# 242 "templates/../templates/__vector.h"
struct vector4f
spherical_to_cartesian4fv(struct spherical4f a);
# 8 "templates/../vector4f.h" 2
# 9 "templates/__matrix.h" 2
# 99 "templates/__matrix.h"
struct matrix44f
{
  float m[4][4];
};
# 114 "templates/__matrix.h"
struct matrix44f *
zero44fm(struct matrix44f *out);







struct matrix44f *
identity44fm(struct matrix44f *out);







struct matrix_name *
transpose44fm(struct matrix44f *out);
# 146 "templates/__matrix.h"
struct matrix44f *
transform44fm(float n_x,
                         float n_y,
                         float n_z,
                         float angle_n_deg,
                         float t_x,
                         float t_y,
                         float t_z,
                         float scale,
                         struct matrix44f *out);
# 166 "templates/__matrix.h"
struct matrix44f *
rotation_x44fm(float angle_x_deg, struct matrix44f *out);
# 178 "templates/__matrix.h"
struct matrix44f *
rotation_y44fm(float angle_y_deg, struct matrix44f *out);
# 190 "templates/__matrix.h"
struct matrix44f *
rotation_z44fm(float angle_z_deg, struct matrix44f *out);
# 202 "templates/__matrix.h"
struct matrix44f *
rotation_n44fm(float angle_n_deg,
                          float n_x,
                          float n_y,
                          float n_z,
                          struct matrix44f *out);
# 217 "templates/__matrix.h"
struct matrix44f *
translation44fm(float x,
                           float y,
                           float z,
                           struct matrix44f *out);







struct matrix44f *
scale44fm(float scale, struct matrix44f *out);
# 239 "templates/__matrix.h"
struct matrix44f *
concatenate44fm(struct matrix44f *a,
                           struct matrix44f *b,
                           struct matrix44f *out);







struct vector4f
multiply44fm(struct matrix44f *a, struct vector4f v);
# 260 "templates/__matrix.h"
float *
flatten44fm(struct matrix44f *a);
# 9 "matrix44f.h" 2
# 5 "mathutil.h" 2
# 1 "vector4f.h" 1
# 6 "mathutil.h" 2
# 19 "mathutil.h"
struct matrix44f *
view_look_at(struct vector4f eye_W,
             struct vector4f at_W,
             struct vector4f up_W,
             struct matrix44f *out);
