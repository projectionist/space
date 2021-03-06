#define M_PI 3.1415926

attribute vec4 a_position;
uniform mat4 u_mvp;

void main()
{
  gl_Position = u_mvp * a_position;
  gl_PointSize = 1.0;
}
