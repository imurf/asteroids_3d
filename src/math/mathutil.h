#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

#include "matrix44f.h"
#include "vector4f.h"

#define DEG_TO_RAD(angle) angle * 0.01745329252
#define RAD_TO_DEG(angle) angle * 57.2957795131

/* view_look_at - compute a view matrix.
 *
 * @eye_W - position of the view eye in world space.
 * @at_W - position of the point being looked at in view space.
 * @up_W - world space up vector.
 * @out - matrix to store the result.
 *
 * returns - matrix 'out'.
 */
struct matrix44f *
view_look_at(struct vector4f eye_W, 
             struct vector4f at_W, 
             struct vector4f up_W,
             struct matrix44f *out);


#endif
