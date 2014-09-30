#ifndef UTILITY_H
#define UTILITY_H

/**
 * File: utility.h
 * ---------------
 * Defines a convenience function, which removes whitespace from 
 * the ends of the supplied string argument.
 **/

#include <string>

namespace {
  void TrimString(std::string &s) {
    const std::string whitespace(" \f\n\r\t\v");
    std::string::size_type first_idx = s.find_first_not_of(whitespace);
    if (first_idx == std::string::npos) {
      s.clear();
    } else {
      std::string::size_type last_idx = s.find_last_not_of(whitespace);
      s = s.substr(first_idx, last_idx - first_idx + 1);
    }
  }
}

#endif
