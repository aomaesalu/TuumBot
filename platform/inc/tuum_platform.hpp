/** @file tuum_platform.hpp
 *  General application dependencies module.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 27. November 2015
 */

#ifndef TUUM_PLATFORM_H
#define TUUM_PLATFORM_H

#include "Configuration.hpp"

namespace rtx {

  // Global configuration object
  extern Configuration gC;

  int init(int argc, char *argv[]);
}

#endif
