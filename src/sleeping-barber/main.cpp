#include <iostream>

#include <thread>
#include <semaphore>
#include <mutex>

#include <string>

#include <queue>

#include <cpp-terminal/base.hpp>

#include "Person.hpp"
#include "VariantPicker.hpp"
#include "wait.hpp"

#define BARBER_WAIT_TIME 2500
#define BARBER_COLOR Term::color24_fg(97, 222, 42)

#define CUSTOMER_WAIT_TIME 2000
#define CUSTOMER_RAND_BOUNDS 2000
#define CUSTOMER_COLOR Term::color24_fg(255, 53, 184)

#define CHAIRS 3

std::counting_semaphore<CHAIRS> g_customers(0);

std::mutex g_customersQueueMtx;
std::queue<std::string> g_customersQueue;

class Barber : public Person {
public:
  Barber(const std::string& name) : Person(name, BARBER_COLOR, BARBER_WAIT_TIME) {}

  void start() const override {
    std::string customerName;

    while (true) {
      if (!g_customers.try_acquire()) {
        print("falls asleep");
        g_customers.acquire();
        print("wakes up");
      }

      {
        std::lock_guard lock(g_customersQueueMtx);
        customerName = CUSTOMER_COLOR + g_customersQueue.front() + Term::color(Term::fg::reset);
        g_customersQueue.pop();
      }

      print("starts doing a haircut for " + customerName);
      wait(m_waitTime);

      print("finishes haircut for " + customerName);
    }
  }
};

class Customer : public Person {
public:
  Customer(const std::string& name) : Person(name, CUSTOMER_COLOR, CUSTOMER_WAIT_TIME) {}

  void start() const override {
    std::lock_guard lock(g_customersQueueMtx);

    if (g_customersQueue.size() == CHAIRS) {
      print("has nowhere to sit so they leaves");

      return;
    }

    print("waits for a haircut");
    g_customersQueue.push(m_name);
    g_customers.release();
  }
};

int main() {
  VariantPicker nameGenerator {
    "Liam",
    "Olivia",
    "Noah",
    "Emma",
    "Oliver",
    "Charlotte",
    "Elijah",
    "Amelia",
    "James",
    "Ava",
  };

  std::thread barber([&nameGenerator]() {
    Barber barber(nameGenerator.getOne());

    barber.start();
  });

  while (true) {
    wait(CUSTOMER_WAIT_TIME, CUSTOMER_RAND_BOUNDS);

    std::thread customer([&nameGenerator]() {
      Customer customer(nameGenerator.getOne());

      customer.start();
    });

    customer.detach();
  }

  barber.join();

  return 0;
}
