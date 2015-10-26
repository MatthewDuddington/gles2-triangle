// complete OpenGL triangle example.

#ifdef WIN32
  // This is the "GL extension wrangler"
  // Note: you will need to use the right version of glew32s.lib (32 bit vs 64 bit)
  #define GLEW_STATIC
  #include "GL/glew.h"
  #pragma comment(lib, "glew32s.lib")

  // This is the "GL utilities" framework for drawing with OpenGL
  #include "freeglut/include/GL/glut.h"
#else // (osx?)
  #include "GLUT/glut.h"
#endif

#include <math.h>

static GLuint program;
static int frame_number = 0;

// draw a triangle once!
class triangle
{
public:
  static void display()
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // corners of the triangle
    static const float vertices[] = {
       0.5f,  0.5f, // 2, GL_FLOAT (ie. vec2)
      -0.5f,  0.5f,
       0.5f, -0.5f
    };

    // set the attributes (assume pos=0)
    glVertexAttribPointer(
      0, 2, GL_FLOAT, GL_FALSE,
      2*sizeof(float), (GLvoid*)vertices
    );
    glEnableVertexAttribArray(0);

    GLuint offset_loc = glGetUniformLocation(program, "offset");
    glUniform2f(offset_loc, cosf(frame_number*0.1f), 0);
    frame_number++;

    // draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
  }

  static void reshape(int w, int h)
  {
    glViewport(0, 0, w, h);
  }

  static void timer(int value) {
    glutTimerFunc(30, timer, 1);
    glutPostRedisplay();
  }
};

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutCreateWindow("triangle");

  #ifdef WIN32
    // On windows, we need to do this to get modern OpenGL. Thanks, Microsoft.
    glewInit();
    if (!glewIsSupported("GL_VERSION_2_0") )
    {
      return 1;
    }
  #endif

  // vertex shader copies pos to glPosition
  const char *vs = "\
    attribute vec2 pos;\
    uniform vec2 offset;\
    void main() {\
      gl_Position = vec4(pos + offset, 0, 1);\
    }"
  ;
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vs, NULL);
  glCompileShader(vertex_shader);

  // fragment shader draws in red
  const char *fs = "\
    void main() {\
      gl_FragColor = vec4(1, 0, 0, 1);\
    }"
  ;
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fs, NULL);
  glCompileShader(fragment_shader);

  // combine fragment and vertex shader
  program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glUseProgram(program);

  glutReshapeFunc(triangle::reshape);
  glutDisplayFunc(triangle::display);
  glutTimerFunc(30, triangle::timer, 1);
  glutMainLoop();
  return 0;
}
