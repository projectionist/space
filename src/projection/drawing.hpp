#pragma once

#include <chrono>

namespace projection {
  class drawing {
    int _w = -1;
    int _h = -1;
    float _elapsed = 0.0f;
    bool _initialized = false;
    uint64_t _frames = 0;
    std::chrono::high_resolution_clock::time_point start_time;
  public:
    explicit drawing();

    int width() { return _w; };
    int height() { return _h; };
    float elapsed() { return _elapsed; };
    bool initialized() { return _initialized; };
    uint64_t frames() { return _frames; };
    float frame_rate() {  return _frames / _elapsed; };

    void initialize(int width, int height);
    virtual void setup() =0;

    void redraw();
    virtual void draw() =0;
  };
}
