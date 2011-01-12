#include <string>
#include <vector>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#endif

const std::string strVertexShader(
    "#version 120\n"
    "varying vec4 position;\n"
    "void main()\n"
    "{\n"
    "  gl_Position = position;\n"
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

static const float vertexPositions[] = {
  0.75f, 0.75f, 0.0f, 1.0f,
  0.75f, -0.75f, 0.0f, 1.0f,
  -0.75f, -0.75f, 0.0f, 1.0f
};

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
  glUseProgram(0);

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

    fprintf(stderr, "Compile failure: %s\n", strInfoLog);
  }
  return shader;
}


void init()
{
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
