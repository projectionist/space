#define M_PI 3.1415926
attribute float a_u;
attribute float a_offset;

void main()
{
  float r = a_u / 2.0;
  float phi = ((3.0 * 2.0 * a_u) + a_offset) * M_PI;

  vec2 xy = r * vec2(cos(phi), sin(phi));
  gl_Position = vec4(xy, 0.0, 1.0);
}
