#include <csignal>
#include <memory>

#include <projection/start.hpp>
#include <scratch/scratch.hpp>

using namespace std;

int main()
{
  scratch::scratch s;
  projection::play(s);

  /*

  auto p = uniq_ptr<projection::projection>();
  projection::start(p);

  */

  return EXIT_SUCCESS;
}
