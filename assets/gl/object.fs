#version 330 core

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    vec3 diffuse;
    vec3 specular;

    float shininess;
};

uniform Material material;

struct DirLight 
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define DIR_LIGHTS 1

uniform DirLight directionalLights[DIR_LIGHTS];

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};
#define POINT_LIGHTS 4

uniform PointLight pointLights[POINT_LIGHTS];

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
flat in vec3 Tri[3];

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

bool onEdge(vec3 norm, vec3 viewDir)
{
    for(int i = 0; i < 3; i++)
    {
        //if(length(cross(Tri[i] - Tri[(i+1) % 3], Tri[i] - FragPos)) < 0.1)
        if(abs(dot(norm, viewDir)) < 0.1) 
        {
            return true;
        }
    }
    return false;
}

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional Lighting
    vec3 result = vec3(0.0,0.0,0.0);
    if (onEdge(norm, viewDir))
    {
        result = vec3(1.0, 0.5, 0.5);
    }
    else
    {
        // Directional Lights
        for(int i = 0; i < DIR_LIGHTS; i++)
        {
            result += CalcDirLight(directionalLights[i], norm, viewDir);
        }

        // Point Lights
        for(int i = 0; i < POINT_LIGHTS; i++)
        {
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        }
    }
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine
    vec3 ambient  = light.ambient  * vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * (material.diffuse +
        vec3(texture(material.texture_diffuse1, TexCoords)));
    vec3 specular = light.specular * spec * (material.specular + 
        vec3(texture(material.texture_specular1, TexCoords)));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float atten = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    // combine
    vec3 ambient  = atten * light.ambient  * (material.diffuse +
        vec3(texture(material.texture_diffuse1, TexCoords)));
    vec3 diffuse  = atten * light.diffuse  * diff *
        vec3(texture(material.texture_diffuse1, TexCoords));
    vec3 specular = atten * light.specular * spec * (material.specular + 
        vec3(texture(material.texture_specular1, TexCoords)));
    return (ambient + diffuse + specular);
}


