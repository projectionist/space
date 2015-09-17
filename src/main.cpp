#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <iostream>
#include <projection/projection.hpp>
#include <thread>

using namespace std;

projection::projection p;

// void signal_int(int signum)
void signal_int(int)
{
  p.stop();
}

int main()
{
  struct sigaction sigint;
  sigint.sa_handler = signal_int;
  sigemptyset(&sigint.sa_mask);
  sigint.sa_flags = SA_RESETHAND;
  sigaction(SIGINT, &sigint, NULL);

  p.run();

  return EXIT_SUCCESS;
}
