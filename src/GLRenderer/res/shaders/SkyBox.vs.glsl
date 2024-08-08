#version 330 core
layout(location=0) in vec3 position;
layout(location=1) in vec3 tex_coord;

uniform mat4 matrix_mvp;

out vec3 frag_tex_coord;

void main(){
    vec4 posi = matrix_mvp * vec4(position.x, position.y, position.z, 1.0);
    posi.z = posi.w;
    gl_Position = posi;
    frag_tex_coord = tex_coord;
}