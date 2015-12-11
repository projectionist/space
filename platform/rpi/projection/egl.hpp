#pragma once

#include <bcm_host.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "rpi_display.hpp"

namespace projection {
  namespace rpi {

    void init_egl(rpi_display *egl)
    {
      bcm_host_init();

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
      int32_t success = 0;

      static EGL_DISPMANX_WINDOW_T nativewindow;

      DISPMANX_ELEMENT_HANDLE_T dispman_element;
      DISPMANX_DISPLAY_HANDLE_T dispman_display;
      DISPMANX_UPDATE_HANDLE_T dispman_update;
      VC_RECT_T dst_rect;
      VC_RECT_T src_rect;

      egl->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
      assert(egl->display != EGL_NO_DISPLAY);

      ret = eglInitialize(egl->display, &major, &minor);
      assert(ret == EGL_TRUE);

      ret = eglBindAPI(EGL_OPENGL_ES_API);
      assert(ret == EGL_TRUE);

      ret = eglChooseConfig(egl->display, config_attribs,
                &egl->config, 1, &n);
      assert(ret && n == 1);

      egl->context = eglCreateContext(egl->display,
                  egl->config,
                  EGL_NO_CONTEXT, context_attribs);
      assert(egl->context != EGL_NO_CONTEXT);

      // create an EGL window surface
      success = graphics_get_display_size(0 /* LCD */, &egl->screen_width, &egl->screen_height);
      assert( success >= 0 );

      dst_rect.x = 0;
      dst_rect.y = 0;
      dst_rect.width = egl->screen_width;
      dst_rect.height = egl->screen_height;

      src_rect.x = 0;
      src_rect.y = 0;
      src_rect.width = egl->screen_width << 16;
      src_rect.height = egl->screen_height << 16;

      dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
      dispman_update = vc_dispmanx_update_start( 0 );

      dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display,
         0/*layer*/, &dst_rect, 0/*src*/,
         &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

      nativewindow.element = dispman_element;
      nativewindow.width = egl->screen_width;
      nativewindow.height = egl->screen_height;
      vc_dispmanx_update_submit_sync(dispman_update);

      egl->surface = eglCreateWindowSurface(egl->display, egl->config, &nativewindow, NULL );
      assert(egl->surface != EGL_NO_SURFACE);

      // connect the context to the surface
      ret = eglMakeCurrent(egl->display, egl->surface, egl->surface, egl->context);
      assert(EGL_FALSE != ret);

      egl->drawing->initialize(egl->screen_width, egl->screen_height);
    }

    void fini_egl(rpi_display *egl)
    {
      // clear screen
      // glClear( GL_COLOR_BUFFER_BIT );
      eglSwapBuffers(egl->display, egl->surface);

      eglMakeCurrent( egl->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
      eglDestroySurface( egl->display, egl->surface );
      eglDestroyContext( egl->display, egl->context );

      eglTerminate(egl->display);
      eglReleaseThread();
    }
  }
}
