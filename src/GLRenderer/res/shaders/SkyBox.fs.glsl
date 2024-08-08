#version 330 core
#extension GL_ARB_shader_image_load_store : enable
layout(early_fragment_tests) in;
uniform samplerCube tex;
in vec3 frag_tex_coord;
out vec4 FragColor;
void main(){
    vec4 color = texture(tex, frag_tex_coord);
    FragColor = color;
}
        