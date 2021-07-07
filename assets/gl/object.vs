#version 330 core
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

#define DIR_LIGHTS @NUM_DIR_LIGHTS@

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace[MAX(DIR_LIGHTS,1)];
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 dirLightView[MAX(DIR_LIGHTS,1)];
uniform mat4 dirLightProj[MAX(DIR_LIGHTS,1)];


void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
    for(int i = 0; i < DIR_LIGHTS; i++)
    {
        vs_out.FragPosLightSpace[i] = dirLightProj[i] * dirLightView[i] 
            * vec4(vs_out.FragPos, 1.0);
    }

	gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);

}
