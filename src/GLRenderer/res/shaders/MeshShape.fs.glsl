#version 330 core
#define MAX_LIGHT 8
// #extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shader_image_load_store : enable
// layout(location = 0) uniform sampler2D tex_2d;
struct Material{
    vec4 a;
    vec4 d;
    vec4 s;
    vec4 e;
    float sh;
};

struct Light{
    // Common
    vec4 a;
    vec4 d;
    vec4 s;
    vec3 dir;

    // Spot
    vec4 pos;
    // cutoff
    float co;
    // exponent
    float expo;

    float k_c;
    float k_l;
    float k_q;
};
layout(early_fragment_tests) in;
uniform samplerCube tex;
uniform Material mate;
uniform Light lights[MAX_LIGHT];
uniform int lights_count = 0;
uniform vec3 view_dir;

in vec4 frag_color;
in vec3 frag_tex_coord;
in vec3 frag_world_posi;
in vec3 frag_world_norm;

out vec4 FragColor;

vec3 get_directional_light_contribution(Light l, Material m, vec3 view_dir, vec3 frag_norm){
    vec3 l_dir = l.dir;
    vec3 reflect_dir = reflect(l.dir, frag_norm);
    vec3 a = l.a.rgb * m.a.rgb;
    vec3 d = l.d.rgb * max(dot(-l_dir, frag_norm), 0) * /* mate.d.rgb*/ texture(tex, frag_tex_coord).rgb;
    // Phong
    // vec3 s = l.s.rgb * pow(max(dot(view_dir, reflect_dir), 0.0), mate.sh) * mate.s.rgb;
    // Blinn_Phong
    vec3 s = l.s.rgb * pow(max(dot(normalize(-l_dir + view_dir), frag_norm), 0.0), mate.sh) * mate.s.rgb;
    return a + d + s;
}

vec3 get_spot_light_contribution(Light l, Material m, vec3 view_dir, vec3 frag_posi, vec3 frag_norm){
    vec3 l_dir = frag_posi - l.pos.xyz;
    l_dir = normalize(l_dir);
    vec3 reflect_dir = reflect(l_dir, frag_norm);
    vec3 a = l.a.rgb * m.a.rgb;
    vec3 d = l.d.rgb * max(dot(-l_dir, frag_norm), 0) * /* mate.d.rgb*/ texture(tex, frag_tex_coord).rgb;
    vec3 s = l.s.rgb * pow(max(dot(view_dir, reflect_dir), 0.0), mate.sh) * mate.s.rgb;
    return a + d + s;
}

void main(){
    vec4 color = texture(tex, frag_tex_coord);
    if(lights_count > 0){
        vec3 c = vec3(0, 0, 0);
        for(int i = 0; i < lights_count; i++){
            if(i >= MAX_LIGHT)
                break;
            Light l = lights[i];
            if(l.pos.w == 1.0){
                c += get_directional_light_contribution(l, mate, view_dir, frag_world_norm);
            }
            else{
                c += get_spot_light_contribution(l, mate, view_dir, frag_world_posi, frag_world_norm);
            }
        }
        color = vec4(c, 1.0);
    }
    FragColor = color;
}