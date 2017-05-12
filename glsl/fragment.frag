#version 430 core

out vec4 outColor;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

uniform vec3 viewPos;
uniform bool wireframe;
uniform sampler2D diffuseTexture;


struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;


void main()
{
	// ambient component
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragTexCoord));

	// diffuse component
	vec3 normal = normalize(fragNormal);
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * vec3(texture(material.diffuse, fragTexCoord)));

	// specular component
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, fragTexCoord)));

	if(wireframe)
	{
		outColor = vec4(1.0);
	}
	else
	{
		outColor = texture(material.diffuse, fragTexCoord) * vec4(ambient + diffuse + specular, 1.0);
		outColor = vec4(ambient + diffuse + specular, 1.0);
	}
}
