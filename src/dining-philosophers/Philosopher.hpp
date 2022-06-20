#pragma once

#include <thread>
#include <mutex>
#include <shared_mutex>
#include <semaphore>

#include <chrono>

#include <string>
#include <sstream>
#include <cstdio>

#include <iterator>

#include <cpp-terminal/base.hpp>

#include "wait.hpp"
#include "CircularList.hpp"
#include "Person.hpp"

#define PHILOSOPHER_COLOR Term::color24_fg(53, 126, 199)
#define PHILOSOPHER_WAIT_TIME 3000

class Philosopher : public Person {
public:
  using pos_t = CircularList<Philosopher>::Iterator;

  Philosopher(const std::string& name)
    : m_state(State::THINKING), m_canEat(0), Person(name, PHILOSOPHER_COLOR, PHILOSOPHER_WAIT_TIME) {}
  Philosopher(Philosopher&& other)
      : m_state(std::move(other.m_state)),
        m_canEat(0),
        Person(std::move(other)) {}

  void setPos(const pos_t& pos) {
    m_pos = pos;
  }

  std::shared_mutex& getAccessMtx() const { return m_access; }

  void start() const override {
    while (true) {
      think();
      hungry();
      tryTakeForks();
      eat();
      putForks();
    }
  }

private:
  void think() const {
    print("started to think");

    wait(m_waitTime);
  }

  void hungry() const {
    print("is hungry");

    setState(State::HUNGRY);
  }

  void tryTakeForks() const {
    auto prevPhilosopher = std::prev(m_pos);
    auto nextPhilosopher = std::next(m_pos);
    bool cond = false;

    std::shared_lock prevAccess(prevPhilosopher->getAccessMtx(), std::defer_lock);
    std::shared_lock nextAccess(nextPhilosopher->getAccessMtx(), std::defer_lock);
    std::shared_lock thisAccess(m_access, std::defer_lock);

    {
      std::scoped_lock lock(prevAccess, nextAccess, thisAccess);

      cond = (
        m_state == State::HUNGRY
        && prevPhilosopher->m_state != State::EATING
        && nextPhilosopher->m_state != State::EATING
      );
    }

    if (cond) {
      setState(State::EATING);
      // Literally saying that philosopher can eat now
      m_canEat.release();
    }
  }

  void eat() const {
    // If philosopher doesn't have 2 forks he'll wait until he gets em
    m_canEat.acquire();

    print("started eating");

    wait(m_waitTime);
  }

  void putForks() const {
    print("finished eating");

    setState(State::THINKING);

    std::prev(m_pos)->tryTakeForks();
    std::next(m_pos)->tryTakeForks();
  }

  enum State {
    THINKING,
    HUNGRY,
    EATING,
  };

  void setState(State newState) const {
    std::lock_guard lock(m_access);

    m_state = newState;
  }

  std::string m_name;
  mutable State m_state;
  pos_t m_pos;

  mutable std::binary_semaphore m_canEat;
  mutable std::shared_mutex m_access;
};
