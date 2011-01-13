#include <string>
#include <vector>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#endif

const std::string strVertexShader(
    "#version 120\n"
    "uniform mat4 projection_matrix;\n"
    "uniform mat4 model_matrix;\n"
    "uniform mat4 view_matrix;\n"
    "attribute vec3 position;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = projection_matrix * model_matrix * view_matrix * vec4(position, 1.0);\n"
    "}\n"
);

const std::string strFragmentShader(
    "#version 120\n"
    "void main()\n"
    "{\n"
    "  gl_FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\n"
);

static GLuint program;
static GLuint buffer_object;
static GLfloat projection_matrix[16];
static GLfloat view_matrix[16];
static GLfloat model_matrix[16];

static const float vertexPositions[] = {
  0.75f, 0.75f, 0.0f, 1.0f,
  0.75f, -0.75f, 0.0f, 1.0f,
  -0.75f, -0.75f, 0.0f, 1.0f
};

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  glUniformMatrix4fv( glGetUniformLocation( program, "projection_matrix" ), 1, GL_FALSE, projection_matrix );
  glUniformMatrix4fv( glGetUniformLocation( program, "view_matrix" ), 1, GL_FALSE, view_matrix );
  glUniformMatrix4fv( glGetUniformLocation( program, "model_matrix" ), 1, GL_FALSE, model_matrix );

  glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer( glGetAttribLocation(program, "position"), 4, GL_FLOAT, GL_FALSE, 0, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
  glUseProgram(0);

  if(glGetError() != GL_NO_ERROR) {
    fprintf(stderr, "%s", gluErrorString(glGetError()));
  }

  glutSwapBuffers();
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
}

void idle(void) {
  glutPostRedisplay();
}

GLuint create_shader(GLenum shader_type, const std::string &source)
{
  GLuint shader = glCreateShader(shader_type);
  const char *shader_code = source.c_str();
  glShaderSource(shader, 1, &shader_code, NULL);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if(status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    GLchar *strInfoLog = new GLchar[infoLogLength + 1];
    glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

    const char *str_shader_type = NULL;
    switch(shader_type)
    {
      case GL_VERTEX_SHADER: str_shader_type = "vertex"; break;
      case GL_FRAGMENT_SHADER: str_shader_type = "fragment"; break;
    }

    fprintf(stderr, "Compile failure in %s shader:\n %s\n", str_shader_type, strInfoLog);
  }
  return shader;
}

void load_matrices() {

  projection_matrix[ 0] = 1; projection_matrix[ 1] = 0;
  projection_matrix[ 2] = 0; projection_matrix[ 3] = 0;

  projection_matrix[ 4] = 0; projection_matrix[ 5] = 1;
  projection_matrix[ 6] = 0; projection_matrix[ 7] = 0;

  projection_matrix[ 8] = 0; projection_matrix[ 9] = 0;
  projection_matrix[10] = 1; projection_matrix[11] = 0;

  projection_matrix[12] = 0; projection_matrix[13] = 0;
  projection_matrix[14] = 0; projection_matrix[15] = 1;

  view_matrix[ 0] = 1; view_matrix[ 1] = 0;
  view_matrix[ 2] = 0; view_matrix[ 3] = 0;

  view_matrix[ 4] = 0; view_matrix[ 5] = 1;
  view_matrix[ 6] = 0; view_matrix[ 7] = 0;

  view_matrix[ 8] = 0; view_matrix[ 9] = 0;
  view_matrix[10] = 1; view_matrix[11] = 0;

  view_matrix[12] = 0; view_matrix[13] = 0;
  view_matrix[14] = 0; view_matrix[15] = 1;

  model_matrix[ 0] = 1; model_matrix[ 1] = 0;
  model_matrix[ 2] = 0; model_matrix[ 3] = 0;

  model_matrix[ 4] = 0; model_matrix[ 5] = 1;
  model_matrix[ 6] = 0; model_matrix[ 7] = 0;

  model_matrix[ 8] = 0; model_matrix[ 9] = 0;
  model_matrix[10] = 1; model_matrix[11] = 0;

  model_matrix[12] = 0; model_matrix[13] = 0;
  model_matrix[14] = 0; model_matrix[15] = 1;

}

void init()
{
  load_matrices();
  GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, strVertexShader);
  GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, strFragmentShader);

  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  glGenBuffers(1, &buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);

  glutCreateWindow("UMOG");

  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  init();

  glutMainLoop();
  return 0;
}
