#version 150 core

in vec3 fpos;
in vec3 fnormal;

out vec4 pcolor;

const int MAX_LIGHTS = 64;

uniform vec3 camPos;

uniform int numLights;
uniform vec3 ambientLight;
uniform struct Light {
   vec4 position;
   vec3 color;
   float attenuation;
} lights[MAX_LIGHTS];

uniform vec3 specularColor = vec3(0.8, 0.8, 1.0);
uniform float shiningness = 10;

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

void main() {
	vec3 camToSurface = normalize(fpos - camPos);
	vec3 color = vec3(0.5, 0.5, 1.0);

	pcolor = vec4(color * ambientLight, 1);
	for(int i = 0; i < numLights; i++)
	{
		pcolor += vec4(applyLight(lights[i], fpos, fnormal, color, camToSurface), 1);
	}
}
