#version 120

uniform mat4 projection_matrix;
uniform mat4 model_view_matrix;

attribute vec3 position;

void main()
{
  gl_Position = projection_matrix * model_view_matrix * vec4(position, 1.0);
}

