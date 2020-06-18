/******************************************************************************
 *
 * module - matrix template module
 * author - Ian Murfin
 *
 *****************************************************************************/

#include <stdio.h>

#include "../vector4f.h"

#if (MATRIX_TYPE == float)
#define TYPE_SUFFIX f
#elif (MATRIX_TYPE == double)
#define TYPE_SUFFIX d
#endif

#ifndef TYPE_SUFFIX
#error "MATRIX_TYPE must be member of the set {float, double}\n"
#endif

#define MATNAMECAT0(name, cols, rows, type) name ## cols ## rows ## type
#define MATNAMECAT1(name, COLS, ROWS, TYPE) MATNAMECAT0(name, COLS, ROWS, TYPE)
#define MATRIX_NAME MATNAMECAT1(matrix, MATRIX_COLS, MATRIX_ROWS, TYPE_SUFFIX)

#define FUNNAMECAT0(name, cols, rows, type) name ## cols ## rows ## type ## m
#define FUNNAMECAT1(name, COLS, ROWS, TYPE) FUNNAMECAT0(name, COLS, ROWS, TYPE)
#define FUNCTION_NAME(name) FUNNAMECAT1(name, MATRIX_COLS, MATRIX_ROWS, TYPE_SUFFIX)

/* A column-major matrix to be used with column vectors, i.e elements are 
 * accessed:
 *   [col][row] == [vector][coordinate]
 *
 * thus for a 4x4 affine transformation matrix Mcp which transforms from
 * a parent coordinate space Pp to a child space Pc, where elements are 
 * written in the form:
 *
 *      [mathematical format]
 *
 *  | ic jc kc tc |   expands    | ix jx kx tx |
 *  | 0  0  0  1  |     to       | iy jy ky ty |
 *                               | iz jz kz tz |
 *                               | 0  0  0  1  |
 * where:
 *  ic = unit base vector of child space x-axis; expressed in parent space.
 *  jc = unit base vector of child space y-axis; expressed in parent space.
 *  kc = unit base vector of child space z-axis; expressed in parent space.
 *  tc = translaton of child space w.r.t parent space.
 *
 * this arrangement allows multiplication with column vectors, like:
 *
 *  | ix jx kx tx || x |   | x*ix + y*jx + z*kx + w*tx |
 *  | iy jy ky ty || y | = | x*iy + y*jy + z*ky + w*ty |
 *  | iz jz kz tz || z |   | x*iz + y*jz + z*kz + w*tz |
 *  | 0  0  0  1  || w |   | x*0  + y*0  + z*0  + w*1  |
 *
 * the in memory arrangement of the elements is:
 *
 *      [flattened array format]
 *
 *   m00 m01 m02 m03   m10 m11 m12 m13   m20 m21 m22 m23   m30 m31 m32 m33
 * | ix  iy  iz  0   | jx  jy  jz  0   | kx  ky  kz  0   | tx  ty  tz  0   |
 *
 * hence, when constructing a matrix struct with an initialiser list, it must
 * be initialised with the format:
 *
 *      [memory initialiser format]
 *
 * (struct MATRIX_NAME){
 *   {ix, iy, iz, 0}, -> 0rth column
 *   {jx, jy, jz, 0}, -> 1st column      [columns are read left-to-right]
 *   {kx, ky, kz, 0}, -> 2nd column
 *   {tx, ty, tz, 0}  -> 3rd column
 * };
 *
 * you can transpose this memory initialiser format to get the mathematical
 * format as defined above.
 *
 * This format is consistent with opengl 2.1, for example the opengl 2.1 
 * function 'glLoadMatrix' expects elements arranged in memory in the format:
 *
 *  | m0 m4 m8  m12 |
 *  | m1 m5 m9  m13 |
 *  | m2 m6 m10 m14 |
 *  | m3 m7 m11 m15 |
 *
 * i.e.
 *
 * | m0 m1 m2 m3 | m4 m5 m6 m7 | m8 m9 m10 m11 | m12 m13 m14 m15 |
 *
 * and
 *
 * (struct MATRIX_NAME){
 *   {m0 , m1 , m2 , m3 }, -> 0rth column
 *   {m4 , m5 , m6 , m7 }, -> 1st column   [columns are read left-to-right]
 *   {m8 , m9 , m10, m11}, -> 2nd column
 *   {m12, m13, m14, m15}  -> 3rd column
 * };
 *
 */
struct MATRIX_NAME
{
  MATRIX_TYPE m[MATRIX_COLS][MATRIX_ROWS];
};

/**** 4X4 MATRIX *************************************************************/

#if (MATRIX_COLS == 4) && (MATRIX_ROWS == 4)

/* zero - sets elements of matrix 'out' to 0.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(zero)(struct MATRIX_NAME *out);

/* identity - sets matrix 'out' to the identity matrix.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(identity)(struct MATRIX_NAME *out);

/* transpose - transposes matrix 'out'.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(transpose)(struct MATRIX_NAME *out);

/* concatenate - performs matrix product a*b (not b*a; matrix products do
 *   not commute). Result returned via matrix 'out'.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if any of a,b,out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(concatenate)(struct MATRIX_NAME *a,
                           struct MATRIX_NAME *b,
                           struct MATRIX_NAME *out);

/* multiple - matrix product of matrix 'a' with column vector 'v'.
 *
 * returns - resultant vector.
 *
 * errors - asserts(0) if a == NULL.
 */
struct vector4f
FUNCTION_NAME(multiply)(struct MATRIX_NAME *a, struct vector4f v);

/* flatten - 'flattens' the array to a 1D array of matrix elements, in
 *  column-major memory format, i.e.
 *
 * | column0 | column1 | column2 | column3 |
 *
 * where column0/1/2/3 are sets of 4 values.
 */
MATRIX_TYPE *
FUNCTION_NAME(flatten)(struct MATRIX_NAME *a);

/* fprint - print a string representation of the matrix to a file stream.
 */
static inline void
FUNCTION_NAME(fprint)(FILE *fstream, 
                      const char *name, 
                      struct MATRIX_NAME *a)
{
  fprintf(fstream, "%s=\n\
      \t|%f, %f, %f, %f|\n\
      \t|%f, %f, %f, %f|\n\
      \t|%f, %f, %f, %f|\n\
      \t|%f, %f, %f, %f|\n",
      name,
      a->m[0][0], a->m[1][0], a->m[2][0], a->m[3][0],
      a->m[0][1], a->m[1][1], a->m[2][1], a->m[3][1],
      a->m[0][2], a->m[1][2], a->m[2][2], a->m[3][2],
      a->m[0][3], a->m[1][3], a->m[2][3], a->m[3][3]);
}

/**** 4X4 MATRIX : TRANSFORM CONSTRUCTORS ************************************/

/* TODO: consider removing this function, seems pointless to have a routine
 * that performs a specific order of transformations unless this order is
 * used often and can be optimised. So far not using it.
 *
 * transformTRS - builds matrix 'out' into a general affine transformation 
 * matrix.
 *
 * @n_x, n_y, n_z - components of vector 'n' to rotate around (CCW rotation).
 * @angle_n_deg   - angle in degrees to rotate about vector 'n'.
 * @t_x, t_y, t_z - components of vector 't' to translate by.
 * @scale         - uniform scale factor to scale by.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 *
 * The constructed matrix is equivilent to the matrix concatenation of the
 * order:
 *    (T)(R)(S)
 * where:
 *    T = translation matrix; translates by vector {t_x, t_y, t_z}
 *    R = rotation matrix; rotates about direction {n_x, n_y, n_z}
 *    S = scale matrix; scales by factor 'scale'
 *
 * i.e. when used with column vectors, you get the product:
 *    (T)(R)(S)(v)
 * where:
 *    v = some column vector
 *
 * i.e. scale is applied first, then the rotation and then the translation.
 */
struct MATRIX_NAME *
FUNCTION_NAME(transformTRS)(MATRIX_TYPE n_x,
                            MATRIX_TYPE n_y,
                            MATRIX_TYPE n_z,
                            MATRIX_TYPE angle_n_deg,
                            MATRIX_TYPE t_x,
                            MATRIX_TYPE t_y,
                            MATRIX_TYPE t_z,
                            MATRIX_TYPE scale,
                            struct MATRIX_NAME *out);

/* rotation_x - build matrix 'out' into a pure rotation matrix which rotates
 *  about the x-axis.
 *
 * @angle_x_deg - angle about the x-axis to rotate.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(rotation_x)(MATRIX_TYPE angle_x_deg, struct MATRIX_NAME *out);

/* rotation_y - build matrix 'out' into a pure rotation matrix which rotates
 *  about the y-axis.
 *
 * @angle_y_deg - angle about the y-axis to rotate.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(rotation_y)(MATRIX_TYPE angle_y_deg, struct MATRIX_NAME *out);

/* rotation_z - build matrix 'out' into a pure rotation matrix which rotates
 *  about the z-axis.
 *
 * @angle_z_deg - angle about the z-axis to rotate.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(rotation_z)(MATRIX_TYPE angle_z_deg, struct MATRIX_NAME *out);

/* rotation_n - build matrix 'out' into a pure rotation matrix which rotates
 *  about the arbitrary vector 'n'.
 *
 * @angle_n_deg - angle about the 'n' vector to rotate.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(rotation_n)(MATRIX_TYPE angle_n_deg, 
                          MATRIX_TYPE n_x,
                          MATRIX_TYPE n_y,
                          MATRIX_TYPE n_z,
                          struct MATRIX_NAME *out);

/* translation - builds matrix 'out' into a pure translation matrix.
 *
 * @param x, y, x - components of vector to translate by.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(translation)(MATRIX_TYPE x, 
                           MATRIX_TYPE y, 
                           MATRIX_TYPE z,
                           struct MATRIX_NAME *out);

/* scale - builds matrix 'out' into a pure scale matrix.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(scale)(MATRIX_TYPE scale, struct MATRIX_NAME *out);

/* modelworld - builds matrix 'out' into a change of basis transformation
 *   matrix that transforms from a local 'model' space to the root 'world'
 *   space.
 *
 * @unit_x_W - the unit x-axis of the local space in world space coordinates.
 * @unit_y_W - the unit y-axis of the local space in world space coordinates.
 * @unit_z_W - the unit z-axis of the local space in world space coordinates.
 * @position_W - the position vector to the origin of the local space in
 *             world space coordinates.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 *
 * note - all unit basis vectors should be of unit length. If they are not
 *   all geometry in model space will be scaled in the respective axis by the 
 *   length of the axis' basis vector.
 */
struct MATRIX_NAME *
FUNCTION_NAME(modelworld)(struct vector4f unit_x_W,
                          struct vector4f unit_y_W,
                          struct vector4f unit_z_W,
                          struct vector4f position_W,
                          struct MATRIX_NAME *out);

/* modelworld_scale - equivilent to function 'modelworld' except it also 
 *   adds a uniform scale to the matrix; all model space geometry will be
 *   scaled by said uniform scale in world space.
 *
 * [see doc for 'modelworld' for more details on this function.]
 */
struct MATRIX_NAME *
FUNCTION_NAME(modelworld_scale)(struct vector4f unit_x_W,
                                struct vector4f unit_y_W,
                                struct vector4f unit_z_W,
                                struct vector4f position_W,
                                float scale,
                                struct MATRIX_NAME *out);

/* worldview - builds matrix 'out' into a change of basis matrix from the
 *   root 'world' space to the 'view' space of a camera.
 *
 * @unit_x_W - the unit x-axis of the view space in world space coordinates.
 * @unit_y_W - the unit y-axis of the view space in world space coordinates.
 * @unit_z_W - the unit z-axis of the view space in world space coordinates.
 * @position_W - the position vector to the origin of the view space in
 *             world space coordinates.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(worldview)(struct vector4f unit_x_W,
                         struct vector4f unit_y_W,
                         struct vector4f unit_z_W,
                         struct vector4f position_W,
                         struct MATRIX_NAME *out);

/* view_look_at - build matrix 'out' into a change of basis matrix from the
 *   root 'world' space to the 'view' space of a camera based on a target
 *   point in world space the camera is looking at.
 *
 * @eye_W - position vector to the eye of the camera (view space origin) in
 *          world space coordinates.
 * @at_W - position vector to the point the camera is looking at in world
 *         space coordinates.
 * @up_W - the world space up vector.
 *
 * returns - matrix 'out'.
 *
 * errors - asserts(0) if out == NULL.
 */
struct MATRIX_NAME *
FUNCTION_NAME(view_look_at)(struct vector4f eye_W,
                            struct vector4f at_W,
                            struct vector4f up_W,
                            struct MATRIX_NAME *out);

#endif

#ifndef __MATRIX_C_SOURCE
#undef MATNAMECAT0
#undef MATNAMECAT1
#undef MATRIX_NAME
#undef FUNNAMECAT0
#undef FUNNAMECAT1
#undef FUNCTION_NAME
#undef TYPE_SUFFIX
#undef MATRIX_ROWS
#undef MATRIX_COLS
#undef MATRIX_TYPE
#endif
