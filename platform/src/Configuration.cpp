/** @file Configuration.cpp
 *  Configuration class implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 27. November 2015
 */

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "Configuration.hpp"

namespace rtx {

  Configuration::Configuration() {

  }

  void Configuration::init(std::string fn) {
    std::cout << "[Configuration::init]Load " << fn << std::endl;

    boost::property_tree::ini_parser::read_ini(fn, pt);
  }

  int Configuration::getInt(std::string k) {
    return pt.get<int>(k);
  }

  std::string Configuration::getStr(std::string k) {
    return pt.get<std::string>(k);
  }

}
