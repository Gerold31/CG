#version 150 core

in vec3 fEyeDirection;

out vec4 pcolor;

float PI = 3.14159265358979323846264;
float SCALE_X = 10;
float SCALE_Y = 20;

float modf(float x, float y)
{
	return x - y * floor(x/y);
}

float rand(vec3 x)
{
	// @todo get this from outside
	int seed = 1;

	x.x = modf(x.x, SCALE_X);
	x.y = modf(x.y, SCALE_Y);

	int n = seed + int(x.x * 57. + x.y * 83. + x.z * 127.);
	n = (n<<13) ^ n;
	return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float interpolate(float a, float b, float x)
{
/*
	float f = x - floor(x);
	return (1-f)*a + f*b;
*/

	float ft = (x - floor(x)) * 3.1415927;
	float f = (1 - cos(ft)) * 0.5;

	return  a*(1-f) + b*f;
}

float interpolate_lin(float a, float b, float x)
{
	float f = x - floor(x);
	return (1-f)*a + f*b;
}

float interpolateNoise(vec3 x)
{
	float r000 = rand(vec3(floor(x.x)    , floor(x.y)    , floor(x.z)    ));
	float r001 = rand(vec3(floor(x.x)    , floor(x.y)    , floor(x.z) + 1));
	float r010 = rand(vec3(floor(x.x)    , floor(x.y) + 1, floor(x.z)    ));
	float r011 = rand(vec3(floor(x.x)    , floor(x.y) + 1, floor(x.z) + 1));
	float r100 = rand(vec3(floor(x.x) + 1, floor(x.y)    , floor(x.z)    ));
	float r101 = rand(vec3(floor(x.x) + 1, floor(x.y)    , floor(x.z) + 1));
	float r110 = rand(vec3(floor(x.x) + 1, floor(x.y) + 1, floor(x.z)    ));
	float r111 = rand(vec3(floor(x.x) + 1, floor(x.y) + 1, floor(x.z) + 1));

	float r00 = interpolate(r000, r001, x.z);
	float r01 = interpolate(r010, r011, x.z);
	float r10 = interpolate(r100, r101, x.z);
	float r11 = interpolate(r110, r111, x.z);

	float r0 = interpolate(r00, r01, x.y);
	float r1 = interpolate(r10, r11, x.y);

	return interpolate(r0, r1, x.x);
}

float noise(vec3 x, float octaves, float persistance)
{
	// @todo get this from outside
	//float octaves = 4;
	//float persistance = .15;

	float total = 0;

	float amplitude = 1;
	float maxAmplitude = 0;

	for(int i=0; i<octaves; i++)
	{
		total += interpolateNoise(x) * amplitude;
		maxAmplitude += amplitude;

		x *= 2;

		amplitude *= persistance;
	}
	total /= maxAmplitude;

	return total;
}

void main()
{
	// @todo get this from outside
	vec3 base = vec3(40/256., 30/256., 150/256.);

	vec3 base_min = vec3(40/256., 30/256., 150/256.);
	vec3 base_max = vec3(256/256., 256/256., 256/256.);

	vec3 v = fEyeDirection;
	v.x = mod(fEyeDirection.x, 2*PI) / (2*PI);
	v.y = mod(fEyeDirection.y, 2*PI) / (2*PI);

	v.x *= SCALE_X;
	v.y *= SCALE_Y;

	float grain = (noise(v, 8, .5) + 1)/2;

	grain = pow(grain, 2);
	//grain = 1-cos(grain * PI/2);

	vec3 color = vec3(interpolate(base_min.r, base_max.r, grain), interpolate(base_min.g, base_max.g, grain), interpolate(base_min.b, base_max.b, grain));

	pcolor = vec4(color, 1.0);
}

