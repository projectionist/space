#include <projection/drawing.hpp>

using namespace std;
using namespace std::chrono;

namespace projection {
  drawing::drawing() {}

  void drawing::initialize(int width, int height)
  {
    start_time = std::chrono::high_resolution_clock::now();
    _w = width;
    _h = height;
    _elapsed = 0.0;
    _initialized = true;
    setup();
  }

  void drawing::redraw()
  {
    auto now = high_resolution_clock::now();
    auto time_span = duration_cast<duration<float>>(now - start_time);
    _elapsed = time_span.count();
    _frames++;

    draw();
  }
}
