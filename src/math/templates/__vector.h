/******************************************************************************
 *
 * DO NOT INCLUDE THIS HEADER! THIS IS A HEADER TEMPLATE USED TO GENERATE
 * HEADER FILES FOR MATH VECTOR TYPE SPECIFICATIONS.
 *
 * GENERATE A TYPE SPECIFICATION HEADER AND INCLUDE THAT INSTEAD.
 *
 * module - math vector template
 * author - Ian Murfin
 *
 * usage:
 *
 * To generate a new vector type from this template:
 *
 * 1 - create a new header file for the type.
 * 2 - define the template arguments 'VECTOR_SIZE' and 'VECTOR_TYPE'.
 * 3 - include this template header file.
 *
 * To complete the new type you must also repeat the above steps for the 
 * source template file '__vector.c'; replace 'header' for 'source' in the 
 * above steps.
 *
 * arguments:
 *
 * VECTOR_SIZE - dimensional size of the vector: 2, 3 or 4.
 * VECTOR_TYPE - data type of the vector's components: float, double or int.
 *
 *****************************************************************************/

#ifndef VECTOR_SIZE
#error "VECTOR_SIZE template argument undefined\n"
#endif
#ifndef VECTOR_TYPE
#error "VECTOR_TYPE template argument undefined\n"
#endif

#if(VECTOR_SIZE < 2)
#error "VECTOR_SIZE < 2 is not a vector\n"
#endif
#if(VECTOR_SIZE > 4)
#error "VECTOR_SIZE > 4 not supported; must be member of set {2,3,4}\n"
#endif

#if(VECTOR_TYPE == float) 
#define TYPE_SUFFIX f
#elif(VECTOR_TYPE == double)
#define TYPE_SUFFIX d
#elif(VECTOR_TYPE == int)
#define TYPE_SUFFIX i
#endif

#ifndef TYPE_SUFFIX
#error "VECTOR_TYPE must be member of set {float,double,int}\n"
#endif

/**** CARTESIAN VECTOR *******************************************************/

#define VECNAMECAT0(name, size, type) name ## size ## type
#define VECNAMECAT1(name, SIZE, TYPE) VECNAMECAT0(name, SIZE, TYPE)
#define VECTOR_NAME VECNAMECAT1(vector, VECTOR_SIZE, TYPE_SUFFIX) 

#define FUNNAMECAT0(name, size, type) name ## size ## type ## v
#define FUNNAMECAT1(name, SIZE, TYPE) FUNNAMECAT0(name, SIZE, TYPE)
#define FUNCTION_NAME(name) FUNNAMECAT1(name, VECTOR_SIZE, TYPE_SUFFIX) 

/* A column vector in cartesian coordinates.
 *
 * points vs directions:
 *
 * If VECTOR_SIZE == 4, the vector is a homogenous vector. This format allows
 * for the distinction between points and directions; w=1 for points, whereas
 * w=0 for directions. 
 *
 * When initialising a 4D vector the w component matters as some functions
 * within the math library will operate on the vector differently if it is a
 * point or a direction, notably matrix transformations. A transformation 
 * matrix's translation component is ignored for direction vectors since you 
 * cannot translate a direction, only points can be translated.
 * 
 * reminder: dividing a point in 4D homogenous coordinates through by the w 
 * (spatial scale) component yields a euclidean 3D point. If w=0, the point
 * has 0 scale, such a point is said to be a point at infinity. Since a point
 * at infinity cannot be translated (all translations would yield infinity, 
 * thus the points position remains unchanged), such points behave like
 * directions; they can only be rotated. This makes sense, since projective
 * geometry shows that there is only one point at infinity for a given
 * direction, thus the direction and the point at infinity can be considered
 * equivilent.
 */
struct VECTOR_NAME
{
  VECTOR_TYPE x;
  VECTOR_TYPE y;
 
#if(VECTOR_SIZE >= 3)
  VECTOR_TYPE z;
#endif

#if(VECTOR_SIZE == 4)
  VECTOR_TYPE w;
#endif
};

/* add - vector addition: a + b
 *
 * note - in 4D case, returned vector's w = a.w + b.w, thus: 
 *    direction + direction = direction
 *    point + direction = point
 *    point + point = error -> will assert(0)
 */
struct VECTOR_NAME
FUNCTION_NAME(add)(struct VECTOR_NAME a, struct VECTOR_NAME b);

/* sub - vector subtraction: b - a
 *
 * note - in 4D case, returned vector's w = a.w - b.w, thus: 
 *    direction - direction = direction
 *    point - point = direction
 *    point - direction = point
 *    direction - point = error -> will assert(0)
 */
struct VECTOR_NAME
FUNCTION_NAME(sub)(struct VECTOR_NAME a, struct VECTOR_NAME b);

/* scale - uniform vector scaling: a * scale
 *
 * note - in 4D case returned vector's w = a.w
 */
struct VECTOR_NAME
FUNCTION_NAME(scale)(struct VECTOR_NAME a, float scale);

/* dot - vector dot product: a . b
 *
 * note - in 4D case w component is ignored.
 */
double
FUNCTION_NAME(dot)(struct VECTOR_NAME a, struct VECTOR_NAME b);

#if(VECTOR_SIZE == 2)

/* cross - 2D vector cross product: a x b. Equivilent to the 2D wedge product,
 * i.e. returns the scalar area of the parallelogram with sides a and b
 *
 * note - in 4D case w component is ignored.
 */
double
FUNCTION_NAME(cross)(struct VECTOR_NAME a, struct VECTOR_NAME b);

#else

/* cross - 3D vector cross product: a x b
 *
 * note - in 4D case return vector's w = a.w * b.w: thus:
 *    direction x direction = direction
 *    direction x point = direction
 *    point x point = point
 *    point x direction = direction
 */
struct VECTOR_NAME
FUNCTION_NAME(cross)(struct VECTOR_NAME a, struct VECTOR_NAME b);

#endif

/* hadamard - vector hadamard product, i.e. componentwise product.
 *
 * note - in 4D case returned vector's w = a.w * b.w
 */
struct VECTOR_NAME
FUNCTION_NAME(hadamard)(struct VECTOR_NAME a, struct VECTOR_NAME b);

/* length - length of vector a
 *
 * note - in 4D case w component is ignored.
 */
double
FUNCTION_NAME(length)(struct VECTOR_NAME a);

/* length_squared - square length of vector a; skips square root calculation.
 *
 * note - in 4D case w component is ignored.
 */
double
FUNCTION_NAME(length_squared)(struct VECTOR_NAME a);

/* norm - normalise vector a
 *
 * note - in 4D case returned vector's w = a.w
 */
struct VECTOR_NAME
FUNCTION_NAME(normalise)(struct VECTOR_NAME a);

/**** SPHERICAL VECTOR *******************************************************/

#if(VECTOR_SIZE > 2)

#define SPHNAMECAT0(name, size, type) name ## size ## type
#define SPHNAMECAT1(name, SIZE, TYPE) SPHNAMECAT0(name, SIZE, TYPE)
#define SPHERICAL_NAME SPHNAMECAT1(spherical, VECTOR_SIZE, TYPE_SUFFIX) 

/* A vector in spherical coordinates. Elements follow the ISO convention
 * used in physics, i.e:
 *
 * r - radial distance to point.
 * theta - polar angle, i.e. rotation around zenith direction: 0 <= theta < 2PI
 * phi - azimuthal angle, i.e. angle from zenith direction: 0 <= PHI < PI
 *
 * note - phi=0 means collinear and parallel with zenith direction.
 *
 * note - w=0 means a direction vector, w=1 means a position vector.
 */
struct SPHERICAL_NAME
{
  VECTOR_TYPE r;
  VECTOR_TYPE theta;
  VECTOR_TYPE phi;

#if (VECTOR_SIZE > 3)
  VECTOR_TYPE w;
#endif

};

/* spherical_to_cartesian - converts a spherical position vector to its
 * cartesian equivilent; origins coincide.
 *
 * note - this function requires a mapping between the two coordinate spaces
 * to be defined; a zenith direction and reference plane must be mapped.
 *
 * To be consistant with opengl:
 *
 * zenith direction = (+)y-axis, i.e. world up direction.
 * reference plane  = x-z plane.
 *
 * theta=0 means vector collinear and parallel with (+)x-axis. Thus increasing
 * theta rotates from the (+)x-axis through the (+)z-axis.
 *
 * phi=0 means vector collinear and parallel with (+)y-axis, likewise, phi=PI
 * means vector collinear and anti-parallel with (-)y-axis.
 *
 * note - in 4D case returned vector's w = a.w
 */
struct VECTOR_NAME
FUNCTION_NAME(spherical_to_cartesian)(struct SPHERICAL_NAME a);

#endif

/**** POLAR VECTOR ***********************************************************/

// TODO

#ifndef __VECTOR_C_SOURCE
#undef TYPE_SUFFIX
#undef VECNAMECAT0
#undef VECNAMECAT1
#undef VECTOR_NAME
#undef SPHNAMECAT0
#undef SPHNAMECAT1
#undef SPHERICAL_NAME
#undef FUNNAMECAT0
#undef FUNNAMECAT1
#undef FUNCTION_NAME
#undef VECTOR_SIZE
#undef VECTOR_TYPE
#endif
