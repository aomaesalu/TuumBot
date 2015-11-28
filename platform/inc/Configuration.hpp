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
  private:
    boost::property_tree::ptree pt;

  public:
    Configuration();

    void init(std::string);

    int get(std::string);
  };

}

#endif // RTX_CNF_H
