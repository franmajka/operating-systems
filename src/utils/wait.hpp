#include <random>
#include <thread>

#define WAIT_TIME 3000
#define RAND_BOUNDS 1000

// Duration is time in milliseconds
void wait(size_t duration = WAIT_TIME, size_t randBounds = RAND_BOUNDS) {
  static std::mt19937 rnd(std::time(nullptr));
  auto waitTime = std::uniform_int_distribution<>(duration - randBounds, duration + randBounds)(rnd);
  std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
}
