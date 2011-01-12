#include <GLUT/glut.h>

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glutSwapBuffers();
}

void reshape(int width, int height) {
  glViewport(0, 0, width, height);
}

void idle(void) {
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(640, 480);

  glutCreateWindow("UMOG");

  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}
