#pragma once

#include <chrono>
#include <cassert>
#include <iostream>

#define check_gl_error() { const auto gl_error_code = glGetError(); if (gl_error_code != GL_NO_ERROR) { cerr << "GL_ERROR: " << gl_error_code_name(gl_error_code) << endl; assert(gl_error_code == GL_NO_ERROR); } }

namespace projection {
  class drawing {
    float _w = -1;
    float _h = -1;
    float _elapsed = 0.0f;
    bool _initialized = false;
    uint64_t _frames = 0;
    std::chrono::high_resolution_clock::time_point start_time;
  public:
    explicit drawing();

    float width() { return _w; };
    float height() { return _h; };
    float elapsed() { return _elapsed; };
    bool initialized() { return _initialized; };
    uint64_t frames() { return _frames; };
    float frame_rate() {  return _frames / _elapsed; };

    void initialize(float width, float height);
    virtual void setup() =0;

    void redraw();
    virtual void draw() =0;

    static std::string gl_error_code_name(const unsigned int error_code);
  };
}
