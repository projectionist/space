#pragma once

#include <cstdio>
#include <cstdlib>

#include <projection/display.hpp>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

namespace projection {

  void redraw(void *data, struct wl_callback *callback, uint32_t time);

  const struct wl_callback_listener frame_listener = {
    redraw
  };

  static const char *vert_shader_text =
    "uniform mat4 rotation;\n"
    "attribute vec4 pos;\n"
    "attribute vec4 color;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_Position = rotation * pos;\n"
    "  v_color = color;\n"
    "}\n";

  static const char *frag_shader_text =
    "precision mediump float;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = v_color;\n"
    "}\n";


  static GLuint create_shader(const char *source, GLenum shader_type)
  {
    GLuint shader;
    GLint status;

    shader = glCreateShader(shader_type);
    assert(shader != 0);

    glShaderSource(shader, 1, (const char **) &source, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
      char log[1000];
      GLsizei len;
      glGetShaderInfoLog(shader, 1000, &len, log);
      fprintf(stderr, "Error: compiling %s: %*s\n",
        shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment",
        len, log);
      exit(1);
    }

    return shader;
  }


  void redraw(void *data, struct wl_callback *callback, uint32_t time)
  {
    struct display *display = (struct display *)data;

    struct wl_region *region;

    assert(display->callback == callback);
    display->callback = NULL;

    if (callback)
      wl_callback_destroy(callback);

    if (!display->configured)
      return;

    display->drawing.redraw(display->geometry.width, display->geometry.height, time);

    // if (display->opaque || display->fullscreen) {
    region = wl_compositor_create_region(display->compositor);
    wl_region_add(region, 0, 0,
            display->geometry.width,
            display->geometry.height);
    wl_surface_set_opaque_region(display->surface, region);
    wl_region_destroy(region);


    display->callback = wl_surface_frame(display->surface);
    wl_callback_add_listener(display->callback, &frame_listener, display);

    eglSwapBuffers(display->egl.dpy, display->egl_surface);
  }

  void init_gl(struct display *display)
  {
    GLuint frag, vert;

    frag = create_shader(frag_shader_text, GL_FRAGMENT_SHADER);
    vert = create_shader(vert_shader_text, GL_VERTEX_SHADER);

    display->drawing.initialize(frag, vert);
  }
}
