#include <csignal>
#include <memory>

#include <projection/start.hpp>
#include <scratch/scratch.hpp>

using namespace std;

int main()
{
  auto s = make_shared<scratch::scratch>();
  projection::play(s);

  return EXIT_SUCCESS;
}
