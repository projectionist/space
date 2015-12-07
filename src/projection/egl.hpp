#pragma once

#include <EGL/egl.h>
#include <projection/display.hpp>


namespace projection {
  void init_egl(struct display *display)
  {
    static const EGLint context_attribs[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };

    EGLint config_attribs[] = {
      EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
      EGL_RED_SIZE, 1,
      EGL_GREEN_SIZE, 1,
      EGL_BLUE_SIZE, 1,
      EGL_ALPHA_SIZE, 1,
      EGL_DEPTH_SIZE, 1,
      EGL_STENCIL_SIZE, 1,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_NONE
    };

    EGLint major, minor, n;
    EGLBoolean ret;

    display->egl.dpy = eglGetDisplay((EGLNativeDisplayType)display->display);
    assert(display->egl.dpy);

    ret = eglInitialize(display->egl.dpy, &major, &minor);
    assert(ret == EGL_TRUE);
    ret = eglBindAPI(EGL_OPENGL_ES_API);
    assert(ret == EGL_TRUE);

    ret = eglChooseConfig(display->egl.dpy, config_attribs,
              &display->egl.conf, 1, &n);
    assert(ret && n == 1);

    display->egl.ctx = eglCreateContext(display->egl.dpy,
                display->egl.conf,
                EGL_NO_CONTEXT, context_attribs);
    assert(display->egl.ctx);
  }

  void fini_egl(struct display *display)
  {
    /* Required, otherwise segfault in egl_dri2.c: dri2_make_current()
     * on eglReleaseThread(). */
    eglMakeCurrent(display->egl.dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglTerminate(display->egl.dpy);
    eglReleaseThread();
  }
}
