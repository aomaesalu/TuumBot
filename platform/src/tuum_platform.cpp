/** @file tuum_platform.cpp
 *  General application dependencies module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 27. November 2015
 */

#include <string>

#include "tuum_platform.hpp"

#include <boost/program_options.hpp>

using namespace boost::program_options;

namespace rtx {

  Configuration gC;

  void load_config(std::string fn) {
    gC.init(fn);
  }

  int init(int argc, char *argv[]) {
    /*options_description desc{"Options"};
    desc.add_options()
      ("help,h", "Help screen")
      ("config", value<std::string>()->notifier(load_config), "Configuration file");
    variables_map vm;
    store(parse_command_line(argc, argv, desc), vm);
    notify(vm);*/
    load_config("rtex.ini");
  }
}
