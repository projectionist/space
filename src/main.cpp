#include <csignal>
#include <memory>

#include <projection/projection.hpp>

using namespace std;

static shared_ptr<projection::projection> p = make_shared<projection::projection>();

int main()
{
  p->run();

  return EXIT_SUCCESS;
}
