#include <iostream>
#include <sstream>

#include <functional>
#include <random>

#include <chrono>
#include <thread>
#include <mutex>
#include <semaphore>

#include <vector>
#include <string>

#include <memory>

#include <cpp-terminal/base.hpp>

#include "CircularList.hpp"

#include "Philosopher.hpp"
#include "VariantPicker.hpp"

int main() {
  VariantPicker nameGenerator {
    "Confucius",
    "Plato",
    "Aristotle",
    "Karl Marx",
    "Socrates",
    "Lao-Tzu",
    "Immanuel Kant",
  };
  CircularList<Philosopher> list;

  size_t philosopherCount;
  std::cout << "How many philosophers do you want? [5] ";

  if (std::cin.peek() == '\n' || !(std::cin >> philosopherCount)) {
    philosopherCount = 5;
  }

  std::cout << "At the table were sitting:" << std::endl;

  auto it = list.end();

  for (size_t i = 0; i < philosopherCount; i++) {
    list.insert(Philosopher(nameGenerator.getOne()));

    if (it == list.end()) {
      it = list.begin();
    } else {
      std::advance(it, 1);
    }

    it->setPos(it);

    std::cout << PHILOSOPHER_COLOR << it->getName() << Term::color(Term::fg::reset) << " <=> ";

    if (i + 1 == philosopherCount) {
      std::cout << PHILOSOPHER_COLOR << std::next(it)->getName() << Term::color(Term::fg::reset);
    }
  }

  std::cout << Term::color(Term::fg::reset) << std::endl << std::endl;

  std::vector<std::thread> threads;
  list.forEach([&threads](Philosopher &philosopher) {
    threads.push_back(std::thread([&philosopher]() {
      philosopher.start();
    }));
  });

  for (auto& t : threads) {
    t.join();
  }

  return 0;
}
