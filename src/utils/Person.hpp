#pragma once

#include <iostream>
#include <string>

#include "getTimeStamp.hpp"

class Person {
public:
  Person(const std::string& name, const std::string& color, size_t waitTime)
    : m_name(name), m_color(color), m_waitTime(waitTime) {}
  Person(Person&& other)
    : m_name(std::move(other.m_name)),
    m_color(std::move(other.m_color)),
    m_waitTime(other.m_waitTime) {}

  virtual void start() const = 0;

  std::string getName() const {
    return m_name;
  }

protected:
  void print(const std::string text) const {
    std::cout <<
      getTimeStamp() + " " +
      m_color + m_name + Term::color(Term::fg::reset) + " " +
      text + "\n";
  }

  std::string m_name;
  std::string m_color;
  size_t m_waitTime;
};
