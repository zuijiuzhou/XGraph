#version 450 core
layout(location = 0) in vec3 position;
// layout(location = 15) in vec4 color;
uniform mat4 osg_ModelViewProjectionMatrix;
uniform dmat4 m1;



out vec4 frag_color;
void main(){
  gl_Position = osg_ModelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0);
  frag_color = vec4(1, 1, 0, 1);

  double x = 1.0;
  uint y = 2;
  bool z = 0;
  
}