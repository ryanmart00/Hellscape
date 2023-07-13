#version 330 core
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

struct Material
{
    // no textures for now
    // sampler2D texture_diffuse1;
    // sampler2D texture_specular1;

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

    sampler2D shadow; 
};

#define DIR_LIGHTS @NUM_DIR_LIGHTS@

uniform DirLight directionalLights[MAX(DIR_LIGHTS,1)];

struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    samplerCube shadow; 
};

#define POINT_LIGHTS @NUM_POINT_LIGHTS@

uniform PointLight pointLights[MAX(POINT_LIGHTS,1)];

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace[MAX(DIR_LIGHTS,1)];
    vec3 Color;
} fs_in;

uniform vec3 viewPos;
uniform float far_plane;


vec3 CalcDirLight(DirLight light, int num, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);

    vec3 result = vec3(0.0,0.0,0.0);
    // Directional Lights
    for(int i = 0; i < DIR_LIGHTS; i++)
    {
        result += CalcDirLight(directionalLights[i], i, norm, viewDir);
    }

    // Point Lights
    for(int i = 0; i < POINT_LIGHTS; i++)
    {
        result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
    }

    float closestDepth = texture(pointLights[0].shadow, 
        fs_in.FragPos - pointLights[0].position).r;
    //FragColor = vec4(vec3(closestDepth / far_plane), 1.0);
    
    FragColor = vec4(result + fs_in.Color, 1.0);
}

vec3 CalcDirLight(DirLight light, int num, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

    // compute shadow
    vec3 projCoords = 0.5 * fs_in.FragPosLightSpace[num].xyz + 0.5;
    float fragmentDepth = projCoords.z;
    float bias = 0.005;
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(light.shadow, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(light.shadow, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += (fragmentDepth - bias <= pcfDepth || fragmentDepth > 1.0) ? 0.0 : 1.0;
        }
    }
    shadow /= 9.0;

    // combine
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + (1.0-shadow)*(diffuse + specular));
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 fragToLight = fragPos - light.position;
    vec3 lightDir = normalize(-fragToLight);
    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float atten = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));

    float currentDepth = length(fragToLight);
    float bias   = 0.05;
    float closestDepth = far_plane * texture(light.shadow, fragToLight).r;
    float shadow = currentDepth -  bias > closestDepth && currentDepth <= far_plane ? 1.0 : 0.0;


    // combine
    vec3 ambient  = light.ambient  * material.diffuse;
    vec3 diffuse  = light.diffuse  * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return atten * (ambient + (1.0f - shadow)*(diffuse + specular));
}


