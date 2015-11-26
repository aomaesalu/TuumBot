/** @file Configuration.hpp
 *  General Configuration class.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 27. November 2015
 */

#ifndef RTX_CNF_H
#define RTX_CNF_H

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace rtx {

  class Configuration {
  public:
    enum Key {
      MAINBOARD_ID,
      MOTOR_ID_SEQ,
      MOTOR_COUNT,
    };

  private:
    std::map<Key, std::string> values;

  public:
    Configuration();

    void init(std::string);
  };

}

#endif // RTX_CNF_H
