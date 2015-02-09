#version 150 core

in vec3 fpos;
in vec3 fnormal;

uniform vec3 campos;
uniform mat4 model;
uniform vec3 ambientLight;
uniform int numLights;

#define MAX_LIGHTS 64
uniform struct Light {
   vec4 position;
   vec3 color;
   float attenuation;
} lights[MAX_LIGHTS];

vec3 specularColor = vec3(0.8, 0.8, 1.0);
float shiningness = 10;

out vec4 pcolor;

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
	vec3 color = vec3(0.5, 0.5, 1.0);

	vec3 spos = vec3(model * vec4(fpos, 1.));
	vec3 snormal = normalize(transpose(inverse(mat3(model))) * fnormal);
	vec3 camToSurface = normalize(spos - campos);

	pcolor = vec4(color.rgb * ambientLight, 1);
	for(int i=0; i<numLights; i++)
	{
		pcolor += vec4(applyLight(lights[i], spos, snormal, color, camToSurface), 1);
	}
}
