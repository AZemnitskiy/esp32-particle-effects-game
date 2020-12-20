#ifndef collisions_h
#define collisions_h

#ifdef _MSC_VER
	#define C2_INLINE __forceinline
#else
	#define C2_INLINE inline __attribute__((always_inline))
#endif

#include <math.h>

// 2d vector
typedef struct c2v
{
	float x;
	float y;
} c2v;


typedef struct c2Circle
{
	c2v p;
	float r;
} c2Circle;

// a capsule is defined as a line segment (from a to b) and radius r
typedef struct c2Capsule
{
	c2v a;
	c2v b;
	float r;
} c2Capsule;


C2_INLINE c2v c2V(float x, float y) { c2v a; a.x = x; a.y = y; return a; }
C2_INLINE c2v c2Sub(c2v a, c2v b) { a.x -= b.x; a.y -= b.y; return a; }
C2_INLINE float c2Dot(c2v a, c2v b) { return a.x * b.x + a.y * b.y; }
C2_INLINE c2v c2Mulvs(c2v a, float b) { a.x *= b; a.y *= b; return a; }

// see: http://www.randygaul.net/2014/07/23/distance-point-to-line-segment/
bool c2CircletoCapsule(c2Circle A, c2Capsule B)
{
	c2v n = c2Sub(B.b, B.a);
	c2v ap = c2Sub(A.p, B.a);
	float da = c2Dot(ap, n);
	float d2;

	if (da < 0) d2 = c2Dot(ap, ap);
	else
	{
		float db = c2Dot(c2Sub(A.p, B.b), n);
		if (db < 0)
		{
			c2v e = c2Sub(ap, c2Mulvs(n, (da / c2Dot(n, n))));
			d2 = c2Dot(e, e);
		}
		else
		{
			c2v bp = c2Sub(A.p, B.b);
			d2 = c2Dot(bp, bp);
		}
	}

	float r = A.r + B.r;
	return d2 < r * r;
}

bool c2CircletoRect(c2Circle circle, c2v rectStart, float rectWidth, float rectHeight)
{
    float x = abs(circle.p.x - rectStart.x);
    float y = abs(circle.p.y - rectStart.y);

    if (x > (rectWidth/2 + circle.r)) { return false; }
    if (y > (rectHeight/2 + circle.r)) { return false; }

    if (x <= (rectWidth/2)) { return true; } 
    if (y <= (rectHeight/2)) { return true; }

    float cornerDistance_sq = pow(x - rectWidth/2, 2) +
                         pow(y - rectHeight/2,2);

    return (cornerDistance_sq <= pow(circle.r,2));
}

#endif /* gfx_particle_sys_h */
