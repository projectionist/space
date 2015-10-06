#include <csignal>
#include <memory>

#include <projection/play.hpp>
#include <scratch/scratch.hpp>

using namespace std;

int main()
{
  scratch::scratch *s = new scratch::scratch();
  shared_ptr<projection::drawing> drawing = shared_ptr<projection::drawing>(s);
  projection::play(drawing);

  return EXIT_SUCCESS;
}
