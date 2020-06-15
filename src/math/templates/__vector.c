
#define __VECTOR_C_SOURCE

#include <math.h>
#include <assert.h>

#include "__vector.h"

/**** CARTESIAN VECTOR *******************************************************/

struct VECTOR_NAME
FUNCTION_NAME(add)(struct VECTOR_NAME a, struct VECTOR_NAME b)
{
  assert(!(a.w == (VECTOR_TYPE)1 && (VECTOR_TYPE)b.w == 1));

  return (struct VECTOR_NAME){
    a.x + b.x,
    a.y + b.y
    
#if(VECTOR_SIZE > 2)
    , a.z + b.z
#endif

#if(VECTOR_SIZE > 3)
    , a.w + b.w
#endif
  };
}

/* b - a */
struct VECTOR_NAME
FUNCTION_NAME(sub)(struct VECTOR_NAME a, struct VECTOR_NAME b)
{
  assert(!(a.w == (VECTOR_TYPE)1 && (VECTOR_TYPE)b.w == 0));

  return (struct VECTOR_NAME){
    b.x - a.x,
    b.y - a.y
    
#if(VECTOR_SIZE > 2)
    , b.z - a.z
#endif

#if(VECTOR_SIZE > 3)
    , b.w - a.w
#endif
  };
}

struct VECTOR_NAME
FUNCTION_NAME(scale)(struct VECTOR_NAME a, float scale)
{
  return (struct VECTOR_NAME){
    a.x * scale,
    a.y * scale
    
#if(VECTOR_SIZE > 2)
    , a.z * scale
#endif

#if(VECTOR_SIZE > 3)
    , a.w
#endif

  };
}
double
FUNCTION_NAME(dot)(struct VECTOR_NAME a, struct VECTOR_NAME b)
{
  return (double)(\
    (a.x * b.x) + \
    (a.y * b.y)   \
    
#if(VECTOR_SIZE > 2)
    + (a.z * b.z)
#endif

  );
}

#if(VECTOR_SIZE == 2)
double
FUNCTION_NAME(cross)(struct VECTOR_NAME a, struct VECTOR_NAME b)
{
  return (double)((a.x * b.y) - (a.y * b.x));
}
#else
struct VECTOR_NAME
FUNCTION_NAME(cross)(struct VECTOR_NAME a, struct VECTOR_NAME b)
{
  return (struct VECTOR_NAME){
    ((a.y * b.z) - (a.z * b.y)),
    -((a.x * b.z) - (a.z * b.x)),
    ((a.x * b.y) - (a.y * b.x))

#if(VECTOR_SIZE > 3)
    , a.w * b.w
#endif

  };
}
#endif

struct VECTOR_NAME
FUNCTION_NAME(hadamard)(struct VECTOR_NAME a, struct VECTOR_NAME b)
{
  return (struct VECTOR_NAME){
    a.x * b.x,
    a.y * b.y
    
#if(VECTOR_SIZE > 2)
    , a.z * b.z
#endif

#if(VECTOR_SIZE > 3)
    , a.w * b.w
#endif
  };
}

double
FUNCTION_NAME(length)(struct VECTOR_NAME a)
{
  return (double)sqrt(\
    (a.x * a.x) +     \
    (a.y * a.y)       \

#if(VECTOR_SIZE > 2)
    + (a.z * a.z)
#endif
  );
}

double
FUNCTION_NAME(length_squared)(struct VECTOR_NAME a)
{
  return (double)(\
    (a.x * a.x) + \
    (a.y * a.y)   \

#if(VECTOR_SIZE > 2)
    + (a.z * a.z)
#endif
  );
}

struct VECTOR_NAME
FUNCTION_NAME(normalise)(struct VECTOR_NAME a)
{
  double invlen = 1.0 / sqrt(\
    (a.x * a.x) +            \
    (a.y * a.y)              \

#if(VECTOR_SIZE > 2)
    + (a.z * a.z)
#endif
  );
  
  return (struct VECTOR_NAME){
    (a.x * invlen),
    (a.y * invlen)

#if(VECTOR_SIZE > 2)
    , (a.z * invlen)
#endif

#if(VECTOR_SIZE > 3)
    , a.w
#endif

  };
}

/**** SPHERICAL VECTOR *******************************************************/

#if(VECTOR_SIZE == 3)

struct VECTOR_NAME
FUNCTION_NAME(spherical_to_cartesian)(struct SPHERICAL_NAME a)
{
  return (struct VECTOR_NAME){
    a.r * cos(a.theta) * sin(a.phi),
    a.r * cos(a.phi),
    a.r * sin(a.theta) * sin(a.phi)

#if (VECTOR_SIZE > 3)
    , a.w
#endif

  };
}

#endif

/**** POLAR VECTOR ***********************************************************/

// TODO

