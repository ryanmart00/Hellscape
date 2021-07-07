
//Old

#version 330 core
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

#define DIR_LIGHTS @NUM_DIR_LIGHTS@

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    mat4 DirFragPosLightSpace[MAX(DIR_LIGHTS,1)];
} gs_in[];

flat out vec3 Tri[3];
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 DirFragPosLightSpace[MAX(DIR_LIGHTS,1)];

void main() {
    Tri[0] = gs_in[0].FragPos;
    Tri[1] = gs_in[1].FragPos;
    Tri[2] = gs_in[2].FragPos;

    

    gl_Position = gl_in[0].gl_Position;
    FragPos = gs_in[0].FragPos;
    Normal = gs_in[0].Normal;
    TexCoords = gs_in[0].TexCoords;
    EmitVertex();
        
    gl_Position = gl_in[1].gl_Position;
    FragPos = gs_in[1].FragPos;
    Normal = gs_in[1].Normal;
    TexCoords = gs_in[1].TexCoords;
    EmitVertex();
    
    gl_Position = gl_in[2].gl_Position;
    FragPos = gs_in[2].FragPos;
    Normal = gs_in[2].Normal;
    TexCoords = gs_in[2].TexCoords;
    EmitVertex();

    EndPrimitive();
    
}
