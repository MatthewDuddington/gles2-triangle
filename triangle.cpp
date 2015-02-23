// complete OpenGL triangle example.

#pragma comment(lib, "glew32s.lib")
#define FREEGLUT_STATIC
#define GLEW_STATIC

#include "GL/glew.h"
#include "GL/glut.h"

// draw a triangle once!
class triangle
{
public:
  static void display()
  {
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

    // draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
  }

  static void reshape(int w, int h)
  {
    glViewport(0, 0, w, h);
  }
};

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutCreateWindow("triangle");

  // On windows, we need to do this to get modern OpenGL. Thanks, Microsoft.
  glewInit();
  if (!glewIsSupported("GL_VERSION_2_0") )
  {
    return 1;
  }

  // vertex shader copies pos to glPosition
  const char *vs = "attribute vec2 pos; void main() { gl_Position = vec4(pos, 0, 1); }";
  GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vs, NULL);
  glCompileShader(vertex_shader);

  // fragment shader draws in red
  const char *fs = "void main() { gl_FragColor = vec4(1, 0, 0, 1); }";
  GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fs, NULL);
  glCompileShader(fragment_shader);

  // combine fragment and vertex shader
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);
  glUseProgram(program);

  glutReshapeFunc(triangle::reshape);
  glutDisplayFunc(triangle::display);
  glutMainLoop();
  return 0;
}
