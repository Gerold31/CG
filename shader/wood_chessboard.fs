#version 150 core

in vec3 fpos;
in vec3 fcolor;
in vec3 fnormal;
in float fseed;

uniform mat4 model;
uniform vec3 camPos;
uniform vec3 ambientLight;
uniform int numLights;

#define MAX_LIGHTS 64
uniform struct Light {
   vec4 position;
   vec3 color;
   float attenuation;
} lights[MAX_LIGHTS];

out vec4 pcolor;

vec3 specularColor = 1.3 * fcolor;
float shiningness = 2;

float rand(vec3 x)
{
	int seed = int(fseed + .5);

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

vec3 applyLight(Light light, vec3 surfacePos, vec3 surfaceNormal, vec3 surfaceColor, vec3 camToSurface)
{
	vec3 surfaceToLight = normalize(light.position.xyz - surfacePos);
	float distanceToLight = length(light.position.xyz - surfacePos);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

	float diffuseCoefficient = max(0.0, dot(surfaceNormal, surfaceToLight));
	vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.color;

	float specularCoefficient = 0.0;
	if(diffuseCoefficient > 0.0)
		specularCoefficient = pow(max(0.0, dot(-camToSurface, reflect(-surfaceToLight, surfaceNormal))), shiningness);
	vec3 specular = specularCoefficient * specularColor * light.color;

	return attenuation * (diffuse + specular);
}

void main()
{
	vec3 base = fcolor;
	vec3 base_min = 0.9*base;
	vec3 base_max = 1.1*base;

	vec3 v = vec3(fpos.x/20, fpos.y/100, fpos.z/20);

	float grain = (noise(v, 3, .2) + 1) * 60;
	grain = grain - int(grain);

	float t = .8;

	if(grain < t)
		grain = grain/t;
	else
		grain = (1-grain)/(1-t);

//	grain = pow(cos(grain), 3);
	grain = 1-pow(grain, 2);
//	grain = 1-pow(2, -5 * pow(grain, 2));

	vec3 color = vec3(interpolate(base_min.r, base_max.r, grain), interpolate(base_min.g, base_max.g, grain), interpolate(base_min.b, base_max.b, grain));

	vec3 spos = vec3(model * vec4(fpos, 1.));
	vec3 snormal = normalize(transpose(inverse(mat3(model))) * fnormal);
	vec3 camToSurface = normalize(spos - camPos);

	pcolor = vec4(color.rgb * ambientLight, 1);
	for(int i=0; i<numLights; i++)
	{
		pcolor += vec4(applyLight(lights[i], spos, snormal, color, camToSurface), 1);
	}

}

