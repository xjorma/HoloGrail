#pragma once

// https://www.shadertoy.com/view/4djSRW
inline float hash11(float p)
{
    p = fract(p * .1031f);
    p *= p + 33.33f;
    p *= p + p;
    return fract(p);
}


inline vec3 hash31(float p)
{
	vec3 p3 = fract(vec3(p) * vec3(.1031f, .1030f, .0973f));
	p3 += dot(p3, p3.yzx + vec3(33.33f));
	return fract((p3.xxy + p3.yzz) * p3.zyx);
}

inline vec2 hash21(float p)
{
    vec3 p3 = fract(vec3(p) * vec3(.1031f, .1030f, .0973f));
    p3 += dot(p3, p3.yzx + vec3(33.33f));
    return fract((p3.xx + p3.yz) * p3.zy);
}

inline vec3 shash31(float p)
{
    return (hash31(p) - vec3(0.5f)) * 2.0f;
}


// https://www.shadertoy.com/view/4ttSWf
inline float hash1(vec2 p)
{
    p = 50.f * fract(p * 0.3183099f);
    return fract(p.x * p.y * (p.x + p.y));
}

inline float noise(vec2 x)
{
    vec2 p = floor(x);
    vec2 w = fract(x);
    vec2 u = w * w * w * (w * (w * 6.f - 15.f) + 10.f);

    float a = hash1(p + vec2(0, 0));
    float b = hash1(p + vec2(1, 0));
    float c = hash1(p + vec2(0, 1));
    float d = hash1(p + vec2(1, 1));

    return -1.f + 2.f * (a + (b - a) * u.x + (c - a) * u.y + (a - b - c + d) * u.x * u.y);
}


// https://github.com/BrianSharpe/GPU-Noise-Lib/blob/master/gpu_noise_lib.glsl#L2351

//
//	Given an arbitrary 3D point this calculates the 4 vectors from the corners of the simplex pyramid to the point
//	It also returns the integer grid index information for the corners
//
inline void Simplex3D_GetCornerVectors(vec3 P,					//	input point
    vec3 &Pi,			//	integer grid index for the origin
    vec3 &Pi_1,			//	offsets for the 2nd and 3rd corners.  ( the 4th = Pi + 1.0 )
    vec3 &Pi_2,
    vec4 &v1234_x,		//	vectors from the 4 corners to the intput point
    vec4 &v1234_y,
    vec4 &v1234_z)
{
    //
    //	Simplex math from Stefan Gustavson's and Ian McEwan's work at...
    //	http://github.com/ashima/webgl-noise
    //

    //	simplex math constants
    const float SKEWFACTOR = 1.0f / 3.0f;
    const float UNSKEWFACTOR = 1.0f / 6.0f;
    const float SIMPLEX_CORNER_POS = 0.5;
    const float SIMPLEX_PYRAMID_HEIGHT = 0.70710678118654752440084436210485f;	// sqrt( 0.5 )	height of simplex pyramid.

    P *= SIMPLEX_PYRAMID_HEIGHT;		// scale space so we can have an approx feature size of 1.0  ( optional )

    //	Find the vectors to the corners of our simplex pyramid
    Pi = floor(P + dot(P, vec3(SKEWFACTOR)));
    vec3 x0 = P - Pi + dot(Pi, vec3(UNSKEWFACTOR));
    vec3 g = step(vec3(x0.yzx), vec3(x0.xyz));
    vec3 l = 1.0f - g;
    Pi_1 = min(vec3(g.xyz), vec3(l.zxy));
    Pi_2 = max(vec3(g.xyz), vec3(l.zxy));
    vec3 x1 = x0 - Pi_1 + UNSKEWFACTOR;
    vec3 x2 = x0 - Pi_2 + SKEWFACTOR;
    vec3 x3 = x0 - SIMPLEX_CORNER_POS;

    //	pack them into a parallel-friendly arrangement
    v1234_x = vec4(x0.x, x1.x, x2.x, x3.x);
    v1234_y = vec4(x0.y, x1.y, x2.y, x3.y);
    v1234_z = vec4(x0.z, x1.z, x2.z, x3.z);
}

inline void FAST32_hash_3D(vec3 gridcell,
    vec3 v1_mask,		//	user definable v1 and v2.  ( 0's and 1's )
    vec3 v2_mask,
    vec4 &hash_0,
    vec4 &hash_1,
    vec4 &hash_2)		//	generates 3 random numbers for each of the 4 3D cell corners.  cell corners:  v0=0,0,0  v3=1,1,1  the other two are user definable
{
    //    gridcell is assumed to be an integer coordinate

    //	TODO: 	these constants need tweaked to find the best possible noise.
    //			probably requires some kind of brute force computational searching or something....
    const vec2 OFFSET = vec2(50.0, 161.0);
    const float Domain = 69.0;
    const vec3 SOMELARGEFLOATS = vec3(635.298681, 682.357502, 668.926525);
    const vec3 ZINC = vec3(48.500388, 65.294118, 63.934599);

    //	truncate the domain
    gridcell.xyz = gridcell.xyz - floor(gridcell * (1.0f / Domain)) * Domain;
    vec3 gridcell_inc1 = step(gridcell, vec3(Domain - 1.5f)) * (gridcell + 1.0f);

    //	compute x*x*y*y for the 4 corners
    vec4 P = vec4(gridcell.xy, gridcell_inc1.xy) + OFFSET.xyxy;
    P *= P;
    vec4 V1xy_V2xy = mix(vec4(P.xyxy), vec4(P.zwzw), vec4(v1_mask.xy, v2_mask.xy));		//	apply mask for v1 and v2
    P = vec4(P.x, V1xy_V2xy.xz, P.z) * vec4(P.y, V1xy_V2xy.yw, P.w);

    //	get the lowz and highz mods
    vec3 lowz_mods = vec3(1.0f / (SOMELARGEFLOATS.xyz + gridcell.zzz * ZINC.xyz));
    vec3 highz_mods = vec3(1.0f / (SOMELARGEFLOATS.xyz + gridcell_inc1.zzz * ZINC.xyz));

    //	apply mask for v1 and v2 mod values
    v1_mask = (v1_mask.z < 0.5) ? lowz_mods : highz_mods;
    v2_mask = (v2_mask.z < 0.5) ? lowz_mods : highz_mods;

    //	compute the final hash
    hash_0 = fract(P * vec4(lowz_mods.x, v1_mask.x, v2_mask.x, highz_mods.x));
    hash_1 = fract(P * vec4(lowz_mods.y, v1_mask.y, v2_mask.y, highz_mods.y));
    hash_2 = fract(P * vec4(lowz_mods.z, v1_mask.z, v2_mask.z, highz_mods.z));
}

//
//	SimplexPerlin3D_Deriv
//	SimplexPerlin3D noise with derivatives
//	returns vec3( value, xderiv, yderiv, zderiv )
//
inline vec4 SimplexPerlin3D_Deriv(vec3 P)
{
    //	calculate the simplex vector and index math
    vec3 Pi;
    vec3 Pi_1;
    vec3 Pi_2;
    vec4 v1234_x;
    vec4 v1234_y;
    vec4 v1234_z;
    Simplex3D_GetCornerVectors(P, Pi, Pi_1, Pi_2, v1234_x, v1234_y, v1234_z);

    //	generate the random vectors
    //	( various hashing methods listed in order of speed )
    vec4 hash_0;
    vec4 hash_1;
    vec4 hash_2;
    FAST32_hash_3D(Pi, Pi_1, Pi_2, hash_0, hash_1, hash_2);
    //SGPP_hash_3D( Pi, Pi_1, Pi_2, hash_0, hash_1, hash_2 );
    hash_0 -= 0.49999f;
    hash_1 -= 0.49999f;
    hash_2 -= 0.49999f;

    //	normalize random gradient vectors
    vec4 norm = inversesqrt(hash_0 * hash_0 + hash_1 * hash_1 + hash_2 * hash_2);
    hash_0 *= norm;
    hash_1 *= norm;
    hash_2 *= norm;

    //	evaluate gradients
    vec4 grad_results = hash_0 * v1234_x + hash_1 * v1234_y + hash_2 * v1234_z;

    //	evaluate the surflet f(x)=(0.5-x*x)^3
    vec4 m = v1234_x * v1234_x + v1234_y * v1234_y + v1234_z * v1234_z;
    m = max(0.5f - m, 0.0f);		//	The 0.5 here is SIMPLEX_PYRAMID_HEIGHT^2
    vec4 m2 = m * m;
    vec4 m3 = m * m2;

    //	calc the deriv
    vec4 temp = -6.0f * m2 * grad_results;
    float xderiv = dot(temp, v1234_x) + dot(m3, hash_0);
    float yderiv = dot(temp, v1234_y) + dot(m3, hash_1);
    float zderiv = dot(temp, v1234_z) + dot(m3, hash_2);

    const float FINAL_NORMALIZATION = 37.837227241611314102871574478976f;	//	scales the final result to a strict 1.0->-1.0 range

    //	sum with the surflet and return
    return vec4(dot(m3, grad_results), xderiv, yderiv, zderiv) * FINAL_NORMALIZATION;
}

//	Simplex 4D Noise 
//	by Ian McEwan, Ashima Arts
//
inline vec4 permute(vec4 x) { return mod(((x * 34.0f) + 1.0f) * x, 289.0f); }
inline float permute(float x) { return floor(mod(((x * 34.0f) + 1.0f) * x, 289.0f)); }
inline vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159f - 0.85373472095314f * r; }
inline float taylorInvSqrt(float r) { return 1.79284291400159f - 0.85373472095314f * r; }

inline vec4 grad4(float j, vec4 ip) {
    vec4 p, s;

    p.xyz = floor(fract(vec3(j) * ip.xyz) * 7.0f) * ip.z - 1.0f;
    p.w = 1.5f - dot(abs(vec3(p.xyz)), vec3(1.0f));

    s = vec4(lessThan(p, vec4(0.0f)));
    p.xyz = p.xyz + (s.xyz * 2.0f - 1.0f) * s.www;

    return p;
}

inline float snoise(vec4 v) {
    const vec2  C = vec2(0.138196601125010504f,  // (5 - sqrt(5))/20  G4
        0.309016994374947451f); // (sqrt(5) - 1)/4   F4
// First corner
    vec4 i = floor(v + dot(v, vec4(C.yyyy)));
    vec4 x0 = v - i + dot(i, vec4(C.xxxx));

    // Other corners

    // Rank sorting originally contributed by Bill Licea-Kane, AMD (formerly ATI)
    vec4 i0;

    vec3 isX = step(vec3(x0.yzw), vec3(x0.xxx));
    vec3 isYZ = step(vec3(x0.zww), vec3(x0.yyz));
    //  i0.x = dot( isX, vec3( 1.0 ) );
    i0.x = isX.x + isX.y + isX.z;
    i0.yzw = 1.0f - isX;

    //  i0.y += dot( isYZ.xy, vec2( 1.0 ) );
    i0.y += isYZ.x + isYZ.y;
    i0.zw += 1.0f - isYZ.xy;

    i0.z += isYZ.z;
    i0.w += 1.0f - isYZ.z;

    // i0 now contains the unique values 0,1,2,3 in each channel
    vec4 i3 = clamp(i0, 0.0f, 1.0f);
    vec4 i2 = clamp(i0 - 1.0f, 0.0f, 1.0f);
    vec4 i1 = clamp(i0 - 2.0f, 0.0f, 1.0f);

    //  x0 = x0 - 0.0 + 0.0 * C 
    vec4 x1 = x0 - i1 + 1.0f * C.xxxx;
    vec4 x2 = x0 - i2 + 2.0f * C.xxxx;
    vec4 x3 = x0 - i3 + 3.0f * C.xxxx;
    vec4 x4 = x0 - 1.0f + 4.0f * C.xxxx;

    // Permutations
    i = mod(i, vec4(289.0));
    float j0 = permute(permute(permute(permute(i.w) + i.z) + i.y) + i.x);
    vec4 j1 = permute(permute(permute(permute(
        i.w + vec4(i1.w, i2.w, i3.w, 1.0f))
        + i.z + vec4(i1.z, i2.z, i3.z, 1.0f))
        + i.y + vec4(i1.y, i2.y, i3.y, 1.0f))
        + i.x + vec4(i1.x, i2.x, i3.x, 1.0f));
    // Gradients
    // ( 7*7*6 points uniformly over a cube, mapped onto a 4-octahedron.)
    // 7*7*6 = 294, which is close to the ring size 17*17 = 289.

    vec4 ip = vec4(1.0f / 294.0f, 1.0f / 49.0f, 1.0f / 7.0f, 0.0f);

    vec4 p0 = grad4(j0, ip);
    vec4 p1 = grad4(j1.x, ip);
    vec4 p2 = grad4(j1.y, ip);
    vec4 p3 = grad4(j1.z, ip);
    vec4 p4 = grad4(j1.w, ip);

    // Normalise gradients
    vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;
    p4 *= taylorInvSqrt(dot(p4, p4));

    // Mix contributions from the five corners
    vec3 m0 = max(0.6f - vec3(dot(x0, x0), dot(x1, x1), dot(x2, x2)), 0.0f);
    vec2 m1 = max(0.6f - vec2(dot(x3, x3), dot(x4, x4)), 0.0f);
    m0 = m0 * m0;
    m1 = m1 * m1;
    return 49.0f * (dot(m0 * m0, vec3(dot(p0, x0), dot(p1, x1), dot(p2, x2)))
        + dot(m1 * m1, vec2(dot(p3, x3), dot(p4, x4))));

}
