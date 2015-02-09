#version 150 core

in vec2 poscoord;

out vec3 fpos;
out vec3 fnormal;

uniform mat4 proj;
uniform vec3 campos;
uniform float shift;
uniform float height;

const float PI = 3.14159265358979323846264;

const float SCALE_X = 10;
const float SCALE_Y = 20;
const float SCALE_Z = 30;

const int SEED = 1;

const float OCTAVES = 8;
const float PERSISTANCE = .5;

float modf(float x, float y)
{
	return x - y * floor(x/y);
}

float rand(vec3 x)
{
	x.x = modf(x.x, SCALE_X);
	x.y = modf(x.y, SCALE_Y);
	x.z = modf(x.z, SCALE_Z);

	int n = SEED + int(x.x * 57. + x.y * 83. + x.z * 127.);
	n = (n<<13) ^ n;
	return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
}

float interpolate_cos(float a, float b, float x)
{
	float ft = (x - floor(x)) * PI;
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

	float r00 = interpolate_cos(r000, r001, x.z);
	float r01 = interpolate_cos(r010, r011, x.z);
	float r10 = interpolate_cos(r100, r101, x.z);
	float r11 = interpolate_cos(r110, r111, x.z);

	float r0 = interpolate_cos(r00, r01, x.y);
	float r1 = interpolate_cos(r10, r11, x.y);

	return interpolate_cos(r0, r1, x.x);
}

float noise(vec3 x)
{
	float total = 0;

	float amplitude = 1;
	float maxAmplitude = 0;

	for(int i = 0; i < OCTAVES; i++)
	{
		total += interpolateNoise(x) * amplitude;
		maxAmplitude += amplitude;

		x *= 2;

		amplitude *= PERSISTANCE;
	}
	total /= maxAmplitude;

	return total;
}

float translate(vec2 pos) {
	float grain = noise(vec3(pos, shift))/4;
	fnormal = vec3(cos(pos.x + shift * PI)/4, 1, 0);
	return sin(pos.x + shift * PI)/4 + grain;
}

void main() {
	vec2 pos = poscoord + campos.xz;
	gl_Position = proj * vec4(pos.x, height + translate(pos), pos.y, 1.0);
	fpos = vec3(pos.x, height + translate(pos), pos.y);
}
