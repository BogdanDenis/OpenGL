#version 400 core

in vec3 fragPos;
in vec3 Normal;

uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 objectColour;
uniform vec3 lightColour;

out vec4 colour;

void main () {
	//Ambient
	float ambientStrength = 0.24f;
	vec3 ambient = ambientStrength * lightColour;
	//vec3 objColour = vec3 (0.2 / (1 - Normal.y + 0.1), 0.2 / (1 - Normal.y + 0.1), 0.2 / (1 - Normal.y + 0.1));
	vec3 grass = vec3 (0.541, 0.788, 0.368);
	vec3 ground = vec3 (0.368, 0.301, 0.192);
	vec3 objColour = mix (ground, mix (grass, ground, 0.8), Normal.y);

	//Diffuse
	vec3 norm = Normal;
	//gl_FrontFacing ? norm = norm : norm = -norm;
	vec3 lightDir = normalize (lightPos - fragPos);
	float diff = max (dot (norm, lightDir), 0.4);
	vec3 diffuse = diff * lightColour;

	//Specular
	float specularStrength = 0.001f;
	vec3 viewDir = normalize (cameraPos - fragPos);
	vec3 reflectDir = reflect (-lightDir, norm);
	float spec = pow (max (dot (viewDir, reflectDir), 0.0), 2);
	vec3 specular = specularStrength * spec * lightColour;
	vec3 result = (ambient + diffuse + specular) * objColour;
	colour = vec4 (result, 1.0);
}