#include <iostream>
#include <cstdio>

#include <string>

#include <vector>

#include <memory>

#include <thread>
#include <shared_mutex>

#include <cpp-terminal/base.hpp>

#include "Person.hpp"
#include "wait.hpp"
#include "VariantPicker.hpp"

#define READERS_WAIT_TIME 3000
#define WRITERS_WAIT_TIME 4000

std::shared_mutex g_mtx;

class Reader : public Person {
public:
  Reader(const std::string& name) : Person(name, Term::color24_fg(255, 53, 184), READERS_WAIT_TIME) {}

  void start() const override {
    while (true) {
      print("wants to read");

      {
        std::shared_lock lock(g_mtx);
        print("started reading");

        wait(m_waitTime);
        print("finished reading");
      }

      wait(m_waitTime);
    }
  }
};

class Writer : public Person {
public:
  Writer(const std::string& name) : Person(name, Term::color24_fg(97, 222, 42), WRITERS_WAIT_TIME) {}

  void start() const override {
    while (true) {
      print("wants to write");

      {
        std::lock_guard lock(g_mtx);
        print("started writing");

        wait(m_waitTime);
        print("finished writing");
      }

      wait(m_waitTime);
    }
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


  size_t readersCount;
  std::cout << "How many readers do you want? [2] ";

  if (std::cin.peek() == '\n' || !(std::cin >> readersCount)) {
    readersCount = 2;
  }

  std::cin.get();

  size_t writersCount;
  std::cout << "And how many writers? [1] ";

  if (std::cin.peek() == '\n' || !(std::cin >> writersCount)) {
    writersCount = 1;
  }

  std::cin.get();

  std::vector<std::unique_ptr<Person>> persons;

  for (size_t i = 0; i < readersCount; i++) {
    persons.push_back(std::make_unique<Reader>(nameGenerator.getOne()));
  }

  for (size_t i = 0; i < writersCount; i++) {
    persons.push_back(std::make_unique<Writer>(nameGenerator.getOne()));
  }


  std::vector<std::thread> threads;

  for (const auto& person : persons) {
    threads.push_back(std::thread([&person]() {
      person->start();
    }));
  };

  for (auto& t : threads) {
    t.join();
  }

  return 0;
}
