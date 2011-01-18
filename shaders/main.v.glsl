#version 120

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

attribute vec3 vertex;

void main()
{
  gl_Position = projection_matrix * model_view_matrix * vec4(vertex, 1.0);
}

