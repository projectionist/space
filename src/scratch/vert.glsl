#define M_PI 3.1415926
attribute float a_u;

void main()
{
  float u = mod(a_u, 5.0);
  vec2 xy = (u / (2.0 * M_PI)) * vec2(cos(u), sin(u));
  gl_Position = vec4(xy, 0.0, 1.0);
}
