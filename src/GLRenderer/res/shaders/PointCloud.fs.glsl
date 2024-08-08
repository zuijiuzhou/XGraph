#version 330 core

#define MOD(a, b) a - b * floor(a / b)

in vec4 frag_color;
flat in int point_id;


out vec4 FragColor;

void main(){
   
    vec2 p = gl_PointCoord * 2.0 - vec2(1.0);
    if(dot(p,p) > sin(atan(p.y, p.x) * 5.0))
                discard;
    FragColor = frag_color;
}