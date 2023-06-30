#version 330 core
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 aBoneIdsA;
layout (location = 4) in ivec4 aBoneIdsB;
layout (location = 5) in vec4 aWeightsA;
layout (location = 6) in vec4 aWeightsB;

#define DIR_LIGHTS @NUM_DIR_LIGHTS@
#define MAX_BONES @NUM_MAX_BONES@

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace[MAX(DIR_LIGHTS,1)];
    vec3 Color;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 dirLightView[MAX(DIR_LIGHTS,1)];
uniform mat4 dirLightProj[MAX(DIR_LIGHTS,1)];

uniform mat4 bones[MAX(MAX_BONES,1)];
uniform int numBones;


void main()
{
    mat4 boneMat = mat4(1.0);
    if(aWeightsA.x != 0.0)
    {
        boneMat = aWeightsA.x * bones[aBoneIdsA.x] + aWeightsA.y * bones[aBoneIdsA.y] 
        + aWeightsA.z * bones[aBoneIdsA.z] + aWeightsA.w * bones[aBoneIdsA.w];
        + aWeightsB.x * bones[aBoneIdsB.x] + aWeightsB.y * bones[aBoneIdsB.y] 
        + aWeightsB.z * bones[aBoneIdsB.z] + aWeightsB.w * bones[aBoneIdsB.w];
    }
    vs_out.FragPos = vec3(model * boneMat * vec4(aPos, 1.0));
    vs_out.Normal = mat3(model) * mat3(boneMat) * aNormal;
    vs_out.TexCoords = aTexCoords;
    for(int i = 0; i < DIR_LIGHTS; i++)
    {
        vs_out.FragPosLightSpace[i] = dirLightProj[i] * dirLightView[i] 
            * vec4(vs_out.FragPos, 1.0);
    }

	gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
    vs_out.Color = vec3(0.0);
    vs_out.Color = vec3(aWeightsA.x * bones[aBoneIdsA.x] * vec4(1.0,0.0,0.0, 0.0));

}
