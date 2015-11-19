attribute float a_progress;

void main()
{
  gl_Position = vec4(a_progress, a_progress, 0.0, 1.0);
}
