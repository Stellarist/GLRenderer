#version 450 core

layout(location=0)in vec3 a_pos;
layout(location=1)in vec3 a_normal;
layout(location=2)in vec2 a_tex_coord;

out vec2 tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    tex_coords=a_tex_coord;
    gl_Position=projection*view*model*vec4(a_pos,1.);
}
