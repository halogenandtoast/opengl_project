#include <string>
#include <vector>

#include <GL/glew.h>
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

class Demo{
  public:
    GLuint program, buffer_object, vertex_shader, fragment_shader;
} demo;

static GLfloat projection_matrix[16] = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};
static GLfloat model_view_matrix[16] = {
  1, 0, 0, 0,
  0, 1, 0, 0,
  0, 0, 1, 0,
  0, 0, 0, 1
};

static const float vertexData[] = {
  0.75f, 0.75f, 0.0f, 1.0f,
  0.75f, -0.75f, 0.0f, 1.0f,
  -0.75f, -0.75f, 0.0f, 1.0f
};

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(demo.program);
  glUniformMatrix4fv( glGetUniformLocation( demo.program, "projection_matrix" ), 1, GL_FALSE, projection_matrix );
  glUniformMatrix4fv( glGetUniformLocation( demo.program, "model_view_matrix" ), 1, GL_FALSE, model_view_matrix );
  glBindBuffer(GL_ARRAY_BUFFER, demo.buffer_object); // bind buffer
  glEnableVertexAttribArray(0);
  glVertexAttribPointer( glGetAttribLocation(demo.program, "position"), 4, GL_FLOAT, GL_FALSE, 0, 0);
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

GLuint create_shader(GLenum shader_type, const char* source_file)
{
  GLuint shader = glCreateShader(shader_type);
  GLchar *source;
  GLint filesize;

  FILE *file = fopen(source_file, "r");
  fseek(file, 0, SEEK_END);
  filesize = ftell(file);
  fseek(file, 0, SEEK_SET);
  source = (GLchar *)malloc(sizeof(GLchar)*(filesize+1));
  fread(source, 1, filesize, file);
  fclose(file);
  source[filesize] = '\0';

  glShaderSource(shader, 1, (const GLchar **)&source, NULL);
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

void make_program(GLint vertex_shader, GLint fragment_shader) {
  demo.program = glCreateProgram();
  glAttachShader(demo.program, vertex_shader);
  glAttachShader(demo.program, fragment_shader);
  glLinkProgram(demo.program);
}

void init()
{
  GLuint vertex_shader = create_shader(GL_VERTEX_SHADER, "shaders/main.v.glsl");
  GLuint fragment_shader = create_shader(GL_FRAGMENT_SHADER, "shaders/main.f.glsl");
  make_program(vertex_shader, fragment_shader);

  glGenBuffers(1, &demo.buffer_object);
  glBindBuffer(GL_ARRAY_BUFFER, demo.buffer_object);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);

  glutCreateWindow("UMOG");

  glewInit();

  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  init();

  glutMainLoop();
  return 0;
}
