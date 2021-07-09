#version 330
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    // get distance between fragment and light source
    float lightDist = length(FragPos.xyz - lightPos);

    // Scale down by far_plane to clip far values
    lightDist = lightDist / far_plane;

    // write this to buff
    gl_FragDepth = lightDist;
}

