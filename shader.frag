#version 430 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 emission;
    float shininess;
};

struct pointLight{
    vec3 position;

    float constant;
    float linear;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 pos;
in vec4 vColor;
in vec2 textureCoord;
in vec3 vnormal;

out vec4 color;

uniform pointLight myPointLight;
uniform sampler2D myTexture;
uniform float useTexture;
uniform Material myMaterial;
uniform vec3 eyePos;

vec3 CalcPointLight(pointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance);   

	vec3 ambient = light.ambient * myMaterial.ambient;
	vec3 diffuse = light.diffuse * myMaterial.diffuse * max(dot(normal, lightDir), 0.0);
	vec3 specular = light.specular * myMaterial.specular * pow(max(dot(viewDir, reflectDir), 0.0), myMaterial.shininess);

    return (ambient + diffuse + specular) * attenuation;
}

void main(){
    vec3 normal = normalize(vnormal);
    vec3 viewDir = normalize(eyePos - pos);
    
    vec3 result = CalcPointLight(myPointLight, normal, pos, viewDir);
    color = vec4(result + myMaterial.emission, 1.f) * mix(vColor, texture(myTexture, textureCoord), useTexture);
}
