
#define __MATRIX_C_SOURCE

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "../vector4f.h"

#include "__matrix.h"

/* including mathutil.h causes circular includes which creates problems, thus
 * duplicate the defines */
#define DEG_TO_RAD(angle) angle * 0.01745329252
#define RAD_TO_DEG(angle) angle * 57.2957795131

#define M0 (MATRIX_TYPE)0
#define M1 (MATRIX_TYPE)1

/* note: these are column-major matrices, but in code they look like
 * they are row-major. Remember the format of 2D arrays is:
 *
 * | inner-array-0 | inner-array-1 | inner-array-2 | inner-array-3 |
 *
 * where:
 *  inner-array-n = the nth array of the 2nd dimension of the 2D array.
 *
 * For column major matrices each inner-array is a column, not a row, hence
 * when written in code you should read the arrays as a transposed column-major
 * array.
 *
 *  {{column-0},
 *   {column-1},    i.e. columns go left-to-right not up-to-down
 *   {column-2},
 *   {column-3}}
 */

/**** 4X4 MATRIX *************************************************************/

#if (MATRIX_COLS == 4) && (MATRIX_ROWS == 4)

struct MATRIX_NAME *
FUNCTION_NAME(zero)(struct MATRIX_NAME *out)
{
  assert(out != NULL);
  assert(out != NULL);
  *out = (struct MATRIX_NAME){
    {{M0, M0, M0, M0},
     {M0, M0, M0, M0},
     {M0, M0, M0, M0},
     {M0, M0, M0, M0}}
  };
  return out;
}

struct MATRIX_NAME *
FUNCTION_NAME(identity)(struct MATRIX_NAME *out)
{
  assert(out != NULL);
  *out = (struct MATRIX_NAME){
    {{M1, M0, M0, M0},
     {M0, M1, M0, M0},
     {M0, M0, M1, M0},
     {M0, M0, M0, M1}}
  };
  return out;
}

struct MATRIX_NAME *
FUNCTION_NAME(transpose)(struct MATRIX_NAME *out)
{
  MATRIX_TYPE tmp;
  for(int col = 0; col <= 3; ++col)
  {
    for(int row = col + 1; row <= 3; ++row)
    {
      tmp = out->m[col][row];
      out->m[col][row] = out->m[row][col];
      out->m[row][col] = tmp;
    }
  }
  return out;
}

struct MATRIX_NAME *
FUNCTION_NAME(transform)(MATRIX_TYPE n_x,
                         MATRIX_TYPE n_y,
                         MATRIX_TYPE n_z,
                         MATRIX_TYPE angle_n_deg,
                         MATRIX_TYPE t_x,
                         MATRIX_TYPE t_y,
                         MATRIX_TYPE t_z,
                         MATRIX_TYPE scale,
                         struct MATRIX_NAME *out)
{
  assert(out != NULL);
  angle_n_deg = DEG_TO_RAD(angle_n_deg);
  MATRIX_TYPE c = cos(angle_n_deg), 
              s = sin(angle_n_deg),
              a = (MATRIX_TYPE)1 - c,
              xy = n_x * n_y,
              xz = n_x * n_z,
              yz = n_y * n_z,
              x2 = n_x * n_x,
              y2 = n_y * n_y,
              z2 = n_z * n_z,
              sx = s * n_x,
              sy = s * n_y,
              sz = s * n_z;
#define sf scale
  *out = (struct MATRIX_NAME){
    //{{(sf*(c + a*x2 )), (sf*(a*xy - sz)), (sf*(a*xz + sy)), (t_x)},
    // {(sf*(a*xy + sz)), (sf*(c + a*y2 )), (sf*(a*yz - sx)), (t_y)},
    // {(sf*(a*xz - sy)), (sf*(a*yz + sx)), (sf*(c + a*z2 )), (t_z)},
    // {(M0            ), (M0            ), (M0            ), (M1)}}
    {{(sf*(c + a*x2 )), (sf*(a*xy + sz)), (sf*(a*xz - sy)), (M0)},
     {(sf*(a*xy - sz)), (sf*(c + a*y2 )), (sf*(a*yz + sx)), (M0)},
     {(sf*(a*xz + sy)), (sf*(a*yz - sx)), (sf*(c + a*z2 )), (M0)},
     {(t_x           ), (t_y           ), (t_z           ), (M1)}}
  };
  return out;
#undef sf
}

struct MATRIX_NAME *
FUNCTION_NAME(rotation_x)(MATRIX_TYPE angle_x_deg, struct MATRIX_NAME *out)
{
  assert(out != NULL);

#define angle_x_rad angle_x_deg

  angle_x_rad = DEG_TO_RAD(angle_x_deg);
  MATRIX_TYPE c = cos(angle_x_rad), 
              s = sin(angle_x_rad);
  *out = (struct MATRIX_NAME){
    //{{M1, M0, M0, M0},
    // {M0, c , -s, M0},
    // {M0, s , c , M0},
    // {M0, M0, M0, M1}}
    {{M1, M0, M0, M0},
     {M0, c , s , M0},
     {M0, -s, c , M0},
     {M0, M0, M0, M1}}
  };
  return out;

#undef angle_x_rad
}

struct MATRIX_NAME *
FUNCTION_NAME(rotation_y)(MATRIX_TYPE angle_y_deg, struct MATRIX_NAME *out)
{
  assert(out != NULL);

#define angle_y_rad angle_y_deg

  angle_y_rad = DEG_TO_RAD(angle_y_deg);
  MATRIX_TYPE c = cos(angle_y_rad), 
              s = sin(angle_y_rad);
  *out = (struct MATRIX_NAME){
    //{{c , M0, s , M0},
    // {M0, M1, M0, M0},
    // {-s, M0, c , M0},
    // {M0, M0, M0, M1}}
    {{c , M0, -s, M0},
     {M0, M1, M0, M0},
     {s , M0, c , M0},
     {M0, M0, M0, M1}}
  };
  return out;

#undef angle_y_rad
}

struct MATRIX_NAME *
FUNCTION_NAME(rotation_z)(MATRIX_TYPE angle_z_deg, struct MATRIX_NAME *out)
{
  assert(out != NULL);

#define angle_z_rad angle_z_deg

  angle_z_rad = DEG_TO_RAD(angle_z_deg);
  MATRIX_TYPE c = cos(angle_z_rad), 
              s = sin(angle_z_rad);
  *out = (struct MATRIX_NAME){
    //{{c , -s, M0, M0},
    // {s , c , M0, M0},
    // {M0, M0, M1, M0},
    // {M0, M0, M0, M1}}
    {{c , s , M0, M0},
     {-s, c , M0, M0},
     {M0, M0, M1, M0},
     {M0, M0, M0, M1}}
  };
  return out;

#undef angle_z_rad
}

struct MATRIX_NAME *
FUNCTION_NAME(rotation_n)(MATRIX_TYPE angle_n_deg, 
                          MATRIX_TYPE n_x,
                          MATRIX_TYPE n_y,
                          MATRIX_TYPE n_z,
                          struct MATRIX_NAME *out)
{
  assert(out != NULL);
  
#define angle_n_rad angle_n_deg

  angle_n_rad = DEG_TO_RAD(angle_n_deg);
  MATRIX_TYPE c = cos(angle_n_rad), 
              s = sin(angle_n_rad),
              a = (MATRIX_TYPE)1 - c,
              xy = n_x * n_y,
              xz = n_x * n_z,
              yz = n_y * n_z,
              x2 = n_x * n_x,
              y2 = n_y * n_y,
              z2 = n_z * n_z,
              sx = s * n_x,
              sy = s * n_y,
              sz = s * n_z;
  *out = (struct MATRIX_NAME){
    //{{(c + a*x2 ), (a*xy - sz), (a*xz + sy), (M0)},
    // {(a*xy + sz), (c + a*y2 ), (a*yz - sx), (M0)},
    // {(a*xz - sy), (a*yz + sx), (c + a*z2 ), (M0)},
    // {(M0       ) ,(M0       ), (M0       ), (M1)}}
    {{(c + a*x2 ), (a*xy + sz), (a*xz - sy), (M0)},
     {(a*xy - sz), (c + a*y2 ), (a*yz + sx), (M0)},
     {(a*xz + sy), (a*yz - sx), (c + a*z2 ), (M0)},
     {(M0       ) ,(M0       ), (M0       ), (M1)}}
  };
  return out;

#undef angle_n_rad
}

struct MATRIX_NAME *
FUNCTION_NAME(translation)(MATRIX_TYPE x, 
                           MATRIX_TYPE y, 
                           MATRIX_TYPE z,
                           struct MATRIX_NAME *out)
{
  assert(out != NULL);
  *out = (struct MATRIX_NAME){
    {{M1, M0, M0, M0},
     {M0, M1, M0, M0},
     {M0, M0, M1, M0},
     {x , y , z , M1}}
  };
  return out;
}

struct MATRIX_NAME *
FUNCTION_NAME(scale)(MATRIX_TYPE scale, struct MATRIX_NAME *out)
{
  assert(out != NULL);
  *out = (struct MATRIX_NAME){
    {{scale, M0   , M0   , M0},
     {M0   , scale, M0   , M0},
     {M0   , M0   , scale, M0},
     {M0   , M0   , M0   , M1}}
  };
  return out;
}

struct MATRIX_NAME *
FUNCTION_NAME(concatenate)(struct MATRIX_NAME *a,
                           struct MATRIX_NAME *b,
                           struct MATRIX_NAME *out)
{
  assert(a != NULL && b != NULL && out != NULL);
  memset((void *)out, M0, sizeof(struct MATRIX_NAME));
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j)
      for(int n = 0; n < 3; ++n)
        out->m[i][j] += a->m[n][j] * b->m[i][n];
}

struct vector4f
FUNCTION_NAME(multiply)(struct MATRIX_NAME *a, struct vector4f v)
{
  assert(a != NULL);
  return (struct vector4f){
    (a->m[0][0]*v.x + a->m[1][0]*v.y + a->m[2][0]*v.z + a->m[3][0]*v.w),
    (a->m[0][1]*v.x + a->m[1][1]*v.y + a->m[2][1]*v.z + a->m[3][1]*v.w),
    (a->m[0][2]*v.x + a->m[1][2]*v.y + a->m[2][2]*v.z + a->m[3][2]*v.w),
    (a->m[0][3]*v.x + a->m[1][3]*v.y + a->m[2][3]*v.z + a->m[3][3]*v.w)
  };
}

MATRIX_TYPE *
FUNCTION_NAME(flatten)(struct MATRIX_NAME *a)
{
  return &(a->m[0][0]);
}

#endif

