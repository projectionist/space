#include <csignal>
#include <memory>

#include <projection/projection.hpp>

using namespace std;

static shared_ptr<projection::projection> p = make_shared<projection::projection>();

void handle_signal(int signal)
{
  switch (signal) {
    case SIGHUP:
    case SIGINT:
    case SIGQUIT:
    case SIGKILL:
      p->stop();
  }
}

void handle_signals()
{
  struct sigaction sigint;
  sigint.sa_handler = &handle_signal;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, NULL);
}

int main()
{
  auto p = make_shared<projection::projection>();

  p->run();

  return EXIT_SUCCESS;
}
