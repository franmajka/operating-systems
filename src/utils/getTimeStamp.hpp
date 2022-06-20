#pragma once

#include <string>
#include <sstream>
#include <iomanip>

#include <cpp-terminal/base.hpp>

#include "Timer.hpp"

std::string getTimeStamp() {
  static Timer timer;

  auto t = timer.getDiff();

  std::stringstream ss;
  ss
    << Term::color(Term::fg::red) << "["
    << std::put_time(std::localtime(&t), "%M:%S")
    << "]" << Term::color(Term::fg::reset) ;

  return ss.str();
}
